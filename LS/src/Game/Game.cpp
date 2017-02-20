#include "Game/Game.h"



Game::Game(Setting &setting, EventManager &events)
	: _setting(setting), _event(events), _scene(), _resource(setting._renderSetting), _deferred(_resource.getQuad()),
	_factory(_scene, events, "Resources/"), _shadowMapShader("shadow_cube_map","shadow_cube_map","shadow_cube_map")
{
	Camera& cam = _scene.setCamera(setting);
	_deferred.setWindowSize((float)setting.Width(), (float)setting.Height(), cam);
	_modelMatrixLocation = _shadowMapShader.getUniform("model");
	_shadowMatricesLocation = _shadowMapShader.getUniform("shadowMatrices");
	_lightPosLocation = _shadowMapShader.getUniform("lightPos");
	_farPlaneLocation = _shadowMapShader.getUniform("far_plane");

	GLfloat aspect = 1.0f;
	GLfloat near = 0.0f;
	GLfloat far = 10.0f;
	_shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
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


	for (size_t i = 0; i < rI._pLightInfo.size(); i++)
	{
		_resource.getCubeMap(i).bindDraw();
		_shadowMapShader.bind();
		glUniformMatrix4fv(_shadowMatricesLocation, 6, GL_FALSE, &rI._pLightInfo[i]._shadowMatrices[0][0][0]);
		glUniform3fv(_lightPosLocation, 1, &rI._pLightInfo[i]._pos[0]);
		glUniform1f(_farPlaneLocation, rI._pLightInfo[i]._fadeDist);
		dF.renderMeshOnly(rI, _modelMatrixLocation);
		gl::CheckGLErrors("Render stage failed: CubeMap");
	}

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
