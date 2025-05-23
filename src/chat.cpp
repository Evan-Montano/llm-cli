#include <iostream>
#include <string>
#include <fstream>
#include <deque>
#include <cctype>
#include <filesystem>
#include <vector>
#include <random>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "train.hpp"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

static struct termios orig_termios;

// Restore original terminal settings
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}
#endif

int getKey() {
#ifdef _WIN32
    return _getch();
#else
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        return static_cast<unsigned char>(c);
    }
    return -1;
#endif
}

using json = nlohmann::json;
namespace fs = std::filesystem;
const int BUFFER_SIZE = 16;
std::string buffer;
json tree;

void LoadTreeToMemory() {
    std::ifstream inFile("../data/character_modal.cbor", std::ios::binary);
    if (inFile && inFile.peek() != std::ifstream::traits_type::eof()) {
        try {
            std::vector<uint8_t> treeBuffer(std::istreambuf_iterator<char>(inFile), {});
            tree = json::from_cbor(treeBuffer);
        }
        catch (const json::parse_error& e) {
            std::cerr << "\nError parsing CBOR: " << e.what() << "\n";
            return;
        }
    }
    else {
        return;
    }
}

char sample_next_char(const json& node) {

    if (!node.contains("nxt")) return '\0';
    const auto& nxt = node["nxt"];
    // Build cumulative distribution
    std::vector<std::pair<char, int>> entries;
    int total = 0;
    for (auto it = nxt.begin(); it != nxt.end(); ++it) {
        char c = it.key()[0];
        int count = it.value().get<int>();
        entries.emplace_back(c, count);
        total += count;
    }
    if (total == 0) return '\0';

    std::uniform_int_distribution<int> dist(0, total - 1);
    static thread_local std::mt19937 gen(std::random_device{}());
    int pick = dist(gen);

    // find bucket
    for (auto& e : entries) {
        if (pick < e.second) return e.first;
        pick -= e.second;
    }
    return entries.back().first;
}

// Generate up to maxLen characters, starting from the given context
std::string generate_response(const std::string& context, size_t maxLen = 100) {
    std::string out;
    std::deque<char> window;
    size_t start = context.size() > BUFFER_SIZE
        ? context.size() - BUFFER_SIZE
        : 0;
    for (size_t i = start; i < context.size(); ++i)
        window.push_back(context[i]);

    for (size_t i = 0; i < maxLen; ++i) {
        // --- BACKOFF TRAVERSAL ---
        json const* node = nullptr;
        for (int depth = std::min((int)window.size(), BUFFER_SIZE); depth > 0; --depth) {
            node = &tree;
            bool ok = true;
            for (int j = window.size() - depth; j < (int)window.size(); ++j) {
                std::string key(1, window[j]);
                if (node->contains("cldrn") && (*node)["cldrn"].contains(key)) {
                    node = &(*node)["cldrn"][key];
                }
                else {
                    ok = false;
                    break;
                }
            }
            if (ok && node->contains("nxt")) break;
            node = nullptr;
        }
        if (!node) node = &tree;  // ultimate fallback

        char next = sample_next_char(*node);
        if (next == '\0') break;
        out.push_back(next);

        if (window.size() == BUFFER_SIZE) window.pop_front();
        window.push_back(next);
    }
    return out;
}

// In RunEngine()
void RunEngine() {
    // assume 'buffer' holds the full user prompt line
    std::string context = "User: " + buffer + "\nAI:";
    std::string reply = generate_response(context, /*maxLen=*/200);
    std::cout << "  " << reply << "\n\n";
    // clear buffer for next turn
    buffer.clear();
}

void InitializeChatModule() {
#ifndef _WIN32
    enableRawMode();
#endif

    std::cout << "Getting things ready..\n";
    LoadTreeToMemory();
    if (tree.contains("\n")
        && tree["\n"].contains("cldrn")
        && tree["\n"]["cldrn"].contains("A")
        && tree["\n"]["cldrn"]["A"].contains("cldrn")
        && tree["\n"]["cldrn"]["A"]["cldrn"].contains("I")
        && tree["\n"]["cldrn"]["A"]["cldrn"]["I"].contains("nxt"))
    {
        std::cout << "'\\nAI:' seen, next-char count = "
            << tree["\n"]["cldrn"]["A"]["cldrn"]["I"]["nxt"].size()
            << "\n";
    }
    else {
        std::cerr << "'\\nAI:' node missing or empty—did you retrain on dialogues.txt?\n";
    }
    std::cout << "Chat with DownsGPT (Esc to quit)\n\n";

    while (true) {
        std::cout << ">> " << std::flush;

        // character-by-character input
        while (true) {
            int key = getKey();
            if (key == 27) {
                std::cout << "\n\nUpdating modal..\n";
                // Refresh cbol file with tree
                std::ofstream outFile("../data/character_modal.cbor", std::ios::binary);
                if (!outFile) {
                    std::cerr << "Error opening file for write\n";
                    return;
                }
                std::vector<uint8_t> output = json::to_cbor(tree);
                outFile.write(reinterpret_cast<const char*>(output.data()), output.size());

                std::cout << "Exit..\n";
                return;
            }
            else if (key == 10 || key == 13) {
                std::cout << '\n';
                break;
            }
            else if (key == 127 || key == 8) {
                if (!buffer.empty()) {
                    buffer.pop_back();
                    std::cout << "\b \b" << std::flush;
                }
            }
            else if (key >= 32 && key <= 126) {
                buffer.push_back(static_cast<char>(key));
                std::cout << static_cast<char>(key) << std::flush;
            }

        }

        // Use user's input for modal training
        ProcessSubstring(buffer, tree);

        // Run engine for modal output
        RunEngine();
    }
}