#include "Game/Game.h"



Game::Game(Setting &setting, EventManager *eventManager)
	: _setting(setting), _scene(), _resource(setting._renderSetting), _deferred(_resource.getQuad()),
	_factory(eventManager, "Resources/", "Resources/models/"), _shadowShader(), _skinnedShadowShader()
{
	_eventManager = eventManager;
}


Game::~Game()
{
}



void Game::initiate() {

}


void Game::update(float dT) {
	_scene->update(dT);

}
void Game::draw() {

	DrawFrame dF;
	_scene->fetchDrawables(dF);
	glm::vec3 camPos = _scene->getCamera().getWorldPos();
	dF.cullLightsByDistance(camPos);
	RenderInfo rI(_resource, _scene->getCamera(), dF.getLightInfo());
	setupRI(rI);

	glDisable(GL_CULL_FACE);
	size_t numLights = rI._pLightInfo.size();
	for (size_t i = 0; i < numLights; i++)
	{
		DrawFrame tempDF;
		float fadeDist = rI._pLightInfo[i]._fadeDist;
		AABB lightAABB(glm::vec3(-fadeDist) + rI._pLightInfo[i]._pos, glm::vec3(fadeDist) + rI._pLightInfo[i]._pos);
		_scene->fetchDrawables(tempDF, lightAABB);
		_resource.getCubeMap(i).bindDraw();

		//Render meshes
		_shadowShader.setUp(rI._pLightInfo[i]);
		tempDF.renderNonAnimatedMeshes(rI, &_shadowShader);
		//Render skinned meshes
		_skinnedShadowShader.setUp(rI._pLightInfo[i]);
		tempDF.renderAnimatedMeshes(rI, &_skinnedShadowShader);
#ifdef DEBUG
		gl::CheckGLErrors("Render stage failed: CubeMap");
#endif

	}
	glEnable(GL_CULL_FACE);

	_resource.getDeffered().bindDraw();
	dF.render(rI);
#ifdef DEBUG
	gl::CheckGLErrors("Render stage failed: Mesh");
#endif
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
#ifdef DEBUG
	gl::CheckGLErrors("Render stage failed: Composition");
#endif

}

Scene &Game::getScene()
{
	return *_scene;
}
