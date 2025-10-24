//------------------------------------------------------
//
// FOG By Johan 'Mace' GLysing		-		mace-x@usa.net
//
// Converted to ClanLib by Kenneth Gangstoe
//
//------------------------------------------------------

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class FogApplication : public CL_ClanApplication
{
public:
	virtual int main(int argc, char** argv);

private:
	void init();
	void init_gl();
	void init_lightpos();
	void init_fog();
	void init_textures();
	void init_display_lists();

	void run();
	void draw();
	void set_lights();
	void check_timers();

	void on_window_close();

private:
	CL_ResourceManager *manager;

	CL_OpenGLState *glstate;

	CL_OpenGLSurface *texture[3];
	bool quit;
};
