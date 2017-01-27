#include"camera.h"

camera::camera() {

}

camera::camera(float fov, float aspect, float near, float far) {
    this->fieldOfView = fov;
    this->aspectRatio = aspect;
    this->nearPlane = near;
    this->farPlane = far;
    this->perspective = true;
    this->position = glm::vec3(0,1,1);
    // this->rotation.x = 1.0;
    initProjection();
}

camera::camera(float fov, int _width, int _height, float near, float far) {
    this->perspective = true;
    this->width = _width;
    this->height = _height;
    this->nearPlane = near;
    this->farPlane = far;
    this->fieldOfView = fov;
    this->aspectRatio = width/height;
    this->position = glm::vec3(0,0,3);
    // this->rotation.x = 1.0;
    initProjection();
}

void camera::useView() {
    viewMatrix = glm::rotate(glm::mat4(), rotation.x, glm::vec3(1, 0, 0));
    viewMatrix = glm::rotate(viewMatrix, rotation.y, glm::vec3(0, 1, 0));
    viewMatrix = glm::rotate(viewMatrix, rotation.z, glm::vec3(0, 0, 1));
    viewMatrix = glm::translate(viewMatrix, -position);
    VPMatrix = projectionMatrix * viewMatrix;
}

void camera::initProjection() {
	projectionMatrix = glm::perspective(degreesToRadians(fieldOfView), aspectRatio, nearPlane, farPlane);
}

glm::mat4 &camera::getViewMatrix() {
    return this->viewMatrix;
}

float camera::getFieldOfView() {
    return this->fieldOfView;
}

float camera::getAspectRatio() {
    return this->aspectRatio;
}

glm::vec3 &camera::getPosition() {
    return this->position;
}
	
camera::~camera() {

}

float camera::degreesToRadians(float degrees) {
    return degrees * (M_PI / 180.0);
}