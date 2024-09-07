cmake_minimum_required(VERSION 3.10)
project(learn-opengl-fr)
add_executable(${PROJECT_NAME} src/main.cpp)
set(LIBS glfw GLEW OpenGL)
target_link_libraries(${PROJECT_NAME} ${LIBS})

