### FLAGS ###
CXXFLAGS += -std=c++2b
CXXFLAGS += -pedantic -Wall -Wextra
CXXFLAGS += -O2 # enable some optimisation
CXXFLAGS += -Walloc-zero -Wcast-align -Wconversion -Wctad-maybe-unsupported -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wduplicated-branches -Wduplicated-cond -Weffc++ -Wextra-semi -Wfloat-equal -Wformat-signedness -Wformat=2 -Winit-self -Wlogical-op -Wmismatched-tags -Wnoexcept -Wnon-virtual-dtor -Wnull-dereference -Wold-style-cast -Woverloaded-virtual -Wredundant-tags -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wsuggest-attribute=cold -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=pure -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wuseless-cast -Wvolatile -Wzero-as-null-pointer-constant
CXXFLAGS += -Wno-unused-command-line-argument -Wno-unknown-warning-option

### ALLEGRO DEP ###
CXXFLAGS += $(shell pkg-config allegro-5 allegro_primitives-5 allegro_font-5 --cflags)
LDLIBS += $(shell pkg-config allegro-5 allegro_primitives-5 allegro_font-5 --libs)

##############

main: main.cpp
	${CXX} ${CXXFLAGS} $< -o $@ ${LDLIBS}