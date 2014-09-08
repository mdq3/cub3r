
CC = g++
CFLAGS = -std=c++11 -Wall
SOURCES = src/*.cpp
LDFLAGS = -lGL -lGLEW -lsfml-system -lsfml-window -lsfml-graphics -ltinyxml2
EXECUTABLE = bin/cub3r

all:
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)