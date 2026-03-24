#pragma once

#include <cstdint>

#include <glad/glad.h>

/**
 * @namespace RenderManager
 * @brief Handles much of the binding and generation of OpenGL items.
 * @author Kyle Wagler
 * @date 2026-03-23
 */
namespace RenderManager {
    /**
     * @brief Initializes GLFW.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void init();
    /**
     * @brief Terminates GLFW.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void terminate();
    
    /**
     * @brief Creates a shader program.
     * @param vertexFile The vertex file to use for the shader.
     * @param vertexFile The fragment file to use for the shader.
     * @return The ID of the shader program.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    GLuint createShader(const char* vertexFile, const char* fragmentFile);
    /**
     * @brief Destroys a shader program.
     * @param shader The ID of the shader program.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void destroyShader(GLuint shader);
    
    /**
     * @brief Creates a VAO object.
     * @return The ID of the VAO object.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    GLuint createVAO();
    /**
     * @brief Destroys a VAO object.
     * @param shader The ID of the VAO object.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void destroyVAO(GLuint vao);
    
    /**
     * @brief Creates a VBO object.
     * @return The ID of the VBO object.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    GLuint createVBO();
    /**
     * @brief Destroys a VBO object.
     * @param shader The ID of the VBO object.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void destroyVBO(GLuint vbo);
    
    /**
     * @brief Creates a EBO object.
     * @return The ID of the EBO object.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    GLuint createEBO();
    /**
     * @brief Destroys a EBO object.
     * @param shader The ID of the EBO object.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void destroyEBO(GLuint ebo);

    /**
     * @brief Binds a shader object.
     * @param shader The shader ID.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void bindShader(GLuint shader);
    /**
     * @brief Binds a VAO object.
     * @param vao The VAO ID.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void bindVAO(GLuint vao);
    /**
     * @brief Binds a VBO object.
     * @param vbo The VBO ID.
     * @param vertices The vertex data and any layout data.
     * @param size The total size of the array in bytes.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void bindVBO(GLuint vbo, GLfloat* vertices, GLsizeiptr size);
    /**
     * @brief Binds an EBO object.
     * @param ebo The EBO ID.
     * @param indices The order to draw triangles.
     * @param size The total size of the array in bytes.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void bindEBO(GLuint ebo, GLuint* indices, GLsizeiptr size);
    /**
     * @brief Binds a texture object.
     * @param tex The texture ID.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void bindTex(GLuint tex);
    /**
     * @brief Links attributes based on the VBO data.
     * @param layout The layout ID in the vertex shader.
     * @param components The number of attributes in the VBO to bind.
     * @param type The variable type of the attribute.
     * @param stride The total number of items per vertex in the VBO.
     * @param offset The first memory location of the layout item in the VBO in bytes. Ex `index * sizeof(EBO[index])`.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void linkAttrib(GLuint layout, GLuint components, GLenum type, GLsizeiptr stride, void* offset);
    
    /**
     * @brief Unbinds all shader objects.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void unbindShader();
    /**
     * @brief Unbinds all VAO objects.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void unbindVAO();
    /**
     * @brief Unbinds all VBO objects.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void unbindVBO();
    /**
     * @brief Unbinds all EBO objects.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void unbindEBO();
    /**
     * @brief Unbinds all texture objects.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void unbindTex();
    /**
     * @brief Unbinds all objects.
     * @author Kyle Wagler
     * @date 2026-03-23
     */
    void unbindAll();
}

