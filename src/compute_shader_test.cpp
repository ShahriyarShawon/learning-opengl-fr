#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <math.h>
#include <vector>
#include <random>
#include <cstring>

#include "shader.h"

#define ARRAY_SIZE 1000

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char **argv) {
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
    // glViewport(0, 0, 800, 600);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::vector<float> inputData1(ARRAY_SIZE);
    std::vector<float> inputData2(ARRAY_SIZE);
    std::vector<float> outputData(ARRAY_SIZE);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        inputData1[i] = dis(gen);
        inputData2[i] = dis(gen);
    }

	std::cout<<"Done Generating\n";


    GLuint inputBuffer1, inputBuffer2, outputBuffer;

    glGenBuffers(1, &inputBuffer1);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBuffer1);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * ARRAY_SIZE,
                 inputData1.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBuffer1);

    glGenBuffers(1, &inputBuffer2);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBuffer2);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * ARRAY_SIZE,
                 inputData2.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, inputBuffer2);

    glGenBuffers(1, &outputBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * ARRAY_SIZE,
                 NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, outputBuffer);

    ComputeShader cs("../shaders/compute/compute.comp");


    cs.use();
    glDispatchCompute(ARRAY_SIZE / 256+1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Read back the data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
    GLvoid *p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    memcpy(outputData.data(), p, sizeof(float) * ARRAY_SIZE);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    // Verify results
    for (int i = 0; i < ARRAY_SIZE; i++) {
        float expected = inputData1[i] + inputData2[i];
        if (abs(outputData[i] - expected) > 0.0001f) {
            std::cout << "Verification failed at index " << i << std::endl;
            std::cout << "Expected: " << expected << ", Got: " << outputData[i]
                      << std::endl;
            break;
        }
    }
    glDeleteBuffers(1, &inputBuffer1);
    glDeleteBuffers(1, &inputBuffer2);
    glDeleteBuffers(1, &outputBuffer);

    // while (!glfwWindowShouldClose(window)) {
    //     glDrawArrays(GL_TRIANGLES, 0, 3);
    //
    //     glfwSwapBuffers(window);
    //     glfwPollEvents();
    // }

    glfwTerminate();
    return 0;
}
