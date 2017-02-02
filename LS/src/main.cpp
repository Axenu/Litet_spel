#include "gl/glInclude.h"
#include <iostream>
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
#include "gui/Button.h"
#include "gui/Manager.h"

void setupWindow()
{
#ifdef _WIN32
	//Memory leak debug
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

	EventManager eventManager;
	InputManager iManager(window, &eventManager);


	//basic init
	GraphicsResource resource(gl::DefferredSettings(wWidth, wHeight, 3));
	Shader s("Basic");
	DeferredMeshShader meshShader;
	RenderDeferred deferred(resource.getQuad());
	Material material(&meshShader);
	material.setColor("diffuse", glm::vec4(0.8f));
	material.setColor("spec", glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	Scene scene;
	gl::CheckGLErrors("Init stage failed: Resource");

	Grid gridtest;
	Mesh wallMesh = gridtest.generateMesh();
	Mesh cube;
	//avoid this:
	// Model guardModel(MeshPart(&cube, &material));
	//do this instead:
	MeshPart guardModelMeshPart(&cube, &material);
	Model guardModel(guardModelMeshPart);
	MeshPart goModelMeshPart(&wallMesh, &material);
	Model goModel(goModelMeshPart);

	Camera camera(70.0f, wWidth, wHeight, 0.1f, 100.0f);
	deferred.setWindowSize((float)wWidth, (float)wHeight, camera);


    Character* player = new Character(glm::vec3(3.0f, 0.0f, 5.0f), &eventManager);
	player->setLevel(&gridtest);
    player->setCamera(&camera);
	camera.setParent(player);

	//Add some more game objects	
	scene.add(player);
	GameObject *guard = new Guard(guardModel, &gridtest);
	scene.add(guard);
	scene.add(new GameObject(goModel));
	guard = scene.remove(guard);

	//Add some lights
	scene.add(new PointLightObject(PointLight(glm::vec3(0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f), player));
	scene.add(new PointLightObject(PointLight(glm::vec3(3.0f, 1.0f, 5.0f), glm::vec3(0.8f, 0.3f, 0.3f), glm::vec3(1.0f, 0.0f, 0.0f), 5.0f)));

	// gui::Font *font = new gui::Font("Resources/fonts/arial");
	// gui::Label label(font, "Hello World!");
	// label.setZ(99);
	// gui::Rectangle rect(0.5, 0.5);
	// glm::vec4 color(0,0,0,1);
	// rect.setColor(color);
	gui::Button* button = new gui::Button("CLick me!");
	button->setPosition(0,1.8);
	gui::Scene guiScene = gui::Scene();
	guiScene.addChild(button);
	gui::Manager guiManager(&eventManager);
	guiManager.setWindowSize(640, 480);
	guiManager.setScene(&guiScene);

	//init dt calculation
	float lastTime = glfwGetTime();

/* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //update
		//Calculate dt
		float currentTime = glfwGetTime();
	    float dT = currentTime - lastTime;
	    lastTime = currentTime;
		// float dT = tpf(lastTime);
		// dT = 0.016;
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

		guiManager.update(dT);
		guiManager.render();

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
