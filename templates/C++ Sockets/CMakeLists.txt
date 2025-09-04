cmake_minimum_required(VERSION 3.25)
project(Sockets CXX)

set(CMAKE_CXX_STANDARD 23)
set(SRC ${CMAKE_CURRENT_LIST_DIR}/src)
set(INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/include/sockets)
set(LIB ../lib)

add_executable(${PROJECT_NAME}
    ${SRC}/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE})
if (WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE Ws2_32)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "-lstdc++exp")

