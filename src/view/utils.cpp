using namespace std;
#include "view/utils.hpp"


// Allegro error handling 

void checkBitmap(const ALLEGRO_BITMAP* bitmap, const string& fileName) {
  if (!bitmap) throw runtime_error("Failed to load image: " + fileName);
}

void checkSample(const ALLEGRO_SAMPLE* sample, const string& fileName) {
  if (!sample) throw runtime_error("Failed to load sound: " + fileName);
}

// Allegro resource loading

ALLEGRO_BITMAP* loadBitmap(const string& filename) {
  ALLEGRO_BITMAP* bmp = al_load_bitmap(filename.c_str());
  checkBitmap(bmp, filename);
  al_convert_bitmap(bmp);
  return bmp;
}

ALLEGRO_SAMPLE* loadSample(const string& filename) {
  ALLEGRO_SAMPLE* sample = al_load_sample(filename.c_str());
  checkSample(sample, filename);
  return sample;
}

// Color utility function

[[gnu::pure]] ALLEGRO_COLOR getColor(const string& colorName) {
  if      (colorName == "WHITE")   return WHITE;
  else if (colorName == "GREY")    return GREY;
  else if (colorName == "BLACK")   return BLACK;
  else if (colorName == "ORANGE")  return ORANGE;
  else if (colorName == "CYAN")    return CYAN;
  else if (colorName == "GREEN")   return GREEN;
  else if (colorName == "RED")     return RED;
  else if (colorName == "BLUE")    return BLUE;
  else if (colorName == "MAGENTA") return MAGENTA;
  else if (colorName == "YELLOW")  return YELLOW;
  else if (colorName == "SILVER")  return SILVER;
  else if (colorName == "GOLD")    return GOLD;
  else { return BLACK; }
}
