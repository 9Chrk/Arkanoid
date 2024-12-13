# Définition du compilateur et des options
CXX = g++
CXXFLAGS += -std=c++20
CXXFLAGS += -pedantic -Wall -Wextra
CXXFLAGS += -O2
CXXFLAGS += -Iinclude # Inclure les fichiers d'en-tête
CXXFLAGS += -Walloc-zero -Wcast-align -Wconversion -Wctad-maybe-unsupported \
            -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wduplicated-branches \
            -Wduplicated-cond -Weffc++ -Wextra-semi -Wfloat-equal \
            -Wformat-signedness -Wformat=2 -Winit-self -Wlogical-op \
            -Wmismatched-tags -Wnoexcept -Wnon-virtual-dtor -Wnull-dereference \
            -Wold-style-cast -Woverloaded-virtual -Wredundant-tags -Wshadow \
            -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
            -Wsuggest-attribute=cold -Wsuggest-attribute=const \
            -Wsuggest-attribute=noreturn -Wsuggest-attribute=pure \
            -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override \
            -Wswitch-default -Wswitch-enum -Wundef -Wuseless-cast -Wvolatile \
            -Wzero-as-null-pointer-constant

# Gestion des dépendances Allegro
CXXFLAGS += $(shell pkg-config allegro-5 allegro_primitives-5 allegro_font-5 --cflags)
LDLIBS += $(shell pkg-config allegro-5 allegro_primitives-5 allegro_font-5 --libs)

# Dossiers
SRC_DIR = ./src
INCLUDE_DIR = ./include
OBJ_DIR = ./obj
MAIN = main.cpp

# Récupération des fichiers source et objets
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(MAIN)
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

# Règle par défaut
all: $(OBJ_DIR) $(OBJS)
	$(CXX) $(OBJS) -o main $(LDLIBS)

# Compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Création du dossier obj si nécessaire
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Nettoyage
clean:
	rm -rf $(OBJ_DIR)/*.o main