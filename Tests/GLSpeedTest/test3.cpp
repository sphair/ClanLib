#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <GL/gl.h>

void init_test3()
{
}

void deinit_test3()
{
}

void test3()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
