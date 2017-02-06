#pragma once
#include "Game/Game.h"
#include "Render/RenderDefBuffers.h"
#include "Event/Input/InputKeyState.h"
#include "Render/DeferredMeshShader.h"
#include "GridDataStructure.h"

class TestGame
	: public Game {
protected:
	RenderDefBuffers _bufferRenderer;
	InputKeyState _renderBuffer;

	DeferredMeshShader _shader;
	Material _material;

	Grid _gridtest;
	Mesh _wallMesh, _cube;

public:

	TestGame(Setting &setting, EventManager &events);
	virtual ~TestGame();

	virtual void initiate();
	virtual void compose(RenderInfo &rI);
};