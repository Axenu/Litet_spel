#pragma once

#include "GraphicsResource.h"
#include "Game/camera.h"
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include "Render/PointLight.h"

/* Forward Declare
*/

/* Frame temporary render information.
 * Used to gather render data before sending it to the render functions.
 */
class RenderInfo {
private:

public:
	/* Major vars
	*/

	const GraphicsResource& _resource;
	glm::mat4 _V, _VP;
	glm::vec3 _eye;
	std::vector<PointLight> _pLightInfo;

	/* Minor
	*/

	glm::vec3 _lightGrenadePos;

	RenderInfo(const GraphicsResource& resource, const Camera &cam, std::vector<PointLight> &pLights);
	~RenderInfo();
};
