cmake_minimum_required(VERSION 3.25)
project(proj CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME}
    ${SRC}/FileStructure.cpp
    ${SRC}/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)
endif()

