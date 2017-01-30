#include "Node.h"
#include <iostream>
#include<glm/gtx/euler_angles.hpp>


Node::Node() {
	scale = glm::vec3(1,1,1);
    position = glm::vec3(0,0,0);
    rotation = glm::vec3(0,0,0);
    this->isActive = true;
    this->modelMatrix = glm::mat4();
}

void Node::addChild(Node *child) {
	children.push_back(child);
    child->setParent(this);
}

std::vector<Node *> Node::getAllChildren() {
	std::vector<Node *> v;
	for (Node *child : children) {
		for (Node *n : child->getAllChildren()) {
			v.push_back(n);
		}
	}
	v.push_back(this);
	return v;
}

void Node::setParent(Node *parent) {
    this->parent = parent;
}

void Node::removeFromParent() {
    if (this->parent != nullptr) {
        this->parent->removeChild(this);
    }
}

void Node::removeChild(Node *node) {
    Node *n = nullptr;
    for (unsigned int i = 0; i < children.size(); i++) {
        n = children[i];
        if (n == node) {
            delete n;
            children.erase(children.begin()+i);
            return;
        }
    }
}

void Node::update(float dt) {
    if (!isActive) return;
    this->modelMatrix = glm::scale(glm::mat4(), this->scale);
	this->modelMatrix = glm::yawPitchRoll(rotation.x, rotation.y, rotation.z) * this->modelMatrix;
	this->modelMatrix[3] = glm::vec4(position, 1.0f); //Translate / Move
    if (this->parent != nullptr) {
        this->modelMatrix = this->parent->modelMatrix * this->modelMatrix;
    }
    onUpdate(dt);
    for (Node *Node : children)
        Node->update(dt);
}

void Node::setX(float x) {
    position.x = x;
}

void Node::setY(float y) {
    position.y = y;
}

void Node::setZ(float z) {
    position.z = z;
}

void Node::setPosition(float x, float y, float z) {
	this->position = glm::vec3(x, y, z);
}

void Node::setPosition(glm::vec3 pos) {
    position = pos;
}

void Node::moveX(float x) {
    position.x += x;
}

void Node::moveY(float y) {
    position.y += y;
}

void Node::moveZ(float z) {
    position.z += z;
}

void Node::move(glm::vec3 p) {
    position += p;
}

void Node::setScale(float p) {
    scale = glm::vec3(p,p,p);
}

void Node::setScale(glm::vec3 p) {
    scale = p;
}

void Node::setScale(float x, float y) {
    scale.x = x;
    scale.y = y;
}

void Node::setRX(float rx) {
    rotation.x = rx;
}

void Node::setRY(float ry) {
    rotation.y = ry;
}

void Node::setRZ(float rz) {
    rotation.z = rz;
}

void Node::rotateX(float f) {
    rotation.x += f;
}

void Node::rotateY(float f) {
    rotation.y += f;
}

void Node::rotateZ(float f) {
    rotation.z += f;
}

void Node::rotate(glm::vec3 r) {
    rotation.x += r.x;
    rotation.y += r.y;
    rotation.z += r.z;

    while (rotation.x > M_2PI) rotation.x -= M_2PIf;
    while (rotation.y > M_2PI) rotation.y -= M_2PIf;
    while (rotation.z > M_2PI) rotation.z -= M_2PIf;

    while (rotation.x < 0) rotation.x += M_2PIf;
    while (rotation.y < 0) rotation.y += M_2PIf;
    while (rotation.z < 0) rotation.z += M_2PIf;
}

float Node::getX()  {
    return position.x;
}

float Node::getY()  {
    return position.y;
}

float Node::getZ()  {
    return position.z;
}

float Node::getRX()  {
    return rotation.x;
}

float Node::getRY()  {
    return rotation.y;
}

float Node::getRZ()  {
    return rotation.z;
}

glm::vec3 Node::getScale()  {
    return scale;
}

glm::vec3 Node::getPosition()  {
    return position;
}

glm::vec3 Node::getRotation()  {
    return rotation;
}

Node::~Node() {
	
}