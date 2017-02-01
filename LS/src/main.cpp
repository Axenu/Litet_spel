#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define GLEW_STATIC
#include <GL/glew.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <iostream>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <IL/il.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "GridDataStructure.h"
#include "Render/GraphicsResource.h"
#include "Render/RenderInfo.h"
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
#include "gui/Label.h"
#include "gui/Rectangle.h"


void setupWindow()
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
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
	GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "Hello World", NULL, NULL);
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

	//init DevIL
	ilInit();

	//Set GL vars
	glEnable(GL_DEPTH_TEST);//Enable depth testinz
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);//Enable face culling
	glCullFace(GL_BACK);
	glEnable(GL_BLEND); //Enable alpha on gui elements. Could be done every frame on render?
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	Grid gridtest;
	Mesh wallMesh = gridtest.generateMesh();
	Mesh cube;
	Model guardModel(&cube, &meshShader, &material);
	Model goModel(&wallMesh, &meshShader, &material);

	Camera camera = Camera(70.0f, wWidth, wHeight, 0.1f, 100.0f);
	deferred.setWindowSize((float)wWidth, (float)wHeight, camera);


    Character* player = new Character(glm::vec3(3.0f, 0.0f, 5.0f),eventManager);
	player->setLevel(&gridtest);
    player->setCamera(&camera);
	camera.setParent(player);

	//Add some more game objects
	scene.add(player);
	scene.add(new Guard(guardModel, &gridtest));
	scene.add(new GameObject(goModel));

	//Add some lights
	scene.add(new PointLightObject(PointLight(glm::vec3(0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f), player));
	scene.add(new PointLightObject(PointLight(glm::vec3(3.0f, 1.0f, 5.0f), glm::vec3(0.8f, 0.3f, 0.3f), glm::vec3(1.0f, 0.0f, 0.0f), 5.0f)));

	Font *f = new Font("Resources/fonts/arial");
	gui::Label *label = new gui::Label(f, "Hello World!");


/* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //update
		float dT = 0.016f;
		scene.update(dT);

		DrawFrame dF;
		scene.fetchDrawables(dF);
		RenderInfo fD(resource, camera, dF.getLightInfo());

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


		label->render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}

int main()
{
	std::cout << "Init window!" << std::endl;
	setupWindow();


    return 0;
}
