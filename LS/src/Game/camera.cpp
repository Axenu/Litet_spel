#include"Game/Camera.h"
//test
#include <iostream>

Camera::Camera() {

}

Camera::Camera(float fov, float aspect, float near, float far) {
    _fieldOfView = fov;
    _aspectRatio = aspect;
    _nearPlane = near;
    _farPlane = far;
    _perspective = true;
    _position = glm::vec3(0,0,0);
    // _rotation.x = 1.0;
    initProjection();
}

Camera::Camera(float fov, int width, int height, float near, float far) {
    _perspective = true;
    _width = (float)width;
    _height = (float)height;
    _nearPlane = near;
    _farPlane = far;
    _fieldOfView = fov;
    _aspectRatio = _width/_height;
	_position = glm::vec3(0.0f, 0.0f, 0.0f);
	_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    // _rotation.x = 1.0;
    initProjection();
}

Camera::Camera(const Setting &setting)
	: Camera(setting._fov, setting.Width(), setting.Height(), setting._nearPlane, setting._farPlane) {
}

void Camera::update(float dT) {
	Node::update(dT);
	glm::mat4 viewTransform(glm::normalize(_modelMatrix[0]), glm::normalize(_modelMatrix[1]), glm::normalize(_modelMatrix[2]), _modelMatrix[3]);
	_viewMatrix = glm::inverse(viewTransform);

	//_viewMatrix = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0, 0, 1));
    VPMatrix = _projectionMatrix * _viewMatrix;
}

void Camera::initProjection()
{
	_projectionMatrix = glm::perspective(degreesToRadians(_fieldOfView), _aspectRatio, _nearPlane, _farPlane);
}

const glm::mat4 &Camera::getViewMatrix() const
{
    return _viewMatrix;
}

float Camera::getFieldOfView() const
{
    return _fieldOfView;
}

float Camera::getAspectRatio() const
{
    return _aspectRatio;
}
float Camera::getNearPlane() const
{
	return _nearPlane;
}
float Camera::getFarPlane() const
{
	return _farPlane;
}

glm::vec4 Camera::getLookAt()
{
	glm::mat4 invViewMat = glm::inverse(_viewMatrix);
	return invViewMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
}


Camera::~Camera()
{

}

float Camera::degreesToRadians(float degrees)
{
	return (float)(degrees * (M_PI / 180.0));
}
