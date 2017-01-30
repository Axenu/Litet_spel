#pragma once
#include "MeshShader.h"

class DeferredMeshShader
	: public MeshShader
{
private:

	GLint _mvp, _mv, _difCol, _specCol;

	void acquireUniforms();

public:
	DeferredMeshShader();
	~DeferredMeshShader();


	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(FrameData &fD);
};

