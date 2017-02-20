#include "Game/Game.h"



Game::Game(Setting &setting, EventManager &events)
	: _setting(setting), _event(events), _scene(), _resource(setting._renderSetting), _deferred(_resource.getQuad()), 
	_factory(_scene, events, "Resources/")
{
	Camera& cam = _scene.setCamera(setting);
	_deferred.setWindowSize((float)setting.Width(), (float)setting.Height(), cam);
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
	RenderInfo rI(_resource, _scene.getCamera(), dF.getLightInfo());
	setupRI(rI);

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