#pragma once
#include "Game/Game.h"
#include "Render/Composition/RenderDefBuffers.h"
#include "Render/Composition/RenderCubeMap.h"
#include "Event/Input/InputKeyState.h"
#include "Render/Mesh/Shader/DeferredMeshShader.h"
#include "Game/Level/Level.h"
#include "Game/Objects/Character.h"
#include "Game/Objects/Guard.h"
#include "Game/Objects/PointLightObject.h"
#include "StaticVars.h"

class TestGame
	: public Game {
protected:
	RenderDefBuffers _bufferRenderer;
	RenderCubeMap _cubeMapRenderer;
	InputKeyState _renderBufferKey;
	InputKeyState _cubeMapBufferKey;
	Character *_player;

	virtual void setupRI(RenderInfo &rI);
public:

	TestGame(Setting &setting, EventManager &events);
	virtual ~TestGame();
	virtual void initiate();
	virtual void compose(RenderInfo &rI);
	Character *getCharacter();
};
