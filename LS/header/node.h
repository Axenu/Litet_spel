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
	Node(const glm::vec3 &position, Node *parent = nullptr);

	/* Add a child to the node, sets the child's parent to this. */
	void addChild(Node *child);
	/* Get a list over all child nodes of this, including this. */
	std::vector<Node *> getAllChildren();
    void setParent(Node *parent);
	/* Get the reference to the parent */
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
	virtual void init();

#pragma region Get/Set Orientation vars

	void setPositionX(float x);
    void setPositionY(float y);
    void setPositionZ(float z);
    void setPosition(float x, float y, float z);
    void setPosition(glm::vec3 pos);
    void moveX(float x);
    void moveY(float y);
    void moveZ(float z);
    void move(glm::vec3 p);
    void setScale(float p);
    void setScale(glm::vec3 p);
    void setScale(float x, float y);
	void setRotEuler(glm::vec3 xyz);
    void rotateX(float f);
    void rotateY(float f);
    void rotateZ(float f);
    void rotate(glm::vec3 r);
	/* Faces the point, assumes (0, 1, 0) is up*/
	void face(glm::vec3 point);
	/* Sets forward axis, assumes (0, 1, 0) is up*/
	void setForward(glm::vec3 axis);

	float getX();
	float getY();
	float getZ();
	glm::vec3 getScale() const;

	/* Get the transformation matrix */
	const glm::mat4& getModelMatrix();

	glm::vec4 getWorldPos() const;
	/* Get forward in world space
	*/
	glm::vec3 getForward() const;
	glm::vec3 getRight() const;
	glm::vec3 getUp() const;
	/* Get local forward vector
	*/
	glm::vec3 getLocalForward() const;
	glm::vec3 getLocalUp() const;
	glm::vec3 getLocalRight() const;
    glm::vec3 getPosition() const;
	float getDistance(Node const &other) const;
	float getDistance(glm::vec4 const &pos) const;

#pragma endregion

	virtual ~Node();

private:
	bool _isActive;
	/* Our forward and up local orientation vectors */
	glm::vec3 _forward, _up;

	void calcModelMatrix();
	void reOrthogonalize();
protected:
	glm::vec3 _position;
	glm::vec3 _scale;
    glm::mat4 _modelMatrix;
    std::vector<Node *> _children;
    Node *_parent = nullptr;

    virtual void onUpdate(float dt){}
    virtual void onRender(){}
};


#endif
