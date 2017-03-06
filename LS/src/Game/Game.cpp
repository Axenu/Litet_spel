#include "Game/Game.h"



Game::Game(Setting &setting, EventManager *eventManager)
	: _setting(setting), _resource(setting._renderSetting), _deferred(_resource.getQuad()), _scene(),
	_factory(eventManager, "Resources/", "Resources/models/"), _shadowShader(), _skinnedShadowShader()
{
	_eventManager = eventManager;
}


Game::~Game()
{
}



void Game::initiate() {
	_scene = spawnScene();
	_sceneManager = std::unique_ptr<SceneEventManager>(new SceneEventManager(*_eventManager, _factory, *_scene));
}


void Game::update(float dT) {
	_scene->update(dT);

}
void Game::draw(float dT) {
	DrawFrame dF;
	_scene->fetchDrawables(dF);
	glm::vec3 camPos = _scene->getCamera().getWorldPos();
	dF.cullLightsByDistance(camPos);
	RenderInfo rI(_resource, _scene->getCamera(), dF.getLightInfo(), dF.getGrenadeInfo());
	dF.updateVisuals(dT);

	glDisable(GL_CULL_FACE);
	size_t numLights = rI._pLightInfo.size();
	for (size_t i = 0; i < numLights; i++)
	{
		DrawFrame tempDF;
		AABB lightAABB = rI._pLightInfo[i].generateAABB();
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
	//Cleanup objects to remove
	_sceneManager->endFrame();
}

Scene &Game::getScene()
{
	return *_scene;
}
