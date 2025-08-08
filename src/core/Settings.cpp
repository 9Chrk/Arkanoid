#include "core/Settings.hpp"

nlohmann::json loadSettings() {
  static const std::string filepath = "./assets/data/settings.json";
  return openJsonFile(filepath);
}

