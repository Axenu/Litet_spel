#include "Game/Game.h"



Game::Game(Setting &setting, EventManager &events)
	: _setting(setting), _event(events), _scene(), _resource(setting._renderSetting), _deferred(_resource.getQuad()), _camera(setting)
{
	_deferred.setWindowSize((float)setting.Width(), (float)setting.Height(), _camera);
}


Game::~Game()
{
}



void Game::initiate() {

}


void Game::update(float dT) {
	_scene.update(dT);

}
void Game::draw() {

	DrawFrame dF;
	_scene.fetchDrawables(dF);
	RenderInfo rI(_resource, _camera, dF.getLightInfo());

	_resource.getDeffered().bindDraw();
	dF.render(rI);
	gl::CheckGLErrors("Render stage failed: Mesh");
	/*	Render to backbuffer:
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	compose(rI);
}
void Game::compose(RenderInfo &rI) {
	//Render
	_deferred.render(rI);
	gl::CheckGLErrors("Render stage failed: Composition");

}