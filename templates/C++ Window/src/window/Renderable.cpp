#include "window/Renderable.h"

#include "window/RenderManager.h"

Renderable::Renderable(const std::function<void(void)>& lambda) : m_renderLambda(lambda) {
    RenderManager::init();
    m_vao = RenderManager::createVAO();
    m_vbo = RenderManager::createVBO();
    m_ebo = RenderManager::createEBO();
}

Renderable::~Renderable() {
    RenderManager::destroyVAO(m_vao);
    RenderManager::destroyVBO(m_vbo);
    RenderManager::destroyEBO(m_ebo);
    RenderManager::terminate();
}

void Renderable::render() {
    m_renderLambda();
}

