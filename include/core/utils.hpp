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
#include <optional>

#include "json.hpp"
#include "Rectangle.hpp"
#include "constants.hpp"

using json = nlohmann::json;


/* ############## Resource initialization ############## */

/// @brief Throw an error if initialization fails.
/// @param test Result of the initialization.
/// @param description Resource name.
/// @throws std::runtime_error if test is false.
void must_init(bool test, const std::string& description);

/// @brief Pointer variant of must_init.
/// @param test Pointer to check.
/// @param description Resource name.
/// @throws std::runtime_error if test is null.
void must_init(const void* test, const std::string& description);


/* ############## JSON functions ############## */

/// @brief Load a JSON file.
/// @param fileName Path to the file.
/// @return JSON data from the file.
/// @throws std::runtime_error if the file is inaccessible.
json  openJsonFile(const std::string& fileName);

/// @brief Read a key from a JSON file.
/// @param fileName Path to the file.
/// @param key Key to search for.
/// @return Value associated with the key.
/// @throws std::invalid_argument if the key is missing.
json  readJsonFile(const std::string& fileName, const std::string& key);

/// @brief Write a key to an existing JSON file.
/// @param fileName Path to the file.
/// @param key Key to modify.
/// @param value Value to save.
/// @throws std::runtime_error if writing fails.
void writeJsonFile(const std::string& fileName, const std::string& key, const json& value);

/// @brief Write full JSON data to a file.
/// @param fileName Path to the file.
/// @param data Complete data to save.
/// @throws std::runtime_error if writing fails.
void writeJsonFile(const std::string& fileName, const json& data);

/// @brief Load game configuration.
/// @return Configuration data.
json loadSettings();


/* ############## System commands ############## */

/// @brief List available level files.
/// @param dir Directory to explore.
/// @return Sorted list of file names.
std::vector<std::string> listLevels(const std::filesystem::path& dir);
