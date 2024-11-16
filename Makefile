CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

SOURCES = main.cpp \
          source_code/offline_engine.cpp \
		  source_code/player.cpp \
		  source_code/creature.cpp \
		  source_code/mobile_object.cpp \
		  source_code/object_map.cpp \
		  source_code/object.cpp \
		  source_code/model_collection.cpp \
		  source_code/model_cycle.cpp \
		  source_code/model.cpp \
		  source_code/hitbox_container.cpp \
		  source_code/static_hitbox.cpp \
		  source_code/mobile_hitbox.cpp \
		  source_code/hitbox.cpp \
		  source_code/utility_functions.cpp \
		  source_code/structs_and_enums.cpp \
		  source_code/constants.h


TARGET = compiled/main

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

clean:
	rm -rf compiled/*
