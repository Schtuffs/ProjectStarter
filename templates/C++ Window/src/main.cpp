#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Colour.h"
#include "Window.h"
#include "Callbacks.h"

int main(void) {
    // Create and check main window
    Window window("Example");
    if (!window.isCreated()) {
        std::cout << "The window failed to be created.\n";
        return 0;
    }

    // Window setup
    window.setBackground(Colour(0, 0, 0));
    window.setRefreshRate(120);

    // Set a simple callback
    window.setCallback(key_callback_main);

    // Main window loop
    Colour col(0, 0, 0);
    while (!window.shouldClose()) {
        Window::poll();
        window.render();
    }

    return 0;
}

