#include <iostream>
#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <cstring>

#define ARRAY_SIZE 2048

void prefix_sum(const float *in_array, float *out_array, int elements,
                bool inclusive) {
    float f = 0.0f;
    int i;

    if (inclusive) {
        for (i = 0; i < elements; i++) {
            f += in_array[i];
            out_array[i] = f;
        }
    } else {
        for (i = 0; i < elements; i++) {
            out_array[i] = f;
            f += in_array[i];
        }
    }
}

int main() {
    glfwInit();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create a GLFW Window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

	std::vector<float> inputData(ARRAY_SIZE);
	std::vector<float> outputData(ARRAY_SIZE);
	std::vector<float> computeOutput(ARRAY_SIZE);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        inputData[i] = dis(gen);
    }

	prefix_sum(inputData.data(), outputData.data(), ARRAY_SIZE, true);

    GLuint inputBuffer, outputBuffer;

    glGenBuffers(1, &inputBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * ARRAY_SIZE,
                 inputData.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBuffer);

    glGenBuffers(1, &outputBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * ARRAY_SIZE,
                 NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, outputBuffer);

    ComputeShader cs("../shaders/compute/prefix_sum.glsl");


    cs.use();
    glDispatchCompute(ARRAY_SIZE / 256+1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Read back the data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
    GLvoid *p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    memcpy(computeOutput.data(), p, sizeof(float) * ARRAY_SIZE);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glDeleteBuffers(1, &inputBuffer);
    glDeleteBuffers(1, &outputBuffer);


    glfwTerminate();
    return 0;
}
