// src/train.cpp
#include "train.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <cctype>
#include <filesystem>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

const int BUFFER_SIZE = 7;

void ScrubPathInput(std::string& path) {
    if (path.front() == '"' && path.back() == '"') {
        path.erase(0, 1);
        path.pop_back();
    }
}

std::string byteToHexString(char byte) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(byte));
    return ss.str();
}

void ProcessSubstring(const std::string& str, json& tree) {
    if (str.size() < 2) return;

    std::string firstChar = byteToHexString(str[0]);
    json* node = &tree[firstChar];

    for (size_t i = 0; i + 1 < str.size(); ++i) {
        std::string nextChar = byteToHexString(str[i + 1]);

        if (!(*node).contains("next_chars") || !(*node)["next_chars"].is_object()) {
            (*node)["next_chars"] = json::object();
        }

        // Increment the counter
        (*node)["next_chars"][nextChar] =
            (*node)["next_chars"].value(nextChar, 0) + 1;

        if (!(*node).contains("children") || !(*node)["children"].is_object()) {
            (*node)["children"] = json::object();
        }

        // Descend into the child for nextChar
        node = &(*node)["children"][nextChar];
    }
}


std::string NormalizePath(const std::string& raw) {
#ifndef _WIN32
    if (raw.size() >= 2 && std::isalpha(raw[0]) && raw[1] == ':') {
        char drive = std::tolower(raw[0]);
        std::string rest = raw.substr(2);
        for (auto& c : rest) if (c == '\\') c = '/';
        return "/mnt/" + std::string(1, drive) + rest;
    }
#endif

    std::string s = raw;
    for (auto& c : s) if (c == '/') c = '\\';
    return s;
}

void ParseFileByChar(const std::string& filePath) {
    fs::path p{ filePath };

    // Load the JSON tree into memory
    std::cout << "Loading training data from disk..\n";
    json tree;
    {
        std::ifstream inFile("../data/character_modal.json");
        if (inFile && inFile.peek() != std::ifstream::traits_type::eof()) {
            //only attempt parse if file exists AND is non-empty
            try {
                inFile >> tree;
            }
            catch (const json::parse_error& e) {
                std::cerr << "Error parsing JSON: " << e.what() << "\n";
                tree = json::object();
            }
        }
        else {
            tree = json::object();
        }
    }
    std::cout << "Training data loaded..\n";

    std::cout << "Loading input file..\n";
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return;
    }

    std::cout << "Working..\n";
    std::deque<char> window;
    char characterInFile;

    for (int i = 0; i < BUFFER_SIZE && file.get(characterInFile); ++i) {
        window.push_back(characterInFile);
    }

    if (window.size() == BUFFER_SIZE) {
        std::string subString(window.begin(), window.end());
        ProcessSubstring(subString, tree);
    }

    while (file.get(characterInFile)) {
        window.pop_front();
        window.push_back(characterInFile);

        std::string subString(window.begin(), window.end());
        ProcessSubstring(subString, tree);
    }

    std::ofstream outFile("../data/character_modal.json");
    if (!outFile) {
        std::cerr << "Error opening file for write\n";
        return;
    }
    outFile << tree.dump(1) << "\n";

    std::cout << "Finish!";
}

void InitializeTrainModule() {
    std::string textFilePath;
    std::cout << "Enter the path to your training data (.txt filetype only) >>";
    std::getline(std::cin, textFilePath);
    
    ScrubPathInput(textFilePath);
    textFilePath = NormalizePath(textFilePath);
    
    ParseFileByChar(textFilePath);
}