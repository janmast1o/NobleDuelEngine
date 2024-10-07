CXX = g++
FLAGS = -Wall -g -o
STANDARD = -std=c++17
SDL_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

all_targets = compiled/main


all: $(all_targets)


compiled/main: main.cpp
	$(CXX) $(STANDARD) $(FLAGS) compiled/main main.cpp $(SDL_FLAGS)


clean:
	rm -r --force $(all_targets)	