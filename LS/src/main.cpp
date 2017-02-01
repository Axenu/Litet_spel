#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define GLEW_STATIC
#include <GL/glew.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <iostream>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "GridDataStructure.h"
#include "Render/GraphicsResource.h"
#include "Render/FrameData.h"
#include "Render/RenderDeferred.h"
#include "Render/MeshShader.h"
#include "Render/DeferredMeshShader.h"
#include "InputManager.h"
#include "EventManager.h"
#include "Camera.h"
#include "Character.h"
#include "GameObject.h"
#include "Guard.h"
#include"Scene/Scene.h"
#include"Scene/DrawFrame.h"
GLFWwindow* window;
Grid gridtest;
Camera camera;
Character* player;
void setupWindow()
{
    // Init glfw
	if (!glfwInit())
	{
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
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
#endif
	//Set GL vars
	glEnable(GL_DEPTH_TEST);//Enable depth testinz
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);//Enable face culling
	glCullFace(GL_BACK);
	gl::CheckGLErrors("Init stage failed: State");

	EventManager *eventManager = new EventManager();
	InputManager *iManager = new InputManager(window, eventManager);


	//basic init
	GraphicsResource resource(gl::DefferredSettings(wWidth, wHeight, 3));
	Shader *s = new Shader("Basic");
	DeferredMeshShader meshShader;
	RenderDeferred deferred(resource.getQuad());
	Material material;
	Scene scene;
	gl::CheckGLErrors("Init stage failed: Resource");


	Mesh wallMesh = gridtest.generateMesh();
	Mesh cube;
	Guard guardenn(Model(&cube, &meshShader, &material), &gridtest);
	guardenn.update(0.16f);

	scene.add(new GameObject(Model(&wallMesh, &meshShader, &material)));
	scene.add(&guardenn);
	
	

    camera = Camera(70.0f, wWidth, wHeight, 0.1f, 100.0f);
    player = new Character(eventManager);
	player->setLevel(&gridtest);
    player->setCamera(&camera);
	deferred.setWindowSize((float)wWidth, (float)wHeight, camera);

/* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		// camera.rotateZ(0.001f);
        //update
		float dT = 0.016f;
		scene.update(dT);
        player->update(dT);
		camera.update(dT);

		FrameData fD(resource, camera);
		DrawFrame dF;
		scene.fetchDrawables(dF);

		resource.getDeffered().bindDraw();

		dF.render(fD);
		gl::CheckGLErrors("Render stage failed: Mesh");

		/*	Render to backbuffer:
		*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Render
		deferred.render(fD);
		gl::CheckGLErrors("Render stage failed: Composition");
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
