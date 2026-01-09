#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window/Colour.h"
#include "window/Renderable.h"

/*
Allows for easy creation of windows.

Currently not optimized to handle multiple windows being
made since all are refreshed on the same thread.

Having only 1 window skips context swaps.

Hopefully will put rendering onto other threads
in the future to allow for more windows simultaneously
without losing performance.
*/
class Window {
private:
    GLFWwindow* m_window;
    bool m_created;

    std::vector<Renderable*> m_renderObjects;
    double m_prevTime, m_frameTime;

    // Initializes glfw
    void init();
    // Closes OpenGL
    void terminate();

    // For rendering frames at defined frame rate
    bool hasFrameTimePassed();

public:

    // ----- Creation ----- Destruction -----

    Window(const std::string& title = "Window");
    bool isCreated();
    ~Window();


    
    // ----- Setters -----

    // Set background to specified colour
    Window& setBackground(const Colour& col);

    // Sets window to desired refreshrate
    // Setting to 0 removes refresh cap
    Window& setRefreshRate(int newRate);

    // Chooses callback overload based on passed param

    // Sets the key callback for the window
    Window& setCallback(GLFWkeyfun callback);
    // Sets the cursor position callback for the window
    Window& setCallback(GLFWcursorposfun callback);
    // Sets the mouse button callback for the window
    Window& setCallback(GLFWmousebuttonfun callback);
    // Sets the character pressed callback for the window
    Window& setCallback(GLFWcharfun callback);
    // Sets the window resize callback for the window
    Window& setCallback(GLFWframebuffersizefun callback);
    // Sets the window refresh callback for the window
    Window& setCallback(GLFWwindowrefreshfun callback);

    // Add a new drawable into the screen
    Window& add(Renderable& obj);

    // ----- OpenGL ----- Functions -----

    // Main function to check while looping
    bool shouldClose();

    // Poll for actions on window
    static void poll();

    // Render window
    void render();
};

