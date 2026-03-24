#include "Renderable.h"

#include <fstream>
#include <print>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderManager.h"

Renderable::Renderable(const std::function<void(void)>& lambda) : m_lambda(lambda) {
    m_vao = RenderManager::createVAO();
    m_vbo = RenderManager::createVBO();
    m_ebo = RenderManager::createEBO();
}

Renderable::~Renderable() {
    RenderManager::destroyVAO(m_vao);
    RenderManager::destroyVBO(m_vbo);
    RenderManager::destroyEBO(m_ebo);
}

void Renderable::render() {
    m_lambda();
}

