#ifndef __First__FACamera__
#define __First__FACamera__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "node.h"

class camera : public Node {

private:
	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	float width;
	float height;
	bool perspective;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

public:

	glm::mat4 VPMatrix;
	
	camera();
	camera(float fov, float aspect, float near, float far);
	camera(float fov, int _width, int _height, float near, float far);

	void initProjection();
	void useView();

	glm::mat4 &getViewMatrix();
	float getFieldOfView();
	float getAspectRatio();
	glm::vec3 &getPosition();
	
	~camera();

	float degreesToRadians(float degrees);
	
};

#endif