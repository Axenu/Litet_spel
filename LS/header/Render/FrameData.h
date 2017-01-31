#pragma once

#include"GraphicsResource.h"
#include"camera.h"
#include<glm/mat4x4.hpp>

/* Forward declare
*/
class ModelPart;

/* Frame temporary render information.
 * Used to gather render data before sending it to the render functions.
 */
class FrameData {
private:

public:

	const GraphicsResource& _resource;
	glm::mat4 _V, _VP;
	glm::vec3 _eye;

	FrameData(const GraphicsResource& resource, const Camera &cam);
	~FrameData();
};
