
CC = g++
CFLAGS = -std=c++11 -Wall
SOURCES = src/*.cpp
LDFLAGS = -lSDL2 -lSDL2_image -lGL -lGLEW -ltinyxml2
EXECUTABLE = bin/cub3r

all:
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)
