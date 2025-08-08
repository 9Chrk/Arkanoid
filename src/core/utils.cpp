using namespace std;
#include "core/utils.hpp"


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

json loadSettings() {
  static const string filepath = "./assets/data/settings.json";
  return openJsonFile(filepath);
}

// System commands

vector<string> listLevels(const filesystem::path& dir) {
  vector<string> out;
  for (auto& e : filesystem::directory_iterator(dir))
    if (e.path().filename().string().starts_with("level_") && e.path().extension()==".json")
      out.push_back(e.path().filename().string());
  sort(out.begin(), out.end());
  return out;
}
