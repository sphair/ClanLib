
#pragma once

#include "3D/camera.h"

class Skybox
{
public:
	Skybox(CL_GraphicContext &gc);
	~Skybox();

	void render(CL_GraphicContext &gc, const Camera &c);
	CL_Texture &get_skybox_texture() { return skybox_texture; }

private:
	CL_ProgramObject program_object;
	CL_Texture skybox_texture;

	static CL_Vec3f positions[6*4];
};
