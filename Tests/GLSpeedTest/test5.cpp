#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <GL/gl.h>

void init_test5()
{
	glDisable(GL_TEXTURE_2D);
}

void deinit_test5()
{
}

void test5()
{
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(10, 0);
	glVertex2f(10, 10);
	glVertex2f(0, 10);
	glEnd();
}
