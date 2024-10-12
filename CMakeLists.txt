cmake_minimum_required(VERSION 3.10)
project(learn-opengl-fr)

add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(hello_triangle_ex1 src/hello_triangle_ex1.cpp)
add_executable(hello_triangle_ex2 src/hello_triangle_ex2.cpp)
add_executable(hello_triangle_ex3 src/hello_triangle_ex3.cpp)

add_executable(shaders_ex1 src/shaders_ex1.cpp)
add_executable(shaders_ex2 src/shaders_ex2.cpp)

add_executable(textures src/textures.cpp src/stb_image.cpp)

add_executable(compute src/compute_shader_test.cpp)
add_executable(prefixsum src/prefix_sum.cpp)


set(LIBS glfw GLEW OpenGL)

target_link_libraries(${PROJECT_NAME} ${LIBS})
target_link_libraries(hello_triangle_ex1 ${LIBS})
target_link_libraries(hello_triangle_ex2 ${LIBS})
target_link_libraries(hello_triangle_ex3 ${LIBS})

target_link_libraries(shaders_ex1 ${LIBS})
target_link_libraries(shaders_ex2 ${LIBS})

target_link_libraries(textures ${LIBS})

target_link_libraries(compute ${LIBS})
target_link_libraries(prefixsum ${LIBS})
