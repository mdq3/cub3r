# Cub<sup>3</sup>r #


3D cube puzzle game/solver using OpenGL  
  
  
## Usage ##

Keyboard:  
**Esc** - quit program  
**I** - Rotate top face  
**M** - Rotate bottom face  
**J** - Rotate left face  
**L** - Rotateright face  
**K** - Rotate front face  
**O** - Rotate back face  
**(hold) LShift** - Rotate anticlockwise  


## Compilation ##

Use make or manually: 

    g++ -std=c++11 -Wall Main.cpp Model.cpp Camera.cpp Importer.cpp Cube.cpp Window.cpp -lSDL2 -lSDL2_image -lGL -lGLEW -ltinyxml2 -o ../bin/cub3r


## Dependencies ##

* OpenGL (at least 3.3)
* GL Extension Wrangler (glew)
* OpenGL Mathematics (GLM)
* SDL2
* SDL2 image
* TinyXML-2


## Project Directories ##

    /src          - all source code for project
    /include      - all header files for project
    /bin          - compiled binaries
    /resources    - extra resources required by the program
        /models   - models used in program
        /scripts  - scripts used to generate resources
        /shaders  - GLSL shaders
        /textures - Textures used in UV mapping


<div align="center"><a href="https://github.com/mdq3/cub3r"><img src="https://i.imgur.com/psNMmPN.png?1" alt="Cub3r" /></a></div>
