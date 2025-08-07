#pragma once
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>
#include <utility>
#include <cstdio>
#include <memory>
#include <limits>
#include <algorithm>
#include <unordered_map>

#include "json.hpp"
using json = nlohmann::json;

inline constexpr float GameWidth     = 550.f;
inline constexpr float GameHeight    = 500.f;
inline constexpr float PlayableRatio = 1.0f/3.0f;


// JSON functions
json  openJsonFile(const std::string& fileName);
json  readJsonFile(const std::string& fileName, const std::string& key);
void writeJsonFile(const std::string& fileName, const std::string& key, const json& value);
void writeJsonFile(const std::string& fileName, const json& data);

// System commands
std::vector<std::string> executeCommand(const std::string& command);
