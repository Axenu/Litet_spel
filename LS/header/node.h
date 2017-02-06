#ifndef __First__FANode__
#define __First__FANode__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include <GLFW/glfw3.h>
#include "gl/glInclude.h"
#include <vector>

#ifndef M_PI
#define M_PI 3.1415926536
#endif
#ifndef M_PIf
#define M_PIf 3.1415926536f
#endif
#ifndef M_2PI
#define M_2PI 6.2831853072
#endif
#ifndef M_2PIf
#define M_2PIf 6.2831853072f
#endif

class Node {
public:
	Node();
	Node(const glm::vec3 &position);
	Node(const glm::vec3 &position, Node *parent);

	void addChild(Node *child);
	std::vector<Node *> getAllChildren();
    void setParent(Node *parent);
	Node* getParent();
	/* Removes the object from the parent
	*/
    void removeFromParent();
	/* Remove the node from the tree. Moving children to it's current parent
	*/
	void removeNode();
    void removeChild(Node *Node);

    virtual void update(float dt);
    virtual void render(){}

	void setX(float x);
    void setY(float y);
    void setZ(float z);
    void setPosition(float x, float y, float z);
    void setPosition(glm::vec3 pos);
    void moveX(float x);
    void moveY(float y);
    void moveZ(float z);
    void move(glm::vec3 p);
    void setScale(float p);
    void setScale(glm::vec3 p);
    void setScale(float x, float y);
    void setRX(float rx);
    void setRY(float ry);
    void setRZ(float rz);
    void rotateX(float f);
    void rotateY(float f);
    void rotateZ(float f);
    void rotate(glm::vec3 r);

	float getX();
	float getY();
	float getZ();
    float getRX();
    float getRY();
    float getRZ();
	glm::vec3 getScale() const;
    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;


	glm::vec4 getWorldPos() const;
	float getDistance(Node const &other) const;
	float getDistance(glm::vec4 const &pos) const;

	virtual ~Node();



private:
	bool _isActive;
protected:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
    glm::mat4 _modelMatrix;
    std::vector<Node *> _children;
    Node *_parent = nullptr;

    virtual void onUpdate(float dt){}
    virtual void onRender(){}
};


#endif
