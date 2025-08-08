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
#include "Rectangle.hpp"
#include "constants.hpp"

using json = nlohmann::json;


// Resource initialization
void must_init(bool test, const std::string& description);
void must_init(const void* test, const std::string& description);

// JSON functions
json  openJsonFile(const std::string& fileName);
json  readJsonFile(const std::string& fileName, const std::string& key);
void writeJsonFile(const std::string& fileName, const std::string& key, const json& value);
void writeJsonFile(const std::string& fileName, const json& data);
json loadSettings();

// System commands
std::vector<std::string> listLevels(const std::filesystem::path& dir);
