#pragma once
#include "Game/Game.h"
#include "Render/Composition/RenderDefBuffers.h"
#include "Event/Input/InputKeyState.h"
#include "Render/Mesh/Shader/DeferredMeshShader.h"
#include "GridDataStructure.h"
#include "Game/Objects/Character.h"
#include "Game/Objects/Guard.h"
#include "Game/Objects/PointLightObject.h"
#include "ModelLoader.h"

class TestGame
	: public Game {
protected:
	RenderDefBuffers _bufferRenderer;
	InputKeyState _renderBuffer;

	DeferredMeshShader _shader;
	Material _material, _lootMat;
	Character* player;
	Guard* guard;
	Grid _gridtest;
	Mesh _wallMesh, _cube;

	ModelLoader _modelLoader;


public:

	TestGame(Setting &setting, EventManager &events);
	virtual ~TestGame();
	virtual void initiate();
	virtual void compose(RenderInfo &rI);
};