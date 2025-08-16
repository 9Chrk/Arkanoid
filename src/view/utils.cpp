using namespace std;
#include <unordered_map>
#include "view/utils.hpp"


static const unordered_map<string, ALLEGRO_COLOR> COLORS{
        {"WHITE", WHITE},   {"GREY", GREY},     {"BLACK", BLACK},
        {"ORANGE", ORANGE}, {"CYAN", CYAN},     {"GREEN", GREEN},
        {"RED", RED},       {"BLUE", BLUE},     {"MAGENTA", MAGENTA},
        {"YELLOW", YELLOW}, {"SILVER", SILVER}, {"GOLD", GOLD}};

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
    if (const auto it = COLORS.find(colorName); it != COLORS.end()) {
        return it->second;
    }
    return BLACK;
}