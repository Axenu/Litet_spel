#include "Game/Game.h"



Game::Game(Setting &setting, EventManager &events)
	: _setting(setting), _event(events), _scene(), _resource(setting._renderSetting), _deferred(_resource.getQuad()),
	_factory(events, "Resources/", "Resources/models/"), _shadowMapShader("shadow_cube_map","shadow_cube_map","shadow_cube_map")
{
	_modelMatrixLocation = _shadowMapShader.getUniform("model");
	_shadowMatricesLocation = _shadowMapShader.getUniform("shadowMatrices");
	_lightPosLocation = _shadowMapShader.getUniform("lightPos");
	_farPlaneLocation = _shadowMapShader.getUniform("far_plane");
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
		_shadowMapShader.bind();
		glUniformMatrix4fv(_shadowMatricesLocation, 6, GL_FALSE, &rI._pLightInfo[i]._shadowMatrices[0][0][0]);
		glUniform3fv(_lightPosLocation, 1, &rI._pLightInfo[i]._pos[0]);
		glUniform1f(_farPlaneLocation, rI._pLightInfo[i]._fadeDist);
		tempDF.renderMeshOnly(_modelMatrixLocation);
		gl::CheckGLErrors("Render stage failed: CubeMap");
	}
	glEnable(GL_CULL_FACE);

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
