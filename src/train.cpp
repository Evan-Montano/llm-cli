// src/train.cpp
#include "train.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

void ScrubPathInput(std::string& path) {
    if (path.front() == '"' && path.back() == '"') {
        path.erase(0, 1);
        path.pop_back();
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

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file";
        return;
    }

    char ch;
    while (file.get(ch)) {
        std::cout << ch;
    }
}

void InitializeTrainModule() {
    std::string textFilePath;
    std::cout << "Enter the path to your training data (.txt filetype only) >>";
    std::getline(std::cin, textFilePath);
    
    ScrubPathInput(textFilePath);
    textFilePath = NormalizePath(textFilePath);
    
    ParseFileByChar(textFilePath);
}

// Create JSON object
// json tree;
// tree["a"]["char"] = "a";
// tree["a"]["next_chars"]["b"] = 3;
// tree["a"]["next_chars"]["c"] = 1;
// tree["a"]["children"]["b"]["char"] = "b";
// tree["a"]["children"]["b"]["next_chars"]["x"] = 2;

// // Write JSON to file
// std::ofstream outfile("../data/character_modal.json");
// outfile << tree.dump(4); // Pretty print with indent = 4
// outfile.close();

// std::cout << "JSON written to ../data/character_modal.json";

// Read from JSON file
// std::ifstream inFile("../data/character_modal.json");
// json loadedTree;
// inFile >> loadedTree;
// inFile.close();

// std::cout << "Loaded char at a: " << loadedTree["a"]["char"] << "\n";
// std::cout << "Next char count for 'b': " << loadedTree["a"]["next_chars"]["b"] << "\n";


// std::cout << "\n\nPress Enter to exit...\n";
// std::cin.get();
// return 0;

//#include <iostream>
//#include <fstream>
//
//void parseFileCharByChar(const std::string& filepath) {
//    std::ifstream file(filepath);
//    if (!file.is_open()) {
//        std::cerr << "Failed to open file: " << filepath << "\n";
//        return;
//    }
//
//    char ch;
//    while (file.get(ch)) {  // Reads one character at a time
//        std::cout << "Char: " << ch << "\n";
//        // Example: Count words, parse tokens, etc.
//    }
//
//    file.close();
//}
//
//int main() {
//    parseFileCharByChar("example.txt");
//    return 0;
//}