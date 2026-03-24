#pragma once

#include <cstdint>
#include <functional>
#include <mutex>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Colour.h"
#include "Renderable.h"

/**
 * @class Window
 * @brief Main class for creating windows
 * @author Kyle Wagler
 * @date 2026-03-19
 */
class Window {
public:
    static constexpr uint64_t DEFAULT_FRAMERATE     = 60;
    static constexpr uint64_t DEFAULT_WINDOW_WIDTH  = 800;
    static constexpr uint64_t DEFAULT_WINDOW_HEIGHT = 800;

    // ----- Creation ----- Destruction -----

    /**
     * @brief Creates a simple window with default parameters.
     * A simple window that allows for modification through helper functions. Has some functionality for editing certain window properties.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window(const std::string& title = "Window", int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT);
    /**
     * @brief Allows for checking if the window was properly created.
     * Allows use only if the window was properly created and initialized. Calling set functions without checking this is undefined behaviour.
     * @return `true` on working window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    bool isCreated();
    ~Window();


    
    // ----- STATIC -----
    
    /**
     * @brief Allows for checking if main window is closed.
     * This function waits for the main window to close before returning control to developer.
     * @param lambda A function to run while waiting for all windows to close.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    static void await(const std::function<void(void)>& lambda = [](){});
    
    /**
     * @brief Find a window.
     * This function attempts to find a window. Useful for callbacks to find and update a window.
     * @param window The window to find.
     * @throws `std::string` on failure to find window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    static Window& find(const GLFWwindow* window);


    
    // ----- Setters -----
    
    /**
     * @brief Sets this window as the main window. First successfully created window is automatically the main.
     * On main window close, all other windows are also closed. This allows for sub windows to be properly closed when the initial is closed.
     * The user can continue program after Window::await.
     * @return The updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setMain();
    
    /**
     * @brief Changes the background colour of the window.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setBackground(Colour col);
    
    /**
     * @brief Updates window size. Recommended to only call from callback.
     * @param width The new width of the window.
     * @param height The new height of the window.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setSize(int width, int height);
    
    /**
     * @brief Changes the size parameters of the window.
     * @param minWidth The minimum allowed width for the window. Set to`GLFW_DONT_CARE`for any value to be allowed.
     * @param minHeight The minimum allowed height for the window. Set to`GLFW_DONT_CARE`for any value to be allowed.
     * @param maxWidth The maximum allowed width for the window. Set to`GLFW_DONT_CARE`for any value to be allowed.
     * @param maxHeight The maximum allowed height for the window. Set to`GLFW_DONT_CARE`for any value to be allowed.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setSize(int minWidth, int minHeight, int maxWidth, int maxHeight);
    
    /**
     * @brief Get the size of the window.
     * @param width [out] The width of the window.
     * @param height [out] The height of the window.
     * @return The window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& getSize(int& width, int& height);
    
    /**
     * @brief Changes the title of the window.
     * @param title The new title for the window.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setTitle(const std::string& title);
    
    // Chooses callback overload based on passed param
    
    /**
     * @brief Sets a callback for keypressed.
     * @param callback The callback to set with format `callback(GLFWwindow* window, int key, int scancode, int action, int mods)`.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWkeyfun callback);
    /**
     * @brief Sets a callback for cursor position.
     * @param callback The callback to set with format `callback(GLFWwindow* window, double xpos, double ypos)`.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWcursorposfun callback);
    /**
     * @brief Sets a callback for mouse button press.
     * @param callback The callback to set with format `callback(GLFWwindow* window, int button, int action, int mods)`.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWmousebuttonfun callback);
    /**
     * @brief Sets a callback for chars (supports unicode).
     * @param callback The callback to set with format `callback(GLFWwindow* window, unsigned int codepoint)`.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWcharfun callback);
    /**
     * @brief Sets a callback for window size changes.
     * @param callback The callback to set with format `callback(GLFWwindow* window, int width, int height)`.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWframebuffersizefun callback);
    /**
     * @brief Sets a callback for window redraw (such as window covered).
     * @param callback The callback to set with format `callback(GLFWwindow* window)`.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWwindowrefreshfun callback);
    
    /**
     * @brief Checks if the window should be closed or not.
     * @return `true` when window should be closed.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    bool shouldClose();
    
    /**
     * @brief Adds a new renderable object to window.
     * @param obj The new `Renderable` object.
     * @return The updated window.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& add(Renderable& obj);
    /**
     * @brief Removes a `Renderable` object from the window.
     * @param index The index of the `Renderable` object to remove from window.
     * @return `true` on successful removal.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    bool remove(uint64_t index);
    /**
     * @brief Removes a `Renderable` object from the window.
     * @param obj The `Renderable` object to remove from window.
     * @return `true` on successful removal.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    bool remove(Renderable& obj);
    
    /**
     * @brief Runs window with current args.
     * @param lambda A lambda function to run on every update per second.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    void run(const std::function<void(void)>& lambda);
    /**
     * @brief Runs window with current args.
     * @param upsMatchFps If the Updated per Second should match the Frames per Second.
     * @param lambda A lambda function to run on every update per second.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    void run(bool upsMatchFps, const std::function<void(void)>& lambda = [](){});
    /**
     * @brief Runs window with current args.
     * @param frames Frames per Second, 0 for no limit.
     * @param upsMatchFps If the Updated per Second should match the Frames per Second.
     * @param lambda A lambda function to run on every update per second.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    void run(uint64_t frames = DEFAULT_FRAMERATE, bool upsMatchFps = true, const std::function<void(void)>& lambda = [](){});
    
    
    
    // ----- Getters -----
    
    /**
     * @brief Returns the list of all the renderable items.
     * @return `vector` of all renderables.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    std::vector<Renderable> renderables();

private:
    // Holds the window data
    GLFWwindow* m_window;
    bool m_created, m_closed;
    int m_width, m_height;
    std::mutex m_resizedMutex;
    bool m_resized;

    // Rendering objects
    std::vector<Renderable> m_renderObjects;

    // Time since previous render
    uint64_t m_prevTime, m_frameTime;

    // For rendering frames at defined frame rate
    bool hasFrameTimePassed(bool upsMatchFps);
    
    // Required OpenGL functions for a window to function

    // Main window loop
    void loop(bool upsMatchFps, const std::function<void(void)>& lambda);

    // Checks for window updates
    void checkUpdates();

    // Render window
    void render(bool upsMatchFps);

    // On close
    void close();
};

