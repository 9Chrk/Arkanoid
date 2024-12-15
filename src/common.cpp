#include "common.hpp"

[[gnu::pure]]
ALLEGRO_COLOR getColor(string colorName) {
  if (colorName == "WHITE") return WHITE;
  else if (colorName == "GREY") return GREY;
  else if (colorName == "BLACK") return BLACK;
  else if (colorName == "ORANGE") return ORANGE;
  else if (colorName == "CYAN") return CYAN;
  else if (colorName == "GREEN") return GREEN;
  else if (colorName == "RED") return RED;
  else if (colorName == "BLUE") return BLUE;
  else if (colorName == "MAGENTA") return MAGENTA;
  else if (colorName == "YELLOW") return YELLOW;
  else if (colorName == "SILVER") return SILVER;
  else if (colorName == "GOLD") return GOLD;
  else { return BLACK; }
}

json openJsonFile(string fileName) {
  ifstream file(fileName);
  if (file.is_open()) {
    json data;
    file >> data;
    file.close();
    return data;
  } else {
    throw runtime_error("Impossible d'ouvrir le fichier : " + fileName);
  }
}

json readJsonFile(string fileName, string key) {
  json data = openJsonFile(fileName);
  if (data.contains(key)) {
    return data[key];
  } else {
    throw invalid_argument("Clé non trouvée : " + key);
  }
}

void writeJsonFile(string fileName, string key, json value) {
  json data = openJsonFile(fileName);
  data[key] = value;
  
  ofstream newFile(fileName);
  if (newFile.is_open()) {
    newFile << data.dump(2);
    newFile.close();
  } else {
    throw runtime_error("Impossible d'écrire dans le fichier : " + fileName);
  }
}