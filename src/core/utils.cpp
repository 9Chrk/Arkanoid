#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include "core/utils.hpp"


// Resource initialization

void must_init(bool test, const std::string& description) {
  if (!test) throw std::runtime_error("Failed to initialize: " + description);
}

void must_init(const void* test, const std::string& description) {
  must_init(test != nullptr, description);
}

// JSON functions

json openJsonFile(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + fileName);
  }
  json data;
  file >> data;
  file.close();
  return data;
}

json readJsonFile(const std::string& fileName, const std::string& key) {
  json data = openJsonFile(fileName);
  if (!data.contains(key)) {
    throw std::invalid_argument("Key not found :" + key);
  }
  return data[key];
}

void writeJsonFile(const std::string& fileName, const std::string& key, const json& value) {
  json data = openJsonFile(fileName);
  data[key] = value;
  writeJsonFile(fileName, data);
}

void writeJsonFile(const std::string& fileName, const json& data) {
  std::ofstream newFile(fileName);
  if (!newFile.is_open()) {
    throw std::runtime_error("Cannot write to :" + fileName);
  }
  newFile << data.dump(2);
  newFile.close();
}

json loadSettings() {
  static const std::string filepath = "./assets/data/settings.json";
  return openJsonFile(filepath);
}

// System commands

std::vector<std::string> listLevels(const std::filesystem::path& dir) {
  std::vector<std::string> out;
  for (auto& e : std::filesystem::directory_iterator(dir))
    if (e.path().filename().string().starts_with("level_") && e.path().extension()==".json")
      out.push_back(e.path().filename().string());
  std::sort(out.begin(), out.end());
  return out;
}
