/*
  Copyright Michael Quested 2014.

  This file is part of Cub3r.

  Cub3r is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Cub3r is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Cub3r.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * Setup SDL2 window, handle events and 3D camera.
 *
 * @author mdq3
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include "Scene.hpp"

/**
 *
 */
class Window {
 public:
    /**
     * Constructor for Window. Create new SDL2 window with OpenGL context.
     *
     * @param width The initial width of the window
     * @param height The initial height of the window
     */
    Window(int width, int height);

    /**
     * Destructor for Window. Release resources.
     */
    ~Window();

    void renderScene();

    void initWindow();

    void initGL();

    glm::vec3 getCameraPosition();

    /**
     * Close the window.
     */
    void close();

    /**
     * Return the run state of the program.
     *
     * @return true if program is running, false if not
     */
    bool isRunning();

    /**
     * Handle window events.
     */
    void handleEvents();

 private:
    SDL_Window* window;
    SDL_GLContext gContext;

    int width;
    int height;
    bool running;
    bool inFocus;
    GLfloat mouseSensitivity;

    Scene scene;

    void handleKeyPressed(SDL_Keycode value);

    void handleKeyReleased(SDL_Keycode value);
};

#endif // WINDOW_H_