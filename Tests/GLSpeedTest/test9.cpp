#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <GL/gl.h>

CL_OpenGLSurface *surface9_1;
CL_OpenGLSurface *surface9_2;

void init_test9()
{
	glEnable(GL_TEXTURE_2D);
	surface9_1 = new CL_OpenGLSurface(new CL_TargaProvider("Images/test32_alpha.tga"), true);
	surface9_2 = new CL_OpenGLSurface(new CL_TargaProvider("Images/test32_alpha.tga"), true);
}

void deinit_test9()
{
	delete surface9_1;
	delete surface9_2;
}

void test9()
{
	surface9_1->bind();
	surface9_2->bind();
}
