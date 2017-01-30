#ifndef __First__FACamera__
#define __First__FACamera__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "node.h"

class Camera : public Node {

private:
	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	float width;
	float height;
	bool perspective;

	glm::mat4 _viewMatrix;
	glm::mat4 projectionMatrix;

public:

	glm::mat4 VPMatrix;

	Camera();
	Camera(float fov, float aspect, float near, float far);
	Camera(float fov, int _width, int _height, float near, float far);

	void initProjection();
	virtual void update(float dT);

	const glm::mat4 &getViewMatrix() const;
	float getFieldOfView() const;
	float getAspectRatio() const;
	float getNearPlane() const;
	float getFarPlane() const;
	glm::vec3 &getPosition();

	~Camera();

	float degreesToRadians(float degrees);
};

#endif
