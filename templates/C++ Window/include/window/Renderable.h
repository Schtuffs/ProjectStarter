#pragma once

#include <functional>
#include <string>

#include <glad/glad.h>

/**
 * @class Renderable
 * @brief Derivable class for creating renderable objects.
 * @author Kyle Wagler
 * @date 2026-03-23
 */
class Renderable {
public:
    /**
     * @brief Initializes buffer objects for rendering. Derived class MUST make own shader with RenderManager.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    Renderable(const std::function<void(void)>& lambda = [](){});
    /**
     * @brief Deletes own buffer objects.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    ~Renderable();

    /**
     * @brief The main render function. Calls m_lambda.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void render();

protected:
    GLuint m_vao, m_vbo, m_ebo, m_shader;
    std::function<void(void)> m_renderLambda;
};

