//
//  main.cpp
//  OpenGL_Shader_Example_step1
//
//  Created by CGIS on 30/11/15.
//  Copyright © 2015 CGIS. All rights reserved.
//

#include "Windows.h"

// enable optimus!
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#if defined (__APPLE__)
    #define GLFW_INCLUDE_GLCOREARB
#else
    #define GLEW_STATIC
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <string>
#include "Shader.hpp"

int glWindowWidth = 640;
int glWindowHeight = 480;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

gps::Shader myCustomShader;

GLfloat vertex_data[] = {
    0.0f, 0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
};

GLuint vertex_indices[] = {
    0, 1, 4,
    1, 2, 3,
    1, 3, 4,
};

GLuint vertices_vbo;
GLuint vertices_ebo;
GLuint object_vao;
GLenum mode;

GLint transform_vector;

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
    //TODO
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        
        glfwSetWindowShouldClose(glWindow, GLFW_TRUE);
    }
}

void initObjects()
{
    glGenVertexArrays(1, &object_vao);
    glBindVertexArray(object_vao);

    glGenBuffers(1, &vertices_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glGenBuffers(1, &vertices_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

    //vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

bool initOpenGLWindow()
{
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //window scaling for HiDPI displays
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    // for multisampling/antialising
    glfwWindowHint(GLFW_SAMPLES, 4);

    glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
    if (!glWindow) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        return false;
    }

    glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
    glfwSetKeyCallback(glWindow, key_callback);
    glfwMakeContextCurrent(glWindow);

#if not defined (__APPLE__)
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
#endif

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    //for RETINA display
    glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);
    mode = GL_LINE;

    return true;
}

void renderScene()
{
    static bool r_pressed = false;

    static bool up_pressed = false;
    static bool down_pressed = false;
    static bool left_pressed = false;
    static bool right_pressed = false;

    static float x = 0.0f;
    static float y = 0.0f;

    glClearColor(0.8f, 0.8f, 0.8f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, retina_width, retina_height);

    if (GLFW_PRESS == glfwGetKey(glWindow, GLFW_KEY_R)) {
        r_pressed = true;
    }

    if ((GLFW_RELEASE == glfwGetKey(glWindow, GLFW_KEY_R) && r_pressed) ) {
        r_pressed = false;
        mode = mode ^ GL_LINE ^ GL_FILL;
    }

    if (GLFW_PRESS == glfwGetKey(glWindow, GLFW_KEY_UP)) {
        up_pressed = true;
    }

    if ((GLFW_RELEASE == glfwGetKey(glWindow, GLFW_KEY_UP) && up_pressed)) {
        up_pressed = false;
        y += 0.02f;

        glUniform3f(transform_vector, x, y, 0.0f);
    }

    if (GLFW_PRESS == glfwGetKey(glWindow, GLFW_KEY_DOWN)) {
        down_pressed = true;
    }

    if ((GLFW_RELEASE == glfwGetKey(glWindow, GLFW_KEY_DOWN) && down_pressed)) {
        down_pressed = false;
        y -= 0.02f;

        glUniform3f(transform_vector, x, y, 0.0f);
    }

    if (GLFW_PRESS == glfwGetKey(glWindow, GLFW_KEY_LEFT)) {
        left_pressed = true;
    }

    if ((GLFW_RELEASE == glfwGetKey(glWindow, GLFW_KEY_LEFT) && left_pressed)) {
        left_pressed = false;
        x -= 0.02f;

        glUniform3f(transform_vector, x, y, 0.0f);
    }

    if (GLFW_PRESS == glfwGetKey(glWindow, GLFW_KEY_RIGHT)) {
        right_pressed = true;
    }

    if ((GLFW_RELEASE == glfwGetKey(glWindow, GLFW_KEY_RIGHT) && right_pressed)) {
        right_pressed = false;
        x += 0.02f;

        glUniform3f(transform_vector, x, y, 0.0f);
    }

    myCustomShader.useShaderProgram();

    glBindVertexArray(object_vao);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glDrawElements(GL_TRIANGLES, sizeof(vertex_indices), GL_UNSIGNED_INT, 0);
}

void cleanup() {

    if (vertices_vbo) {
        glDeleteBuffers(1, &vertices_vbo);
    }

    if (object_vao) {
        glDeleteVertexArrays(1, &object_vao);
    }

    glfwDestroyWindow(glWindow);
    //close GL context and any other GLFW resources
    glfwTerminate();
}

int main(int argc, const char * argv[]) {

    if (!initOpenGLWindow()) {
        glfwTerminate();
        return 1;
    }

    initObjects();

    myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
    myCustomShader.useShaderProgram();

    transform_vector = glGetUniformLocation(myCustomShader.shaderProgram, "transform_vector");

    while (!glfwWindowShouldClose(glWindow)) {
        renderScene();

        glfwPollEvents();
        glfwSwapBuffers(glWindow);
    }

    cleanup();

    return 0;
}