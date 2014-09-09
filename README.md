Cub<sup>3</sup>r
=====

3D cube puzzle game/solver using OpenGL


Compilation
===========

Use make or manually: 

g++ -std=c++11 -Wall Main.cpp Model.cpp Camera.cpp Importer.cpp Cube.cpp Window.cpp -lGL -lGLEW -lsfml-system -lsfml-window -lsfml-graphics -ltinyxml2 -o ../bin/cub3r


Dependencies
============
* OpenGL 
* GL Extension Wrangler (glew)
* OpenGL Mathematics (GLM)
* SFML
* SFML window
* SFML graphics
* TinyXML-2


Project Directories
===================
    /src          - all source code for project
    /include      - all header files for project
    /bin          - compiled binaries
    /resources    - extra resources required by the program
        /models   - models used in program
        /scripts  - scripts used to generate resources
        /shaders  - GLSL shaders
        /textures - Textures used in UV mapping


<div align="center"><a href="https://github.com/mdq3/cub3r"><img src="https://i.imgur.com/mKpFvN2.png?1" alt="Cub3r" /></a></div>
