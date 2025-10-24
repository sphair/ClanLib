#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <GL/gl.h>

CL_OpenGLSurface *surface8_1;

void init_test8()
{
	glEnable(GL_TEXTURE_2D);
	surface8_1 = new CL_OpenGLSurface(new CL_TargaProvider("Images/test32_alpha.tga"), true);
	surface8_1->bind();
}

void deinit_test8()
{
	delete surface8_1;
}

void test8()
{
	glBegin(GL_TRIANGLE_STRIP);
	int j=0;
	for(int i=0;i<1000;i++)
	{
//		glTexCoord2f(0.0f, 0.0f);
//		glTexCoord2f(1.0f, 0.0f);
//		glTexCoord2f(1.0f, 1.0f);
//		glTexCoord2f(1.0f, 1.0f);

		if (j==0)
		{
			glTexCoord2f(0.0f, 0.0f);
			glVertex3d(0.0, 0.0, 0.0);
			j=1;
		}
		else if (j==1)
		{
			glTexCoord2f(1.0f, 1.0f);
			glVertex3d(0.1, 0.1, 0.1);
			j=0;
		}
	}
	glEnd();
	glFlush();
}
