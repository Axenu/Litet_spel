#include "Node.h"
#include <iostream>
#define GLM_FORCE_RADIANS
#include<glm/gtx/euler_angles.hpp>


Node::Node() {
	_scale = glm::vec3(1,1,1);
    _position = glm::vec3(0,0,0);
    _rotation = glm::vec3(0,0,0);
    this->_isActive = true;
    this->_modelMatrix = glm::mat4();
}

Node::Node(const glm::vec3 &position)
	: _isActive(true), _position(position), _rotation(0.0f), _scale(1.0f), _modelMatrix(glm::translate(glm::mat4(), position)) {
}
Node::Node(const glm::vec3 &position, Node *parent)
	: _isActive(true), _position(position), _rotation(0.0f), _scale(1.0f), _modelMatrix(glm::translate(glm::mat4(), position)) {
	setParent(parent);
}
Node::~Node() {

}

void Node::calcModelMatrix()
{
	this->_modelMatrix = glm::scale(glm::mat4(), this->_scale);
	this->_modelMatrix = glm::yawPitchRoll(_rotation.x, _rotation.y, _rotation.z) * this->_modelMatrix;
	this->_modelMatrix[3] = glm::vec4(_position, 1.0f); 
	if (this->_parent != nullptr) {
		this->_modelMatrix = this->_parent->_modelMatrix * this->_modelMatrix;
	}
}


void Node::addChild(Node *child) {
	//Use set child to update child
	child->setParent(this);
}

std::vector<Node *> Node::getAllChildren() {
	std::vector<Node *> v;
	for (Node *child : _children) {
		for (Node *n : child->getAllChildren()) {
			v.push_back(n);
		}
	}
	v.push_back(this);
	return v;
}

void Node::setParent(Node *parent) {
	//Remove any old parent and set the new one
	this->removeFromParent();
	if (parent != nullptr) { //Verify there is a new parent
		this->_parent = parent;
		parent->_children.push_back(this);
	}
}
Node* Node::getParent() {
	return this->_parent;
}

void Node::removeFromParent() {
    if (this->_parent != nullptr) {
        this->_parent->removeChild(this);
    }
}
/* Remove the node from the tree. Moving children to it's current parent
*/
void Node::removeNode() {
	//Move children to the parent
	for (unsigned int i = 0; i < _children.size(); i++)
		_children[i]->setParent(_parent);
	removeFromParent();
}

void Node::removeChild(Node *node) {
    Node *n = nullptr;
    for (unsigned int i = 0; i < _children.size(); i++) {
        n = _children[i];
        if (n == node) {
            _children.erase(_children.begin()+i);
            return;
        }
    }
}

void Node::update(float dt) {
    if (!_isActive) return;
	onUpdate(dt);
	calcModelMatrix();
    for (Node *Node : _children)
        Node->update(dt);
}

void Node::init()
{
	calcModelMatrix();
}

void Node::setX(float x) {
    _position.x = x;
}

void Node::setY(float y) {
	_position.y = y;
}

void Node::setZ(float z) {
	_position.z = z;
}

void Node::setPosition(float x, float y, float z) {
	this->_position = glm::vec3(x, y, z);
}

void Node::setPosition(glm::vec3 pos) {
	_position = pos;
}

void Node::moveX(float x) {
	_position.x += x;
}

void Node::moveY(float y) {
	_position.y += y;
}

void Node::moveZ(float z) {
	_position.z += z;
}

void Node::move(glm::vec3 p) {
	_position += p;
}

void Node::setScale(float p) {
    _scale = glm::vec3(p,p,p);
}

void Node::setScale(glm::vec3 p) {
	_scale = p;
}

void Node::setScale(float x, float y) {
	_scale.x = x;
	_scale.y = y;
}

void Node::setRX(float rx) {
	_rotation.x = rx;
}

void Node::setRY(float ry) {
	_rotation.y = ry;
}

void Node::setRZ(float rz) {
	_rotation.z = rz;
}

void Node::rotateX(float f) {
	_rotation.x += f;
}

void Node::rotateY(float f) {
	_rotation.y += f;
}

void Node::rotateZ(float f) {
    _rotation.z += f;
}

void Node::rotate(glm::vec3 r) {
    _rotation.x += r.x;
	_rotation.y += r.y;
	_rotation.z += r.z;

    while (_rotation.x > M_2PI) _rotation.x -= M_2PIf;
    while (_rotation.y > M_2PI) _rotation.y -= M_2PIf;
    while (_rotation.z > M_2PI) _rotation.z -= M_2PIf;

    while (_rotation.x < 0) _rotation.x += M_2PIf;
    while (_rotation.y < 0) _rotation.y += M_2PIf;
    while (_rotation.z < 0) _rotation.z += M_2PIf;
}

float Node::getX()  {
    return _position.x;
}

float Node::getY()  {
    return _position.y;
}

float Node::getZ()  {
    return _position.z;
}

float Node::getRX()  {
    return _rotation.x;
}

float Node::getRY()  {
    return _rotation.y;
}

float Node::getRZ()  {
    return _rotation.z;
}

glm::vec3 Node::getScale() const {
    return _scale;
}

glm::vec3 Node::getPosition() const {
    return _position;
}

glm::vec3 Node::getRotation() const {
    return _rotation;
}

const glm::mat4& Node::getModelMatrix()
{
	return _modelMatrix;
}
glm::vec4 Node::getWorldPos() const
{
	return _modelMatrix[3];
}

float Node::getDistance(Node const & other) const
{
	return glm::length(getWorldPos() - other.getWorldPos());
}

float Node::getDistance(glm::vec4 const &pos) const
{
	return glm::length(getWorldPos() - pos);
}
