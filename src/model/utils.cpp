using namespace std;
#include "model/utils.hpp"


// Resource initialization
void must_init(bool test, const string& description) {
  if (!test) throw runtime_error("Failed to initialize: " + description);
}

void must_init(const void* test, const string& description) {
  must_init(test != nullptr, description);
}

// JSON functions

json openJsonFile(const string& fileName) {
  ifstream file(fileName);
  if (!file.is_open()) {
    throw runtime_error("Cannot open file: " + fileName);
  }
  json data;
  file >> data;
  file.close();
  return data;
}

json readJsonFile(const string& fileName, const string& key) {
  json data = openJsonFile(fileName);
  if (!data.contains(key)) {
    throw invalid_argument("Key not found :" + key);
  }
  return data[key];
}

void writeJsonFile(const string& fileName, const string& key, const json& value) {
  json data = openJsonFile(fileName);
  data[key] = value;
  writeJsonFile(fileName, data);
}

void writeJsonFile(const string& fileName, const json& data) {
  ofstream newFile(fileName);
  if (!newFile.is_open()) {
    throw runtime_error("Cannot write to :" + fileName);
  }
  newFile << data.dump(2);
  newFile.close();
}

// System commands

vector<string> executeCommand(const string& command) { // donnée par chatGPT
  vector<string> output;
  unique_ptr<FILE, int(*)(FILE*)> pipe(popen(command.c_str(), "r"), pclose);
  
  if (!pipe) {
    throw runtime_error("Error during command execution.");
  }
  char buffer[128];
  while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
    string line(buffer);
    if (!line.empty() && line.back() == '\n') {
      line.pop_back();
    }
    output.push_back(line);
  }
  return output;
}
