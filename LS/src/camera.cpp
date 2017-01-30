#include"camera.h"

Camera::Camera() {

}

Camera::Camera(float fov, float aspect, float near, float far) {
    this->fieldOfView = fov;
    this->aspectRatio = aspect;
    this->nearPlane = near;
    this->farPlane = far;
    this->perspective = true;
    this->position = glm::vec3(0,1,1);
    // this->rotation.x = 1.0;
    initProjection();
}

Camera::Camera(float fov, int _width, int _height, float near, float far) {
    this->perspective = true;
    this->width = (float)_width;
    this->height = (float)_height;
    this->nearPlane = near;
    this->farPlane = far;
    this->fieldOfView = fov;
    this->aspectRatio = width/height;
    this->position = glm::vec3(0,0,-3);
    // this->rotation.x = 1.0;
    initProjection();
}

void Camera::useView() {
	_viewMatrix = glm::rotate(glm::mat4(), rotation.x, glm::vec3(1, 0, 0));
	_viewMatrix = glm::rotate(_viewMatrix, rotation.y, glm::vec3(0, 1, 0));
	_viewMatrix = glm::rotate(_viewMatrix, rotation.z, glm::vec3(0, 0, 1));
	_viewMatrix = glm::translate(_viewMatrix, -position);
    VPMatrix = projectionMatrix * _viewMatrix;
}

void Camera::initProjection() {
	projectionMatrix = glm::perspective(degreesToRadians(fieldOfView), aspectRatio, nearPlane, farPlane);
}

const glm::mat4 &Camera::getViewMatrix() const {
    return this->_viewMatrix;
}

float Camera::getFieldOfView() const {
    return this->fieldOfView;
}

float Camera::getAspectRatio() const {
    return this->aspectRatio;
}
float Camera::getNearPlane() const {
	return this->nearPlane;
}
float Camera::getFarPlane() const {
	return this->farPlane;
}

glm::vec3 &Camera::getPosition() {
    return this->position;
}

Camera::~Camera() {

}

float Camera::degreesToRadians(float degrees) {
	return (float)(degrees * (M_PI / 180.0));
}