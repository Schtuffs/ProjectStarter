cmake_minimum_required(VERSION 3.25)
project(Window C CXX)

set(SRC ${CMAKE_CURRENT_LIST_DIR}/src)
set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(LIB ${CMAKE_CURRENT_LIST_DIR}/lib)

add_library(${PROJECT_NAME} STATIC
    ${SRC}/Callbacks.cpp
    ${SRC}/Colour.cpp
    ${SRC}/glad.c
    ${SRC}/main.cpp
    ${SRC}/Window.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE})

# OpenGL required
find_package(OpenGL REQUIRED)

# GLFW required
find_package(glfw3 CONFIG REQUIRED)

# Add required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
endif()

