#include "Colour.h"
#include "Window.h"
#include "Callbacks.h"

int main(void) {
    // Create and check main window
    Window window("Example", 800, 800);
    if (!window.isCreated()) {
        return 0;
    }
    
    window.setBackground(Colour(50))
        .setSize(250, 250, 1000, 1000)
        .setCallback(key_callback_main)
        .setCallback(window_callback)
        .run();

    Window::await();

    return 0;
}

