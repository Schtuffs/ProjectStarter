cmake_minimum_required(VERSION 3.25)
project(Window C CXX)

set(CMAKE_CXX_STANDARD 23)
set(WINDOW_SRC ${CMAKE_CURRENT_LIST_DIR}/src/window)
set(SRC ${CMAKE_CURRENT_LIST_DIR}/src)
set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(LIB ${CMAKE_CURRENT_LIST_DIR}/lib)

add_executable(${PROJECT_NAME}
    ${WINDOW_SRC}/glad.c
    ${WINDOW_SRC}/Colour.cpp
    ${WINDOW_SRC}/Library.cpp
    ${WINDOW_SRC}/Renderable.cpp
    ${WINDOW_SRC}/RenderManager.cpp
    ${WINDOW_SRC}/Window.cpp
    ${WINDOW_SRC}/WindowManager.cpp
    ${SRC}/Callbacks.cpp
    ${SRC}/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE})

# OpenGL required
find_package(OpenGL REQUIRED)

# GLFW required
find_package(glfw3 CONFIG REQUIRED)

# Add required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)

target_link_libraries(${PROJECT_NAME} PRIVATE stdc++exp)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)
    set_source_files_properties(${WINDOW_SRC}/glad.c PROPERTIES COMPILE_OPTIONS -Wno-pedantic)
endif()

