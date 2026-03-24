#include "Callbacks.h"

#include <print>

#include "Window.h"

void key_callback_alt([[maybe_unused]]GLFWwindow* window, [[maybe_unused]]int key, [[maybe_unused]]int scancode, [[maybe_unused]]int action, [[maybe_unused]]int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void key_callback_main([[maybe_unused]]GLFWwindow* window, [[maybe_unused]]int key, [[maybe_unused]]int scancode, [[maybe_unused]]int action, [[maybe_unused]]int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        Window* win = new Window("Callback");
        if (!win->isCreated()) {
            return;
        }
        win->setBackground(Colour(rand()%255,rand()%255,rand()%255));
        win->setCallback(key_callback_alt);
        win->run();
    }
}

void window_callback(GLFWwindow* window, int width, int height) {
    try {
        Window& w = Window::find(window);
        w.setSize(width, height);
    } catch (std::string& err) {
        std::println(stderr, "{}", err);
    }
}
