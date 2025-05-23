#ifndef TRAIN_H
#define TRAIN_H

#include <nlohmann/json.hpp>
#include <string>


void InitializeTrainModule();

void ProcessSubstring(const std::string& str, nlohmann::json& tree);

#endif