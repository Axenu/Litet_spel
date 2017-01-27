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
#include "Render\GraphicsResource.h"
#include "Render\FrameData.h"
#include "Render\RenderDeferred.h"
#include "InputManager.h"

GLFWwindow* window;
Grid gridtest;
InputManager manager;

// GLFW key callbacks.
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE)
    {
         glfwSetWindowShouldClose(win, 1);
    }
    else
    {
        InputManager * manager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
        manager->fireKey(key, action);
    }
}

void mouse_key_callback(GLFWwindow* win, int button, int action, int mods) {
    InputManager * manager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
    manager->fireMouseClick(button, action);
}

void cursorPosition_callback(GLFWwindow* win, double x, double y) {
    InputManager * manager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
    manager->fireMouseMove(x, y);
}

void setupWindow() {
    // init glfw
	if (!glfwInit()) {
		std::cout << "GLFW init failed!" << std::endl;
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_DECORATED, true);
	unsigned int wWidth = 640, wHeight = 480;
    window = glfwCreateWindow(wWidth, wHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "GLFW creation of window failed!" << std::endl;
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
	GraphicsResource resource(gl::DefferredSettings(wWidth, wHeight, 3));
	Shader *s = new Shader("Basic");
	Shader *def_mesh = new Shader("Deferred_Mesh");
	RenderDeferred deferred(resource.getQuad());

	if (gl::CheckGLErrors("Initiation failed: GL Error"))
		throw new std::exception("Initiation failed: GL Error");

	Model *m = new Model(s->shaderProgram);

    glfwSetWindowUserPointer(window, &manager);
	keyCallback qcallback;
    manager.subscribeToKey(GLFW_KEY_Q, qcallback);

    while (!glfwWindowShouldClose(window))
    {
		FrameData fD(resource);
        /* Render here */
        glClearColor(1, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //update
		m->update(0.016f);
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
