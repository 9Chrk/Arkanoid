# Compiler and option definitions
CXX = g++
CXXFLAGS += -std=c++20
CXXFLAGS += -pedantic -Wall -Wextra
CXXFLAGS += -O2
CXXFLAGS += -Ilibs
CXXFLAGS += -Iinclude
CXXFLAGS += -Iinclude/core
CXXFLAGS += -Iinclude/model      # Include JSON library
CXXFLAGS += -Iinclude/view          
CXXFLAGS += -Iinclude/controller    
CXXFLAGS += -Iinclude/engine         
CXXFLAGS += -Walloc-zero -Wcast-align -Wconversion -Wctad-maybe-unsupported \
            -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wduplicated-branches \
            -Wduplicated-cond -Weffc++ -Wextra-semi -Wfloat-equal \
            -Wformat-signedness -Wformat=2 -Winit-self -Wlogical-op \
            -Wmismatched-tags -Wnoexcept -Wnon-virtual-dtor -Wnull-dereference \
            -Wold-style-cast -Woverloaded-virtual -Wredundant-tags \
            -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
            -Wsuggest-attribute=cold -Wsuggest-attribute=const \
            -Wsuggest-attribute=noreturn -Wsuggest-override \
            -Wswitch-default -Wswitch-enum -Wundef -Wuseless-cast -Wvolatile \
            -Wzero-as-null-pointer-constant

obj/src/model/GameModel.o: CXXFLAGS += -Wno-suggest-attribute=const # false positive

# Allegro dependency handling
CXXFLAGS += $(shell pkg-config --cflags allegro-5 allegro_primitives-5 allegro_font-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
LDLIBS += $(shell pkg-config --libs allegro-5 allegro_primitives-5 allegro_font-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)

# Directories
SRC_DIR = src
OBJ_DIR = obj
MAIN = src/main.cpp

# Retrieve .cpp files recursively
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')

# Objects with subdirectory preservation
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default rule
all: clean $(OBJ_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o Arkanoid $(LDLIBS)

# Compile rule: create folder if needed
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create obj/ directory if missing
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean
clean:
	rm -rf $(OBJ_DIR) Arkanoid
