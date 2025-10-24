#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <GL/gl.h>
#include <GL/glext.h>

CL_OpenGLSurface *surface7_1;

PFNGLMULTITEXCOORD2FARBPROC test7_glMultiTexCoord2fARB;
PFNGLACTIVETEXTUREARBPROC test7_glActiveTextureARB;

void init_test7()
{
	test7_glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)CL_OpenGL::get_proc_address("glMultiTexCoord2fARB");
	test7_glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)CL_OpenGL::get_proc_address("glActiveTextureARB");

	test7_glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	surface7_1 = new CL_OpenGLSurface(new CL_TargaProvider("Images/test32_alpha.tga"), true);
	surface7_1->bind();
}

void deinit_test7()
{
	delete surface7_1;
}

void test7()
{
	int texture = 0;
	glBegin(GL_TRIANGLE_STRIP);
	int j=0;
	for(int i=0;i<1000;i++)
	{
//		test7_glMultiTexCoord2fARB(GL_TEXTURE0_ARB + texture, 0.0f, 0.0f);
//		test7_glMultiTexCoord2fARB(GL_TEXTURE0_ARB + texture, 1.0f, 0.0f);
//		test7_glMultiTexCoord2fARB(GL_TEXTURE0_ARB + texture, 0.0f, 1.0f);
//		test7_glMultiTexCoord2fARB(GL_TEXTURE0_ARB + texture, 1.0f, 1.0f);

		if (j==0)
		{
			test7_glMultiTexCoord2fARB(GL_TEXTURE0_ARB + texture, 0.0f, 0.0f);
			glVertex3d(0.0, 0.0, 0.0);
			j=1;
		}
		else if (j==1)
		{
			test7_glMultiTexCoord2fARB(GL_TEXTURE0_ARB + texture, 1.0f, 1.0f);
			glVertex3d(0.1, 0.1, 0.1);
			j=0;
		}

	}
	glEnd();
	glFlush();
}
