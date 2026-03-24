#include "Window.h"

#include <chrono>
#include <print>
#include <thread>

#include "WindowManager.h"

#define GET_CURRENT_TIME() std::chrono::nanoseconds(std::chrono::system_clock::now().time_since_epoch()).count()

using namespace std::chrono_literals;

// Holds values for opening/closing OpenGL
static bool s_forceClose = false;
// Holds a specified main window
static GLFWwindow* s_mainWindow = nullptr;
// Ensure the await is on main thread
static std::thread::id s_mainThread = std::this_thread::get_id();
// Holds pointer to all windows
static std::vector<Window*> s_windows;

// ----- Creation ----- Destruction -----

Window::Window(const std::string& title, int width, int height) {
    // Add window to count
    WindowManager::add();
    m_width = width;
    m_height = height;

    // Create the window
    this->m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, NULL);
    if (this->m_window == nullptr) {
        std::println("Could not create window...");
        this->m_created = false;

        // Remove failed window
        WindowManager::remove();
        return;
    }

    this->m_created = true;
    glfwMakeContextCurrent(this->m_window);
    if (s_mainWindow == nullptr) {
        setMain();
    }
    
    // Glad initialization
    gladLoadGL();
    glViewport(0, 0, m_width, m_height);

    // Setting blendmode so .png have blank backgrounds
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable vsync
    glfwSwapInterval(0);

    // Other window attributes
    glfwSetWindowSizeLimits(m_window, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);
    
    // Other variables
    this->m_frameTime = 0;
    this->m_closed = false;
}

bool Window::isCreated() {
    return this->m_created;
}

Window::~Window() {
    this->close();
}

void Window::close() {
    if (this->m_window != nullptr) {
        glfwDestroyWindow(this->m_window);
        this->m_window = nullptr;
        WindowManager::remove();
    }
}



// ----- Static -----

void Window::await(const std::function<void(void)>& lambda) {
    // Force main thread only
    if (std::this_thread::get_id() != s_mainThread) {
        std::println(stderr, "ERROR: Await must be called on the main thread");
        return;
    }
    
    // Wait until windows are closed
    while (WindowManager::hasWindows()) {
        // Poll events then call the users lambda function
        glfwPollEvents();
        lambda();
        
        // This thread must close the windows
        for (size_t i = 0; i < s_windows.size(); i++) {
            if (s_windows[i]->m_closed) {
                // Close window, erase from list, and adjust the index for removed item
                s_windows[i]->close();
                s_windows.erase(s_windows.begin() + i);
                i--;
            }
        }
    }
}

Window& Window::find(const GLFWwindow* window) {
    for (Window* w : s_windows) {
        if (w->m_window == window) {
            return *w;
        }
    }
    throw std::string("ERROR: failed to find the window.");
}



// ----- Setters -----

Window& Window::setMain() {
    if (m_window == nullptr) {
        return *this;
    }

    s_mainWindow = this->m_window;
    s_forceClose = false;
    return *this;
}    

Window& Window::setBackground(Colour col) {
    if (m_window == nullptr) {
        return *this;
    }

    float r = col.r * (1. / Colour::MAX);
    float g = col.g * (1. / Colour::MAX);
    float b = col.b * (1. / Colour::MAX); 
    float a = col.a * (1. / Colour::MAX);
    glfwMakeContextCurrent(this->m_window);
    glClearColor(r, g, b, a);

    return *this;
}    

Window& Window::setSize(int width, int height) {
    if (m_window == nullptr) {
        return *this;
    }

    // Change size variable
    m_width = width;
    m_height = height;
    m_resizedMutex.lock();
    m_resized = true;
    m_resizedMutex.unlock();
    
    return *this;
}  

Window& Window::setSize(int minWidth, int minHeight, int maxWidth, int maxHeight) {
    if (m_window == nullptr) {
        return *this;
    }

    // Change size variable
    glfwSetWindowSizeLimits(m_window, minWidth, minHeight, maxWidth, maxHeight);
    
    return *this;
}

Window& Window::setTitle(const std::string& title) {
    if (m_window == nullptr) {
        return *this;
    }
    
    glfwSetWindowTitle(m_window, title.c_str());
    return *this;
}

Window& Window::add(Renderable& obj) {
    this->m_renderObjects.push_back(obj);
    return *this;
}

bool Window::remove(uint64_t index) {
    // TODO - add removal logic
    (void)index;
    return false;
}

bool Window::remove(Renderable& obj) {
    // TODO - add removal logic
    (void)obj;
    return false;
}



// ----- Getters -----

Window& Window::getSize(int& width, int& height) {
    width = m_width;
    height = m_height;
    return *this;
}

std::vector<Renderable> Window::renderables() {
    return m_renderObjects;
}



// ----- Callbacks -----

Window& Window::setCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(this->m_window, callback);
    return *this;
}

Window& Window::setCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(this->m_window, callback);
    return *this;
}

Window& Window::setCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(this->m_window, callback);
    return *this;
}

Window& Window::setCallback(GLFWcharfun callback) {
    glfwSetCharCallback(this->m_window, callback);
    return *this;
}

Window& Window::setCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(this->m_window, callback);
    return *this;
}

Window& Window::setCallback(GLFWwindowrefreshfun callback) {
    glfwSetWindowRefreshCallback(this->m_window, callback);
    return *this;
}



// ----- Update -----

void Window::run(const std::function<void(void)>& lambda) {
    run(DEFAULT_FRAMERATE, true, lambda);
}

void Window::run(bool upsMatchFps, const std::function<void(void)>& lambda) {
    run(DEFAULT_FRAMERATE, upsMatchFps, lambda);
}

void Window::run(uint64_t frames, bool upsMatchFps, const std::function<void(void)>& lambda) {
    // Remove context to set window in thread
    glfwMakeContextCurrent(nullptr);
    if (frames == 0) {
        m_frameTime = 0;
    }
    else {
        m_frameTime = std::chrono::nanoseconds((std::chrono::nanoseconds(1s).count() / frames)).count();
    }

    // Add window to list for main thread to close
    s_windows.emplace_back(this);
    
    // Put onto thread
    std::thread t(&Window::loop, this, upsMatchFps, lambda);
    t.detach();
}

bool Window::hasFrameTimePassed(bool upsMatchFps) {
    // If no specified rendering fps
    if (this->m_frameTime == 0) {
        return true;
    }
    
    // Sleeps and return true so the Updates per Second match the Frames per Second
    if (upsMatchFps) {
        uint64_t current = GET_CURRENT_TIME();
        int64_t delta = static_cast<int64_t>((current - m_prevTime) - m_frameTime);
        
        std::this_thread::sleep_for(std::chrono::nanoseconds(m_frameTime - delta));
        m_prevTime = current - delta;
        return true;
    }

    // UPS can be anything, so no sleep
    uint64_t current = GET_CURRENT_TIME();
    if (current > m_prevTime && m_frameTime <= (current - m_prevTime)) {
        // Updates the previous time based on current, previous, and frametime
        // current - previous = elapsed time
        // elapsed - frame = true delta
        // current - delta = adjusted time
        m_prevTime = current - ((current - m_prevTime) - m_frameTime);
        return true;
    }
    return false;
}

void Window::render(bool upsMatchFps) {
    // Check time since last frame
    if (!this->hasFrameTimePassed(upsMatchFps)) {
        return;
    }
    
    // Set background
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render objects
    for (Renderable& obj : m_renderObjects) {
        obj.render();
    }

    glfwSwapBuffers(this->m_window);
}

bool Window::shouldClose() {
    // If this is the main window, check if its closed
    if (s_mainWindow != nullptr && this->m_window == s_mainWindow) {
        s_forceClose = glfwWindowShouldClose(this->m_window);
        if (s_forceClose) {
            s_mainWindow = nullptr;
        }
        return s_forceClose;
    }
    
    return glfwWindowShouldClose(this->m_window);
}

void Window::loop(bool upsMatchFps, const std::function<void(void)>& lambda) {
    std::println("Thread made on {}", std::this_thread::get_id());
    m_prevTime = GET_CURRENT_TIME();
    
    // Set window on this thread
    glfwMakeContextCurrent(this->m_window);
    
    // Loop while window is open
    while (!this->shouldClose() && !s_forceClose) {
        // Run user lambda
        lambda();

        // Check for updates
        checkUpdates();
        
        // Render objects
        this->render(upsMatchFps);
    }
    
    // Window is closed
    glfwMakeContextCurrent(nullptr);
    std::println("Thread ended on {}", std::this_thread::get_id());
    m_closed = true;
}

void Window::checkUpdates() {
    if (m_resized) {
        m_resizedMutex.lock();
        m_resized = false;
        glViewport(0, 0, m_width, m_height);
        m_resizedMutex.unlock();
    }
}

