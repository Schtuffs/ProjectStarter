#include "WindowManager.h"

#include <mutex>
#include <print>

#include "RenderManager.h"
#include "Window.h"

static int s_winNum = 0;
static std::mutex s_winMtx;

void WindowManager::add() {
    RenderManager::init();
    std::lock_guard lock(s_winMtx);
    s_winNum++;
}

void WindowManager::remove() {
    RenderManager::terminate();
    std::lock_guard lock(s_winMtx);
    s_winNum--;
}

bool WindowManager::hasWindows() {
    return s_winNum;
}

