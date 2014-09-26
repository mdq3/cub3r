/**
 * Initializes program state and enters main loop.
 *
 * @author mdq3
 */

#ifndef MAIN_H_
#define MAIN_H_

/**
 *
 */
class Main {
 public:

    /**
     * Constructor for a new Game. Create the SFML window, initialize
     * OpenGL, then invoke main program loop.
     *
     * @param windowWidth The initial width of the window
     * @param windowHeight The initial height of the window
     */
    Main(int windowWidth, int windowHeight);

    /**
     * Destructor for Game. Release resources.
     */
    ~Main();

 private:

    Window window;

    /**
     * Create SFML window and initialize OpenGL.
     */
    void init();

    /**
     * Enter main program loop. Handles events and calls render() when
     * necessary.
     */
    void mainLoop();
};

#endif // MAIN_H_
