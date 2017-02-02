#ifndef __First__FACamera__
#define __First__FACamera__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "node.h"

class Camera : public Node
{

private:
	float _fieldOfView;
	float _aspectRatio;
	float _nearPlane;
	float _farPlane;
	float _width;
	float _height;
	bool _perspective;

	glm::mat4 _viewMatrix;
	glm::mat4 _projectionMatrix;

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

	virtual ~Camera();

	float degreesToRadians(float degrees);
};

#endif
