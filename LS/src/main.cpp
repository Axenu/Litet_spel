#include "gl/glInclude.h"
#include <iostream>
#define GLM_FORCE_RADIANS
#include <IL/il.h>
#include <glm/glm.hpp>
#include "Event/Input/InputManager.h"
#include "Event/EventManager.h"
#include "Game/Setting.h"
#include "Game/Game.h"
#include "Game/TestGame.h"
#include "gui/Button.h"
#include "gui/Manager.h"
#include "gui/MainMenuScene.h"

void setupWindow()
{
#ifndef __APPLE__
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
	bool windowalive=false;

	//basic init
	Setting setting(wWidth, wHeight, 3, 0.1f, 100.f, 70.f);
	setting._renderSetting._textureSetup[2] = GL_RGBA; //Specular = RGBA buffer

	TestGame game(setting, eventManager);

	game.initiate();
	gl::CheckGLErrors("Init stage failed: Resource");

	// gui::Font *font = new gui::Font("Resources/fonts/arial");
	// gui::Label label(font, "Hello World!");
	// label.setZ(99);
	// gui::Rectangle rect(0.5, 0.5);
	// glm::vec4 color(0,0,0,1);
	// rect.setColor(color);

	//init dt calculation
	float lastTime = (float)glfwGetTime();
	float currentTime;
	float dT;
	float FPS;

	gui::MainMenuScene* guiScene = new gui::MainMenuScene(&eventManager, &FPS);
	gui::Manager guiManager(&eventManager);
	guiManager.setWindowSize(640, 480);
	guiManager.setScene(guiScene);



/* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		windowalive = game.closeWindow();
		if (windowalive == true)
		{
			break;
		}
        //update
		//Calculate dt
		currentTime = (float)glfwGetTime();
		dT = currentTime - lastTime;
	    lastTime = currentTime;
		FPS = 1.0f/dT;
		//Update game logic
		game.update(dT);
		//Draw game drawables
		game.draw();

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
