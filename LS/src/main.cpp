#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define GLEW_STATIC
#include <GL/glew.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "GridDataStructure.h"
GLFWwindow* window;
Grid gridtest;
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    //  if (key == GLFW_KEY_Q) {
    //      if (action == GLFW_PRESS) {
    //          glfwSetWindowShouldClose(win, 1);
    //          return;
    //      }
    //  }

    //  engine->forwardKeyInput(key, action);
    std::cout << "key_callback" << std::endl;
}

void mouse_key_callback(GLFWwindow* win, int button, int action, int mods) {
    std::cout << "mouse_key_callback" << std::endl;
}

void cursorPosition_callback(GLFWwindow* win, double x, double y) {
    std::cout << "cursorPosition_callback" << std::endl;
}

void setupWindow() {
    // init glfw
	if (!glfwInit()) {
		std::cout << "GLFW init failed!" << std::endl;
    	// exit(EXIT_FAILURE);
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_DECORATED, true);
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "GLFW creation of window failed!" << std::endl;
        // exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
#endif

    // glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_key_callback);
    glfwSetCursorPosCallback(window, cursorPosition_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // glfwSetWindowPos(window, 0, 0);
    /* Loop until the user closes the window */

	//basic init

	Shader *s = new Shader("Basic");

	Model *m = new Model(s->shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(1, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //update
		m->update(0.016);
		m->render();
        //Render
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}
void Deanstestingruta()
{
	gridtest.print2darraydata();
}

int main()
{
	std::cout << "Init window!" << std::endl;
	Deanstestingruta();
	setupWindow();


    return 0;
}
