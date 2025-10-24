#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <GL/gl.h>

void init_test6()
{
	glDisable(GL_TEXTURE_2D);
}

void deinit_test6()
{
}

void test6()
{
	glRectf(0,0,10,10);
}
