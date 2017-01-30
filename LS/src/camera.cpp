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
    this->position = glm::vec3(3.0f, 0.0f , 5.0f);
	this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    // this->rotation.x = 1.0;
    initProjection();
}

void Camera::update(float dT) {
	Node::update(dT);
	_viewMatrix = glm::inverse(this->modelMatrix);
	
	//_viewMatrix = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0, 0, 1));
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