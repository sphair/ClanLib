#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <GL/gl.h>
#include <GL/glext.h>

CL_OpenGLSurface *surface10_1;
CL_OpenGLSurface *surface10_2;

PFNGLMULTITEXCOORD2FARBPROC test10_glMultiTexCoord2fARB;
PFNGLACTIVETEXTUREARBPROC test10_glActiveTextureARB;

void init_test10()
{
	test10_glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)CL_OpenGL::get_proc_address("glMultiTexCoord2fARB");
	test10_glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)CL_OpenGL::get_proc_address("glActiveTextureARB");

	glEnable(GL_TEXTURE_2D);
	surface10_1 = new CL_OpenGLSurface(new CL_TargaProvider("Images/test32_alpha.tga"), true);
	surface10_2 = new CL_OpenGLSurface(new CL_TargaProvider("Images/test32_alpha.tga"), true);

	test10_glActiveTextureARB(GL_TEXTURE0_ARB);
	surface10_1->bind();
	test10_glActiveTextureARB(GL_TEXTURE1_ARB);
	surface10_2->bind();
}

void deinit_test10()
{
	delete surface10_1;
	delete surface10_2;
}

void test10()
{
	glDisable(GL_TEXTURE_2D);
	test10_glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_2D);
	test10_glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
}
