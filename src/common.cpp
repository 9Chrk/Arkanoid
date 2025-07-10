#include "common.hpp"

[[gnu::pure]]
ALLEGRO_COLOR getColor(const string& colorName) {
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

json openJsonFile(const string& fileName) {
  ifstream file(fileName);
  if (!file.is_open()) {
    throw runtime_error("Impossible d'ouvrir le fichier : " + fileName);
  }
  json data;
  file >> data;
  file.close();
  return data;
}

json readJsonFile(const string& fileName, const string& key) {
  json data = openJsonFile(fileName);
  if (!data.contains(key)) {
    throw invalid_argument("Clé non trouvée : " + key);
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
    throw runtime_error("Impossible d'écrire dans le fichier : " + fileName);
  }
  newFile << data.dump(2);
  newFile.close();
}

int checkBitmap(ALLEGRO_BITMAP* bitmap, const string& fileName) {
  if (!bitmap) {
    cerr << "Erreur lors du chargement de l'image : " << fileName << endl;
    return -1;
  }
  return 0;
}

int checkSample(ALLEGRO_SAMPLE* sample, const string& fileName) {
  if (!sample) {
    cerr << "Erreur lors du chargement du son : " << fileName << endl;
    return -1;
  }
  return 0;
}

vector<string> executeCommand(const string& command) { // donnée par chatGPT
  vector<string> output;
  unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

  if (!pipe) {
    throw runtime_error("Erreur lors de l'exécution de la commande.");
  }
  char buffer[128];
  while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
    string line(buffer);
    if (!line.empty() && line.back() == '\n') {
      line.pop_back();
    }
    output.push_back(line);  }
  return output;
}

ALLEGRO_BITMAP* loadBitmap(const char* filename) {
  ALLEGRO_BITMAP* bmp = al_load_bitmap(filename);
  checkBitmap(bmp, filename);
  if (bmp) {
    al_convert_bitmap(bmp);
  }
  return bmp;
}

ALLEGRO_SAMPLE* loadSample(const char* filename) {
  ALLEGRO_SAMPLE* sample = al_load_sample(filename);
  checkSample(sample, filename);
  return sample;
}
