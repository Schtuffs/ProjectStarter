#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window/Window.h"
#include "window/Callbacks.h"

int main() {
    Window window("Window");
    window.setBackground(Colour(50)).setCallback(key_callback_main).render();
    while (!window.shouldClose()) {
        Window::poll();
        window.render();
    }
    return 0;
}