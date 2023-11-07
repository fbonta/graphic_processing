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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int glWindowWidth = 640;
int glWindowHeight = 480;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

GLuint shaderProgram1;
GLuint shaderProgram2;

GLuint firstTriangleShader;
GLuint secondTriangleShader;

GLfloat vertexCoordinates1[] = {
    0.3f, 0.3f, 0.0f,
    -0.3f, 0.3f, 0.0f,
    0.3f, -0.3f, 0.0f,
};
GLfloat vertexCoordinates2[] = {
    -0.3f, -0.3f, 0.0f,
    -0.3f, 0.3f, 0.0f,
    0.3f, -0.3f, 0.0f,
};

GLuint verticesVBO1;
GLuint triangleVAO1;

GLuint verticesVBO2;
GLuint triangleVAO2;

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
    //TODO
}

void initObjects()
{
    glGenBuffers(1, &verticesVBO1);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoordinates1), vertexCoordinates1, GL_STATIC_DRAW);
    glGenVertexArrays(1, &triangleVAO1);
    glBindVertexArray(triangleVAO1);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glGenBuffers(1, &verticesVBO2);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoordinates2), &vertexCoordinates2, GL_STATIC_DRAW);
    glGenVertexArrays(1, &triangleVAO2);
    glBindVertexArray(triangleVAO2);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
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
        glfwTerminate();
        return false;
    }

    glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
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

    return true;
}

void renderScene()
{
    static bool a_pressed = false;
    static bool d_pressed = false;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8f, 0.8f, 0.8f, 1);
    glViewport(0, 0, retina_width, retina_height);

    if (GLFW_PRESS == glfwGetKey(glWindow, GLFW_KEY_A))
    {
        a_pressed = true;
    }

    if (GLFW_PRESS == glfwGetKey(glWindow, GLFW_KEY_D))
    {
        d_pressed = true;
    }

	if ((GLFW_RELEASE == glfwGetKey(glWindow, GLFW_KEY_A)) && a_pressed)
    {
        firstTriangleShader = firstTriangleShader ^ shaderProgram1 ^ shaderProgram2;
        a_pressed = false;
    }

    if ((GLFW_RELEASE == glfwGetKey(glWindow, GLFW_KEY_D)) && d_pressed)
    {
        secondTriangleShader = secondTriangleShader ^ shaderProgram1 ^ shaderProgram2;
        d_pressed = false;
    }

    glUseProgram(firstTriangleShader);
    glBindVertexArray(triangleVAO1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(secondTriangleShader);
    glBindVertexArray(triangleVAO2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

std::string readShaderFile(std::string fileName)
{
    std::ifstream shaderFile;
    std::string shaderString;

    //open shader file
    shaderFile.open(fileName);

    std::stringstream shaderStringStream;

    //read shader content into stream
    shaderStringStream << shaderFile.rdbuf();

    //close shader file
    shaderFile.close();

    //convert stream into GLchar array
    shaderString = shaderStringStream.str();
    return shaderString;
}

void shaderCompileLog(GLuint shaderId)
{
    GLint success;
    GLchar infoLog[512];

    //check compilation info
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "Shader compilation error\n" << infoLog << std::endl;
    }
}

void shaderLinkLog(GLuint shaderProgramId)
{
    GLint success;
    GLchar infoLog[512];

    //check linking info
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
        std::cout << "Shader linking error\n" << infoLog << std::endl;
    }
}

GLuint initBasicShader(std::string vertexShaderFileName, std::string fragmentShaderFileName)
{
    GLuint shaderProgram;

    //read, parse and compile the vertex shader
    std::string v = readShaderFile(vertexShaderFileName);
    const GLchar* vertexShaderString = v.c_str();
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderString, NULL);
    glCompileShader(vertexShader);
    //check compilation status
    shaderCompileLog(vertexShader);

    //read, parse and compile the fragment shader
    std::string f = readShaderFile(fragmentShaderFileName);
    const GLchar* fragmentShaderString = f.c_str();
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
    glCompileShader(fragmentShader);
    //check compilation status
    shaderCompileLog(fragmentShader);

    //attach and link the shader programs
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //check linking info
    shaderLinkLog(shaderProgram);

    return shaderProgram;
}

void cleanup() {
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

    shaderProgram1 = initBasicShader("shaders/shader1.vert", "shaders/shader.frag");
    shaderProgram2 = initBasicShader("shaders/shader2.vert", "shaders/shader.frag");

    firstTriangleShader = shaderProgram1;
    secondTriangleShader = shaderProgram2;

    while (!glfwWindowShouldClose(glWindow)) {
        renderScene();

        glfwPollEvents();
        glfwSwapBuffers(glWindow);
    }

    cleanup();

    return 0;
}