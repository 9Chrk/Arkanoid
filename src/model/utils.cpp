#include "model/utils.hpp"

// Resource initialization
void must_init(bool test, const std::string& description) {
  if (!test) throw std::runtime_error("Failed to initialize: " + description);
}

void must_init(void* test, const std::string& description) {
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

// System commands

std::vector<std::string> executeCommand(const std::string& command) { // donnée par chatGPT
  std::vector<std::string> output;
  std::unique_ptr<FILE, int(*)(FILE*)> pipe(popen(command.c_str(), "r"), pclose);
  
  if (!pipe) {
    throw std::runtime_error("Error during command execution.");
  }
  char buffer[128];
  while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
    std::string line(buffer);
    if (!line.empty() && line.back() == '\n') {
      line.pop_back();
    }
    output.push_back(line);
  }
  return output;
}
