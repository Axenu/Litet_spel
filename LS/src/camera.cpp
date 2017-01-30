#include"Camera.h"

Camera::Camera() {

}

Camera::Camera(float fov, float aspect, float near, float far) {
    _fieldOfView = fov;
    _aspectRatio = aspect;
    _nearPlane = near;
    _farPlane = far;
    _perspective = true;
    position = glm::vec3(0,1,1);
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
    position = glm::vec3(3.0f, 0.0f , 5.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    // _rotation.x = 1.0;
    initProjection();
}

void Camera::update(float dT) {
	Node::update(dT);
	_viewMatrix = glm::inverse(modelMatrix);

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

const glm::vec3 &Camera::getPosition() const
{
    return position;
}

Camera::~Camera()
{

}

float Camera::degreesToRadians(float degrees)
{
	return (float)(degrees * (M_PI / 180.0));
}
