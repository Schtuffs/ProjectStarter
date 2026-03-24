#include "RenderManager.h"

#include <cstdint>
#include <fstream>
#include <print>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static uint64_t s_inits = 0;

void RenderManager::init() {
    s_inits++;
    if (s_inits != 1) {
        return;
    }
    
    // Init glfw
    if (!glfwInit()) {
        std::println("Could not initialize glfw...");
        exit(EXIT_FAILURE);
    }

    // GLFW v3.3, CORE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void RenderManager::terminate() {
    s_inits--;
    if (s_inits != 0) {
        return;
    }

    glfwTerminate();
}

std::string read(const std::string& filename) {
    // Create file
    std::ifstream in(filename, std::ios::binary);
	if (!in.is_open()) {
		throw 1;
	}

	std::string contents;
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();
	return contents;
}

// Returns original ID or 0 based on success
uint32_t compileErrors(GLuint id, GLuint type) {
    char error[1024];
    GLsizei read;
    GLint status = GL_TRUE;
    
    // For the vertex and fragment shaders
    glGetShaderiv(id, type, &status);
    if (status != GL_TRUE) {
        if (type == GL_COMPILE_STATUS) {
            glGetShaderInfoLog(id, 1024, &read, error);
        }
        else {
            glGetProgramInfoLog(id, 1024, &read, error);
        }

        std::println(stderr, "COMPILE ERROR: {}", error);
        return 0;
    }
    return id;
}

GLuint readProgram(const char* file, GLuint shaderType) {
    std::string code;
    try {
        code = read(file);
    } catch (int e) {
        std::println(stderr, "ERROR: Could not read file: {}", file);
        return 0;
    }

    // Compile shader
    const char* source = code.c_str();
    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);
    return compileErrors(shaderID, GL_COMPILE_STATUS);
}

GLuint RenderManager::createShader(const char* vertexFile, const char* fragmentFile) {
    RenderManager::init();

    GLuint vertShader = readProgram(vertexFile, GL_VERTEX_SHADER);
    GLuint fragShader = readProgram(fragmentFile, GL_FRAGMENT_SHADER);
    
    // Bind the shaders to the shading program
    GLuint shader = 0;
    if (vertShader && fragShader) {
        shader = glCreateProgram();
        glAttachShader(shader, vertShader);
        glAttachShader(shader, fragShader);
        glLinkProgram(shader);
        compileErrors(shader, GL_LINK_STATUS);
    }

    if (vertShader) {
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDeleteShader(fragShader);
    }

    return shader;
}

void RenderManager::destroyShader(GLuint shader) {
    glDeleteProgram(shader);
    RenderManager::terminate();
}

GLuint RenderManager::createVAO() {
    RenderManager::init();
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}

void RenderManager::destroyVAO(GLuint vao) {
    glDeleteVertexArrays(1, &vao);
    RenderManager::terminate();
}

GLuint RenderManager::createVBO() {
    RenderManager::init();
    GLuint vbo;
    glGenBuffers(1, &vbo);
    return vbo;
}

void RenderManager::destroyVBO(GLuint vbo) {
    glDeleteBuffers(1, &vbo);
    RenderManager::terminate();
}

GLuint RenderManager::createEBO() {
    RenderManager::init();
    GLuint ebo;
    glGenBuffers(1, &ebo);
    return ebo;
}

void RenderManager::destroyEBO(GLuint ebo) {
    glDeleteBuffers(1, &ebo);
    RenderManager::terminate();
}

void RenderManager::bindShader(GLuint shader) {
    glUseProgram(shader);
}

void RenderManager::bindVAO(GLuint vao) {
    glBindVertexArray(vao);
}

void RenderManager::bindVBO(GLuint vbo, GLfloat* vertices, GLsizeiptr size) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

void RenderManager::bindEBO(GLuint ebo, GLuint* indices, GLsizeiptr size) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void RenderManager::bindTex(GLuint tex) {
    glBindTexture(GL_TEXTURE_2D, tex);
}

void RenderManager::linkAttrib(GLuint layout, GLuint components, GLenum type, GLsizeiptr stride, void* offset) {
    glVertexAttribPointer(layout, components, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
}

// Unbinding

void RenderManager::unbindShader() {
    glUseProgram(0);
}

void RenderManager::unbindVAO() {
    glBindVertexArray(0);
}

void RenderManager::unbindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderManager::unbindEBO() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderManager::unbindTex() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderManager::unbindAll() {
    unbindShader();
    unbindVAO();
    unbindVBO();
    unbindEBO();
    unbindTex();
}


