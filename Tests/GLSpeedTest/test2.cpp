#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <GL/gl.h>

CL_OpenGLSurface *surface1;
CL_OpenGLSurface *surface2;

void init_test2()
{
	surface1 = new CL_OpenGLSurface(new CL_TargaProvider("Images/test32_alpha.tga"), true);
	surface2 = new CL_OpenGLSurface(new CL_TargaProvider("Images/test32_alpha.tga"), true);
}

void deinit_test2()
{
	delete surface1;
	delete surface2;
}

void test2()
{
	surface1->bind();
	surface1->bind();
}
