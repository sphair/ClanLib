#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>

// #include "C:\Program Files\Rational\Quantify\pure.h"

/////////////////////////////////////////////////////////////////////////////
// Application class

class Application : public CL_ClanApplication
{
public:
	int main(int argc, char** argv);

	void layered_test();
	void stencil_test();
	void tesselate_test();
	void viewport_test();

	void resolution_test(CL_DisplayWindow &window);
	void opengl_test(CL_OpenGLWindow &window, CL_OpenGLWindow &window2);
	void gc_test(CL_DisplayWindow &window);
	void gradient_test(CL_DisplayWindow &window);
	void texture_test(CL_DisplayWindow &window);
	void surface_test(CL_DisplayWindow &window);
	void alpha_test(CL_DisplayWindow &window);
	void sprite_cutter_detail_test(CL_DisplayWindow &window);
	void sysfont_cutter_test(CL_DisplayWindow &window);
	void surface_size_test(CL_DisplayWindow &window);
	void font_test(CL_DisplayWindow &window);
	void sprite_gpmfuchs_test(CL_DisplayWindow &window);
	void surfaceandfont_test(CL_DisplayWindow &window);
	void joystick_test(CL_DisplayWindow &window);
	void surface_gc_test(CL_DisplayWindow &window);
	
	void onKeyDown(const CL_InputEvent &key);
	
	bool quit;
} app;

/////////////////////////////////////////////////////////////////////////////
// Application main entry point:

void log_to_cout(const std::string &channel, int level, const std::string &message)
{
	std::cout << "[" << channel.c_str() << "] " << message.c_str() << std::endl;
}

int Application::main(int argc, char** argv)
{
//	QuantifyStopRecordingData();
	
	CL_ConsoleWindow debug_console("Debug console");
	debug_console.redirect_stdio();

	CL_Slot slot_log = CL_Log::sig_log().connect(&log_to_cout);
	
	try
	{
		CL_SetupCore setup_init;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;

//		layered_test();
//		return 0;

//		stencil_test();
//		return 0;

//		tesselate_test();
//		viewport_test();
	
		CL_DisplayWindow window("ClanLib clanDisplay 2.0 test program", 320, 200, false, true);
		// CL_DisplayWindow window2("ClanLib clanDisplay 2.0 test program", 640, 480);
		// window.set_position(0, 0);
		// window2.set_position(640, 480);

		quit = false;
		CL_Slot slot = CL_Keyboard::sig_key_down().connect(this, &Application::onKeyDown);
		
		// opengl_test(window, window2);
		// resolution_test(window);
//		gradient_test(window);
		gc_test(window);
//		texture_test(window);
//		surface_test(window);|
//		alpha_test(window);
//		surface_size_test(window);
//		font_test(window);
//		sprite_cutter_detail_test(window);
//		sprite_gpmfuchs_test(window);
//		surfaceandfont_test(window);
//		joystick_test(window);
//		surface_gc_test(window);
	}
	catch (CL_Error error)
	{
		std::cout << "Uncaught error: " << error.message.c_str() << std::endl;
	}
	debug_console.display_close_message();

	return 0;
}

void Application::onKeyDown(const CL_InputEvent &key)
{
	if(key.id == CL_KEY_ESCAPE)
		quit = true;
}

void Application::layered_test()
{
#ifdef WIN32
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib Layered Test Window");
	desc.set_position(CL_Rect(100, 100, 260, 200));
	desc.set_layered(true);

	CL_DisplayWindow window(desc);

	quit = false;
	CL_Slot slot = CL_Keyboard::sig_key_down().connect(this, &Application::onKeyDown);

	GLfloat	rtri = 0.0f;			// Angle For The Triangle
	GLfloat	rquad = 0.0f;			// Angle For The Quad	
	int width = desc.get_size().width;
	int height = desc.get_size().height;
	int x = 100;
	int y = 100;

	CL_OpenGLState gl_state(window.get_gc());
	gl_state.set_active();

	while (!quit)
	{
		x = x + (CL_Mouse::get_x() - width/2)/10;
		y = y + (CL_Mouse::get_y() - height/2)/10;
		window.set_position(x, y);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// This Will Clear The Background Color To Black
		glClearDepth(1.0);						// Enables Clearing Of The Depth Buffer
		glDepthFunc(GL_LESS);					// The Type Of Depth Test To Do
		glEnable(GL_DEPTH_TEST);				// Enables Depth Testing
		glShadeModel(GL_SMOOTH);				// Enables Smooth Color Shading

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();						// Reset The Projection Matrix

		gluPerspective(45.0f,(GLfloat) width/(GLfloat)height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

		glMatrixMode(GL_MODELVIEW);
		
		glViewport(0, 0, width, height);		// Reset The Current Viewport And Perspective Transformation

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
		glMatrixMode(GL_MODELVIEW);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
		glLoadIdentity();						// Reset The View
		glTranslatef(-1.5f,0.0f,-6.0f);			// Move Into The Screen And Left

		glRotatef(rtri,0.0f,1.0f,0.0f);			// Rotate The Triangle On The Y Axis

		glBegin(GL_POLYGON);					// Start Drawing A Polygon
		glColor4f(1.0f,0.0f,0.0f,1.0f);			// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Front)
		glColor4f(0.0f,1.0f,0.0f,1.0f);			// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Left Of Triangle (Front)
		glColor4f(0.0f,0.0f,1.0f,1.0f);			// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Right Of Triangle (Front)

		glColor3f(1.0f,0.0f,0.0f);				// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Right)
		glColor3f(0.0f,0.0f,1.0f);				// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Left Of Triangle (Right)
		glColor3f(0.0f,1.0f,0.0f);				// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);			// Right Of Triangle (Right)

		glColor3f(1.0f,0.0f,0.0f);				// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Back)
		glColor3f(0.0f,1.0f,0.0f);				// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);			// Left Of Triangle (Back)
		glColor3f(0.0f,0.0f,1.0f);				// Blue
		glVertex3f(-1.0f,-1.0f, -1.0f);			// Right Of Triangle (Back)

		glColor3f(1.0f,0.0f,0.0f);				// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Left)
		glColor3f(0.0f,0.0f,1.0f);				// Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Left Of Triangle (Left)
		glColor3f(0.0f,1.0f,0.0f);				// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Right Of Triangle (Left)
		glEnd();								// Done Drawing The Polygon

		glLoadIdentity();
		glTranslatef(1.5f,0.0f,-7.0f);			// Move Right 1.5 Units
		glRotatef(rquad,1.0f,1.0f,1.0f);		// Rotate The Quad On X,Y & Z


		glBegin(GL_QUADS);						// Start Drawing A Quad
		glColor3f(0.0f,1.0f,0.0f);				// Blue
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Bottom Left Of The Quad (Top)
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Bottom Right Of The Quad (Top)

		glColor3f(1.0f,0.5f,0.0f);				// Orange
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Top Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Top Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Bottom)

		glColor3f(1.0f,0.0f,0.0f);				// Red
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Front)
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Front)

		glColor3f(1.0f,1.0f,0.0f);				// Yellow
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Top Right Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Top Left Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Bottom Left Of The Quad (Back)
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Bottom Right Of The Quad (Back)

		glColor3f(0.0f,0.0f,1.0f);				// Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Left)

		glColor3f(1.0f,0.0f,1.0f);				// Violet
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Right)
		glEnd();								// Done Drawing The Quad
		
		rtri+=5.0f;								// Increase Rotation Variable For The Triangle
		rquad-=5.0f;							// Decrease Rotation Variable For The Quad

		window.flip();
		CL_System::keep_alive(10);

		gl_state.set_active();
	}
	
	quit = false;
#endif
}

void Application::resolution_test(CL_DisplayWindow &window)
{
/*
	CL_System::keep_alive(2000);
	window.set_fullscreen(640, 480, 16);
	CL_System::keep_alive(2000);
	window.set_windowed();
	CL_System::keep_alive(5000);
*/
}

void Application::opengl_test(CL_OpenGLWindow &window, CL_OpenGLWindow &window2)
{
	GLfloat	rtri = 0.0f;			// Angle For The Triangle
	GLfloat	rquad = 0.0f;			// Angle For The Quad	
	int width = 640;
	int height = 480;

	CL_OpenGLState gl_state_w1(window.get_gc());
	CL_OpenGLState gl_state_w2(window2.get_gc());

	for (int i=0; i<500; i++)
	{
		if (i%2) gl_state_w1.set_active();
		else gl_state_w2.set_active();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// This Will Clear The Background Color To Black
		glClearDepth(1.0);						// Enables Clearing Of The Depth Buffer
		glDepthFunc(GL_LESS);					// The Type Of Depth Test To Do
		glEnable(GL_DEPTH_TEST);				// Enables Depth Testing
		glShadeModel(GL_SMOOTH);				// Enables Smooth Color Shading

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();						// Reset The Projection Matrix

		gluPerspective(45.0f,(GLfloat) width/(GLfloat)height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

		glMatrixMode(GL_MODELVIEW);
		
		glViewport(0, 0, width, height);		// Reset The Current Viewport And Perspective Transformation

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
		glMatrixMode(GL_MODELVIEW);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
		glLoadIdentity();						// Reset The View
		glTranslatef(-1.5f,0.0f,-6.0f);			// Move Into The Screen And Left

		glRotatef(rtri,0.0f,1.0f,0.0f);			// Rotate The Triangle On The Y Axis

		glBegin(GL_POLYGON);					// Start Drawing A Polygon
		glColor3f(1.0f,0.0f,0.0f);				// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Front)
		glColor3f(0.0f,1.0f,0.0f);				// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Left Of Triangle (Front)
		glColor3f(0.0f,0.0f,1.0f);				// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Right Of Triangle (Front)

		glColor3f(1.0f,0.0f,0.0f);				// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Right)
		glColor3f(0.0f,0.0f,1.0f);				// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Left Of Triangle (Right)
		glColor3f(0.0f,1.0f,0.0f);				// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);			// Right Of Triangle (Right)

		glColor3f(1.0f,0.0f,0.0f);				// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Back)
		glColor3f(0.0f,1.0f,0.0f);				// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);			// Left Of Triangle (Back)
		glColor3f(0.0f,0.0f,1.0f);				// Blue
		glVertex3f(-1.0f,-1.0f, -1.0f);			// Right Of Triangle (Back)

		glColor3f(1.0f,0.0f,0.0f);				// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Left)
		glColor3f(0.0f,0.0f,1.0f);				// Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Left Of Triangle (Left)
		glColor3f(0.0f,1.0f,0.0f);				// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Right Of Triangle (Left)
		glEnd();								// Done Drawing The Polygon

		glLoadIdentity();
		glTranslatef(1.5f,0.0f,-7.0f);			// Move Right 1.5 Units
		glRotatef(rquad,1.0f,1.0f,1.0f);		// Rotate The Quad On X,Y & Z


		glBegin(GL_QUADS);						// Start Drawing A Quad
		glColor3f(0.0f,1.0f,0.0f);				// Blue
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Bottom Left Of The Quad (Top)
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Bottom Right Of The Quad (Top)

		glColor3f(1.0f,0.5f,0.0f);				// Orange
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Top Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Top Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Bottom)

		glColor3f(1.0f,0.0f,0.0f);				// Red
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Front)
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Front)

		glColor3f(1.0f,1.0f,0.0f);				// Yellow
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Top Right Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Top Left Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Bottom Left Of The Quad (Back)
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Bottom Right Of The Quad (Back)

		glColor3f(0.0f,0.0f,1.0f);				// Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Left)

		glColor3f(1.0f,0.0f,1.0f);				// Violet
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Right)
		glEnd();								// Done Drawing The Quad
		
		rtri+=5.0f;								// Increase Rotation Variable For The Triangle
		rquad-=5.0f;							// Decrease Rotation Variable For The Quad

		if (i%2) window.flip();
		if (i%2) window2.flip();
		CL_System::keep_alive(10);
	}
	
	quit = false;
}

void Application::gradient_test(CL_DisplayWindow &window)
{
	CL_GraphicContext *gc = window.get_gc();
	
	while (!quit)
	{
		gc->clear();
		gc->fill_rect(CL_Rect(10,10,100,100), CL_Gradient(CL_Color::white, CL_Color::green, CL_Color::red, CL_Color::blue));
		window.flip();
		CL_System::keep_alive(10);
	}

	quit = false;
}

void Application::texture_test(CL_DisplayWindow &window)
{
/*	CL_GraphicContext *gc = window.get_gc();

	CL_OpenGLSurface texture("images/test32_alpha.tga");

	std::cout << "Texture (mem) width: " << texture.get_texture_width() << std::endl;
	std::cout << "Texture (mem) height: " << texture.get_texture_height() << std::endl;
	std::cout << "Texture (screen) width: " << texture.get_width() << std::endl;
	std::cout << "Texture (screen) height: " << texture.get_height() << std::endl;
	
	while (!quit)
	{
		gc->clear();

		window.flip();
		CL_System::keep_alive(10);
	}

	quit = false;
*/
}

void Application::gc_test(CL_DisplayWindow &window)
{
	CL_GraphicContext *gc = window.get_gc();

	while (!quit)
	{
		gc->clear();

		gc->draw_rect(CL_Rect(0,0,gc->get_width(),gc->get_height()), CL_Color(255,255,255));

//		gc->draw_line(0, 0, 640, 480, CL_Color(255, 255, 255));
//		gc->draw_rect(CL_Rect(10, 10, 40, 40), CL_Color(0, 0, 255));
//		gc->fill_rect(CL_Rect(50, 50, 70, 70), CL_Color(255, 0, 0));

		window.flip();
		CL_System::keep_alive(10);
	}

	quit = false;
}

void Application::surface_test(CL_DisplayWindow &window)
{
/*	CL_Surface surface(new CL_TargaProvider("images/test32_alpha.tga"), true);
//	CL_Surface surface(new CL_PCXProvider("images/test8.pcx"), true);
//	CL_Surface surface(new CL_PNGProvider("images/test8.png"), true);
//	CL_Surface surface(new CL_JPEGProvider("images/test24.jpg"), true);

	float angle = 0.0f;
	float angle2 = 0.5f;

	CL_GraphicContext *gc = window.get_gc();

	while (!quit)
	{
		gc->fill_rect(CL_Rect(0, 0, 1024, 768), CL_Color(0,0,0));

		/////////////////////////////////////////////
		// Draw 4 fading, scaling and rotating surfaces in center of the screen

		angle += 0.5f;

		surface.set_angle(angle);
		surface.set_alpha(1.0f - ((360.0f - surface.get_angle()) / 360.0f));
		surface.set_scale(1.0f - ((360.0f - surface.get_angle()) / 360.0f), 1.0f - ((360.0f - surface.get_angle()) / 360.0f));

		surface.set_alignment(origin_top_left);
		surface.set_rotation_hotspot(origin_top_left);
		surface.draw(512, 384, gc);

		surface.set_alignment(origin_bottom_right);
		surface.set_rotation_hotspot(origin_bottom_right);
		surface.draw(512, 384, gc);

		surface.set_angle(-angle);
		surface.set_alpha(surface.get_angle() / 360.0f);
		surface.set_scale(surface.get_angle() / 360.0f, surface.get_angle() / 360.0f);

		surface.set_alignment(origin_top_right);
		surface.set_rotation_hotspot(origin_top_right);
		surface.draw(512, 384, gc);

		surface.set_alignment(origin_bottom_left);
		surface.set_rotation_hotspot(origin_bottom_left);
		surface.draw(512, 384, gc);

		/////////////////////////////////////////////
		// Draw 4 rotating surfaces in the corners

		angle2 -= 2.0f;

		surface.set_alpha(1.0f);
		surface.set_scale(0.3f, 0.3f);
		surface.set_rotation_hotspot(origin_center);

		surface.set_angle(-angle2);

		surface.set_alignment(origin_top_left);
		surface.draw(0, 0, gc); 

		surface.set_alignment(origin_bottom_right);
		surface.draw(1024, 768, gc); 

		surface.set_angle(angle2);

		surface.set_alignment(origin_top_right);
		surface.draw(1024, 0, gc); 

		surface.set_alignment(origin_bottom_left);
		surface.draw(0, 768, gc); 

		surface.draw(
			CL_Rect(
				CL_Point(100, 200),
				CL_Size(surface.get_pixelbuffer().get_width() / 2, surface.get_pixelbuffer().get_height() / 2)),
			gc);
		surface.draw(
			CL_Rect(
				CL_Point(0, 0),
				CL_Size(surface.get_pixelbuffer().get_width() / 2, surface.get_pixelbuffer().get_height() / 2)),
			CL_Rect(
				CL_Point(100, 300),
				CL_Size(surface.get_pixelbuffer().get_width(), surface.get_pixelbuffer().get_height())),
			gc);

		gc->draw_line(512, 0, 512, 768, CL_Color(255, 0, 0, 100));
		gc->draw_line(0, 384, 1024, 384, CL_Color(255, 0, 0, 100));

		window.flip();
		CL_System::keep_alive(10);

		GLenum error;
		while ((error = glGetError()) != GL_NO_ERROR)
			std::cout << gluErrorString(error) << std::endl;
	}

	quit = false;
*/
}

void Application::alpha_test(CL_DisplayWindow &window)
{
	// Set a cursor:
/*	CL_SpriteDescription desc;
	desc.add_frame(new CL_TargaProvider("images/manwalk_0001.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0002.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0003.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0004.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0005.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0006.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0007.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0008.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0009.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0010.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0011.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0012.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0013.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0014.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0015.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0016.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0017.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0018.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0019.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0020.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0021.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0022.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0023.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0024.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0025.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0026.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0027.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0028.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0029.tga"), true);

	CL_Sprite sprite(desc);
	sprite.set_alignment(origin_center);
	window.set_cursor(sprite);

	static int col = 0;
	static int col_delta = 1;

	std::list<CL_Surface> images;
	std::list<std::string> text;
	
	images.push_back(CL_Surface(new CL_PCXProvider("images/test8.pcx"), true));	text.push_back("test8.pcx");
	images.push_back(CL_Surface(new CL_PNGProvider("images/test8.png"), true));	text.push_back("test8.png");
	images.push_back(CL_Surface(new CL_TargaProvider("images/test8.tga"), true)); text.push_back("test8.tga");
	images.push_back(CL_Surface(new CL_PNGProvider("images/test8_alpha.png"), true)); text.push_back("test8_alpha.png");
	images.push_back(CL_Surface(new CL_TargaProvider("images/test16.tga"), true)); text.push_back("test16.tga");
	images.push_back(CL_Surface(new CL_JPEGProvider("images/test24.jpg"), true)); text.push_back("test24.jpg");
	images.push_back(CL_Surface(new CL_PCXProvider("images/test24.pcx"), true)); text.push_back("test24.pcx");
	images.push_back(CL_Surface(new CL_PNGProvider("images/test24.png"), true)); text.push_back("test24.png");
	images.push_back(CL_Surface(new CL_TargaProvider("images/test24.tga"), true)); text.push_back("images/test24.tga");
	images.push_back(CL_Surface(new CL_PNGProvider("images/test32.png"), true)); text.push_back("images/test32.png");
	images.push_back(CL_Surface(new CL_TargaProvider("images/test32.tga"), true)); text.push_back("images/test32.tga");
	images.push_back(CL_Surface(new CL_PNGProvider("images/test32_alpha.png"), true)); text.push_back("images/test32_alpha.png");
	images.push_back(CL_Surface(new CL_TargaProvider("images/test32_alpha.tga"), true)); text.push_back("images/test32_alpha.tga");
	
	CL_GraphicContext *gc = window.get_gc();

	CL_Font sysfont("Comic Sans MS", 15);
	sysfont.set_alignment(origin_center);

	while (!quit)
	{
		// Clear screen
		CL_Display::clear(CL_Color(col, col, 0));

		// Fade background between black and white
		col += col_delta;
		if(col > 255 || col < 0)
		{
			col_delta = -col_delta;
			col += col_delta;
		}

		gc->draw_line(200, 0, 200, 600, CL_Color(100,100,100));
		gc->draw_line(600, 0, 600, 600, CL_Color(100,100,100));

		gc->draw_line(0, 150, 800, 150, CL_Color(100,100,100));
		gc->draw_line(0, 450, 800, 450, CL_Color(100,100,100));

		gc->draw_line(400, 0, 400, 600, CL_Color(255,255,255));
		gc->draw_line(0, 300, 800, 300, CL_Color(255,255,255));

		int x = 0;
		int y = 0;
		std::list<CL_Surface>::iterator i1;
		std::list<std::string>::iterator i2;
		for(i1 = images.begin(), i2 = text.begin(); i1 != images.end(); ++i1, ++i2)
		{
			(*i1).set_scale(0.37f, 0.37f);
			(*i1).draw(x + 5, y + 5);
			sysfont.draw(x + 100, y + 140, (*i2));
			
			x += 200;
			if(x == 800)
			{
				x = 0;
				y += 150;
			}
		}
	
		window.flip();
		CL_System::keep_alive(10);
	}

	quit = false;

	window.set_cursor(CL_Sprite());
*/}

void Application::surface_size_test(CL_DisplayWindow &window)
{
/*	CL_Surface surface(new CL_TargaProvider("images/wacked.tga"), true);

	CL_GraphicContext *gc = window.get_gc();
	
	surface.set_rotation_hotspot(origin_bottom_right);

	while (!quit)
	{
		gc->fill_rect(CL_Rect(0, 0, 1024, 768), CL_Color(0,0,0));

		surface.rotate(1.0f);
		
		surface.draw(0, 0, gc);
		surface.draw(CL_Rect(0, 100, 300, 300), gc);
		
		window.flip();
		CL_System::keep_alive(10);
	}
	
	quit = false;
*/}

void Application::font_test(CL_DisplayWindow &window)
{
/*	CL_GraphicContext *gc = window.get_gc();

	CL_SpriteDescription desc;
	desc.add_paletteclipped_frames(
		new CL_PCXProvider("images/clanfont.pcx"), 129, 209, true);

	CL_Sprite sprite(desc);
	CL_Font bitfont(
		sprite,
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		10);
	bitfont.set_alignment(origin_center);
	bitfont.set_color(0.0f, 1.0f, 0.0f, 1.0f);

	CL_Font sysfont("Comic Sans MS", 141);
	sysfont.set_alignment(origin_center);
//	CL_Sprite glyphs = sysfont.get_glyphs();
	
	while (!quit)
	{
		gc->fill_rect(CL_Rect(0, 0, 1024, 768), CL_Color(0,0,0));

		// This grid is to test that coordinate system is sane:
		for (int y=0; y<600; y+=3)
		{
			gc->draw_line(0, y, 800, y, CL_Color(255,255,255));
		}
		for (int x=0; x<800; x+=3)
		{
			gc->draw_line(x, 0, x, 600, CL_Color(255,255,255));
		}
		
		bitfont.draw(window.get_width()/2, 100, "Bitmap font", gc);
		sysfont.draw(window.get_width()/2, 200, "System font", gc);

		window.flip();
		CL_System::keep_alive(10);
	}

	quit = false;
*/}

void Application::sprite_cutter_detail_test(CL_DisplayWindow &window)
{
/*	CL_SpriteDescription desc;
	
//	desc.add_frame(new CL_TargaProvider("images/frameman.tga"), true);
	desc.add_gridclipped_frames(
		new CL_TargaProvider("images/fire.tga"), 
		0, 0,
		29, 59,
		8, 1,
		1, 0,
		true);
		
	CL_Sprite sprite1(desc);
	
	CL_GraphicContext *gc = window.get_gc();

	float last_time = (float)CL_System::get_time();

	sprite1.set_rotation_hotspot(origin_center, 0, 70);
	
	float d = 0;
	while (!quit)
	{
		// Calculate amount of time since last frame
		float delta_time = (CL_System::get_time() - last_time) / 1000.0f;
		last_time = (float)CL_System::get_time();

		d += delta_time * 10;
		
		gc->fill_rect(CL_Rect(0, 0, 1024, 768), CL_Color(0,0,0));

		sprite1.update(delta_time);
		
		for(float x = 0; x < 360; x += 15)
		{
			sprite1.set_angle(x + d);
			sprite1.draw(400, 350, gc);
		}
		
		window.flip();
		CL_System::keep_alive(10);
	}

	quit = false;
*/}

void Application::sprite_gpmfuchs_test(CL_DisplayWindow &window)
{
/*	CL_SpriteDescription desc;
	
	desc.add_frame(new CL_TargaProvider("images/manwalk_0001.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0002.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0003.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0004.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0005.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0006.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0007.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0008.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0009.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0010.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0011.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0012.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0013.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0014.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0015.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0016.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0017.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0018.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0019.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0020.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0021.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0022.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0023.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0024.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0025.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0026.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0027.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0028.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0029.tga"), true);

	CL_Sprite sprite(desc);
	sprite.set_alignment(origin_center);

	CL_GraphicContext *gc = window.get_gc();
	float last_time = (float)CL_System::get_time();
	float cur_spr = 0;
	float time_elapsed = 0.0f;

	while (!quit)
	{
		float delta_time = (CL_System::get_time() - last_time) / 1000.0f;
		last_time = (float)CL_System::get_time();

		cur_spr += delta_time*25;
		while (cur_spr >= 29.0f) cur_spr -= 29.0f;
		time_elapsed += delta_time*5;

		gc->clear(CL_Color::white);

		for (int objnr = 0; objnr < 800; objnr++)
		{
			float angle = time_elapsed + objnr*20%360;
			sprite.set_angle((float) angle);
			sprite.set_frame((int) cur_spr);
			sprite.set_color(rand()%255 / 255.0f,rand()%255 / 255.0f,rand()%255 / 255.0f);
			int x = objnr*30;
			int y = x/1000*30;
			x %= 1000;
			sprite.draw(20+x, 20+y, gc);
		}

		window.flip();
		CL_System::keep_alive(1);
	}

	quit = false;
*/}

void Application::surfaceandfont_test(CL_DisplayWindow &window)
{
/*	CL_GraphicContext *gc = window.get_gc();

	CL_SpriteDescription font_desc;
	font_desc.add_paletteclipped_frames(
		new CL_PCXProvider("images/clanfont.pcx"), 129, 209, true);
	CL_Sprite font_sprite(font_desc);
	CL_Font font(
		font_sprite,
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		10);

	CL_Surface surface(new CL_TargaProvider("images/manwalk_0001.tga"), true);

	CL_SpriteDescription desc;
	desc.add_frame(new CL_TargaProvider("images/manwalk_0001.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0002.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0003.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0004.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0005.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0006.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0007.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0008.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0009.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0010.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0011.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0012.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0013.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0014.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0015.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0016.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0017.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0018.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0019.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0020.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0021.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0022.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0023.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0024.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0025.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0026.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0027.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0028.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0029.tga"), true);
	CL_Sprite sprite(desc);
	
	int width = CL_Display::get_width();
	int height = CL_Display::get_height();

	int start_time = CL_System::get_time();
	int fps_result = 0;
	int fps_counter = 0;
	int total_time = 0;

//	QuantifyStartRecordingData();

	while (!quit)
	{
		int cur_time = CL_System::get_time();
		int delta_time = cur_time - start_time;
		start_time = cur_time;

		total_time += delta_time;
		if(total_time >= 1000)// One second has passed
		{
			fps_result = fps_counter + 1;
			fps_counter = total_time = 0;
		}
		fps_counter++;// Increase fps
		
		gc->clear(CL_Color::black);

		for(int i=1; i<10000;i++)
		{
			// Surface test:
//			for(int j = 0; j < 10; j++)
//				surface.draw(width % i + j, height % i + j, gc);

			// Sprite test:
//			for(int j = 0; j < 10; j++)
//				sprite.draw(width % i + j, height % i + j, gc);

			// Font test:
			font.draw(width % i, height % i, "Bitmap font", gc);
		}

		font.draw(900, 0, CL_String::from_int(fps_result) + " fps");

		window.flip();
		CL_System::keep_alive();
	}

//	QuantifyStopRecordingData();

	quit = false;
*/}

void Application::joystick_test(CL_DisplayWindow &window)
{
	CL_GraphicContext *gc = window.get_gc();

	int width = CL_Display::get_width();
	int height = CL_Display::get_height();

	if (window.get_ic()->get_joystick_count() == 0)
	{
		std::cout << "No joysticks found!" << std::endl;
		return;
	}
	CL_InputDevice joystick = window.get_ic()->get_joystick();

	while (!quit)
	{
		gc->clear(CL_Color::black);

		double x_axis = joystick.get_x()/double(0xffff/2);
		double y_axis = joystick.get_y()/double(0xffff/2);

		int x = int(width/2+x_axis*width/2);
		int y = int(height/2+y_axis*height/2);
		CL_Rect rect(x-10, y-10, x+10, y+10);
		CL_Color color = CL_Color::white;
		if (joystick.get_keycode(0)) color = CL_Color::blue;
		if (joystick.get_keycode(1)) color = CL_Color::burlywood;
		if (joystick.get_keycode(2)) color = CL_Color::cornflowerblue;
		if (joystick.get_keycode(3)) color = CL_Color::gainsboro;
		if (joystick.get_keycode(4)) color = CL_Color::indianred;
		if (joystick.get_keycode(5)) color = CL_Color::lightskyblue;
		if (joystick.get_keycode(6)) color = CL_Color::lime;
		if (joystick.get_keycode(7)) color = CL_Color::orange;
		gc->fill_rect(rect, color);

		window.flip();
		CL_System::keep_alive();
	}

	quit = false;
}

void Application::surface_gc_test(CL_DisplayWindow &window)
{
/*	CL_GraphicContext *gc = window.get_gc();

	CL_Surface surface(new CL_TargaProvider("images/manwalk_0016.tga"), true);
	CL_Surface surface2(new CL_TargaProvider("images/manwalk_0002.tga"), true);
	CL_Surface surface3(new CL_TargaProvider("images/manwalk_0002.tga"), true);

	surface.set_color(1.0f, 1.0f, 0.25f);

	CL_Canvas canvas(surface3);
	surface.draw(0, 0, canvas.get_gc());
	canvas.get_gc()->fill_rect(CL_Rect(0,0,20,20), CL_Color::yellow);

	while (!quit)
	{
		gc->clear(CL_Color::black);
		surface2.draw(0, 0, gc);
		surface.draw(0, 0, gc);
		surface3.draw(64, 0, gc);
		window.flip();
		CL_System::keep_alive();
	}

	quit = false;
*/
}

void Application::stencil_test()
{
	CL_OpenGLWindowDescription desc;
	desc.set_title("ClanLib clanDisplay 2.0 test program");
	desc.set_size(CL_Size(800,600));
	desc.set_stencil_size(8);
	desc.set_fullscreen(false);

	CL_DisplayWindow window(desc);

	quit = false;
	CL_Slot slot = CL_Keyboard::sig_key_down().connect(this, &Application::onKeyDown);

	CL_Surface text("images/stencil_text.png");
	CL_Surface highlight("images/stencil_highlight.png");

	CL_GraphicContext *gc = window.get_gc();
	CL_OpenGLState gl_state(gc);
	gl_state.set_active();
	gl_state.setup_2d();
	
	glClearStencil(0);
	glStencilMask(1);
	glEnable(GL_STENCIL_TEST);

	// discard pixels with alpha < 0.5 (quad drawn otherwise)
	// when GL_ALPHA_TEST is enabled
	glAlphaFunc(GL_GEQUAL,0.5f); 

	int width = CL_Display::get_width();
	int height = CL_Display::get_height();
	
	float sin_count = 0;

	while (!quit)
	{
		sin_count += float(M_PI)/180.0f;

		gl_state.set_active();

		glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		glDisable(GL_STENCIL_TEST);

		int a=int(255.f*fabs(sin(sin_count)));
		int b=int(255.f*fabs(sin(sin_count+(0.25*M_PI))));
		int c=int(255.f*fabs(sin(sin_count+(0.5*M_PI))));

		CL_Display::fill_rect(CL_Rect(0,0,1024,768), 
			CL_Gradient( 
				CL_Color(a,b,c,a),
				CL_Color(b,c,a,b),
				CL_Color(c,b,a,c),
				CL_Color(c,a,b,a)));

		gl_state.set_active();
		glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR); // increment when test passed
		
		glColorMask(0,0,0,0);    // disable color buf writing.
		glEnable(GL_ALPHA_TEST);  
		text.draw(250,130);      // draw to stencil buf (color buf disabled)
		gl_state.set_active();
		glDisable(GL_ALPHA_TEST);
		glColorMask(1,1,1,1);    // enable color buf writing

		// ----------------
		glStencilFunc(GL_EQUAL, 1, 1); // pass (draw pixel) if stencil buf content == 1
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // don't modify stencil buf
		
		highlight.draw(250,int(130+sin(sin_count)*100));
		gl_state.set_active();

		window.flip();
		CL_System::keep_alive(20);
		
		int error = glGetError();
		while( error != 0 )
		{
			std::cout << std::hex << "GLError: 0x" << error << std::endl;
			error = glGetError();
		}
	}

	quit = false;
}

struct Triangle { double x[3]; double y[3]; };
std::vector<Triangle> triangles;

void on_tesselate_triangle(void *data1, void *data2, void *data3)
{
	POINT *p1 = (POINT *) data1;
	POINT *p2 = (POINT *) data2;
	POINT *p3 = (POINT *) data3;

	Triangle triangle;
	triangle.x[0] = p1->x;
	triangle.y[0] = p1->y;
	triangle.x[1] = p2->x;
	triangle.y[1] = p2->y;
	triangle.x[2] = p3->x;
	triangle.y[2] = p3->y;
	triangles.push_back(triangle);
}

void Application::tesselate_test()
{
	CL_OpenGLWindowDescription desc;
	desc.set_title("ClanLib clanDisplay 2.0 test program");
	desc.set_size(CL_Size(1024,768));
	desc.set_fullscreen(false);

	CL_DisplayWindow window(desc);

	HDC screen_dc = GetDC(0);
	BeginPath(screen_dc);
	HFONT font = CreateFont(400, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "Comic Sans MS");
	HGDIOBJ old_font = SelectObject(screen_dc, font);
	int old_bkmode = SetBkMode(screen_dc, TRANSPARENT);
	TextOut(screen_dc, 10, 10, "4", 1);
//	TextOut(screen_dc, 10, 10, "ClanLib", 7);
//	TextOut(screen_dc, 10, 300, "123456", 6);
	SetBkMode(screen_dc, old_bkmode);
	SelectObject(screen_dc, old_font);
	EndPath(screen_dc);
	FlattenPath(screen_dc);

	CL_OutlineTriangulator tesselator;
//	CL_Slot slot = tesselator.sig_triangle().connect(&on_tesselate_triangle);

	int num_points = GetPath(screen_dc, 0, 0, 0);
	POINT *points = new POINT[num_points];
	BYTE *types = new BYTE[num_points];
	GetPath(screen_dc, points, types, num_points);
	POINT last_moveto = points[0];
	int last_moveto_index = 0;
	for (int i=0; i<num_points; i++)
	{
		if (types[i] == PT_MOVETO && i != 0)
		{
			tesselator.next_contour();
			last_moveto = points[i];
			last_moveto_index = i;
		}
		// If duplicate point. Skip it.
//		if (i < 1 || memcmp(points+i, points+i-1, sizeof(POINT)) != 0)
			tesselator.add_vertex(points[i].x, points[i].y, points+i);

		if ((types[i] & PT_CLOSEFIGURE) == PT_CLOSEFIGURE)
		{
//			tesselator.add_vertex(last_moveto.x, last_moveto.y, points+last_moveto_index);
			tesselator.next_polygon();
//			break;
		}
	}
	tesselator.generate();

	delete[] points;
	delete[] types;

	int c = 0;
	while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
	{
		CL_Display::clear(CL_Color::black);

		std::vector<Triangle>::size_type size_triangles, index_triangles;
		size_triangles = triangles.size();
		for (index_triangles = 0; index_triangles < size_triangles; index_triangles++)
		{
			Triangle &cur_triangle = triangles[index_triangles];

			CL_Display::draw_line(
				cur_triangle.x[0], cur_triangle.y[0],
				cur_triangle.x[1], cur_triangle.y[1],
				CL_Color::grey);

			CL_Display::draw_line(
				cur_triangle.x[1], cur_triangle.y[1],
				cur_triangle.x[2], cur_triangle.y[2],
				CL_Color::grey);

			CL_Display::draw_line(
				cur_triangle.x[2], cur_triangle.y[2],
				cur_triangle.x[0], cur_triangle.y[0],
				CL_Color::grey);
		}
/*
		Triangle &cur_triangle = triangles[c];

		CL_Display::draw_line(
			cur_triangle.x[0], cur_triangle.y[0],
			cur_triangle.x[1], cur_triangle.y[1],
			CL_Color::yellow);

		CL_Display::draw_line(
			cur_triangle.x[1], cur_triangle.y[1],
			cur_triangle.x[2], cur_triangle.y[2],
			CL_Color::yellow);

		CL_Display::draw_line(
			cur_triangle.x[2], cur_triangle.y[2],
			cur_triangle.x[0], cur_triangle.y[0],
			CL_Color::yellow);

		c++;
		if (c > size_triangles) c = 0;
*/
		CL_Display::flip();
		CL_System::keep_alive(10);
	}
}

void Application::viewport_test()
{
/*	CL_OpenGLWindowDescription desc;
	desc.set_title("ClanLib clanDisplay 2.0 test program");
	desc.set_size(CL_Size(640,480));
	desc.set_allow_resize(true);
	desc.set_fullscreen(false);

	CL_DisplayWindow window(desc);

	CL_Log::log("debug", "Size of viewport: %1 x %2", CL_Display::get_width(), CL_Display::get_height());

	CL_PixelBuffer pb(101, 101, 101*4, CL_PixelFormat::rgba8888);
	pb.lock();
	unsigned int *pdata = (unsigned int *) pb.get_data();
	for (int i=0; i<101*101; i++) pdata[i] = 0x000000ff;
	int xx, yy;
	for (yy = 0; yy < 101; yy += 2)
	{
		for (xx = 0; xx < 101; xx++)
		{
			pdata[yy*101+xx] = 0xffffffff;
		}
	}
	for (xx = 0; xx < 101; xx += 2)
	{
		for (yy = 0; yy < 101; yy++)
		{
			pdata[yy*101+xx] = 0xffffffff;
		}
	}
	pb.unlock();

	CL_Surface surf(&pb);

	while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
	{
		CL_Display::clear(CL_Color::black);

		for (int x = 0; x < CL_Display::get_width(); x+=2)
		{
			CL_Display::draw_line(x, 0, x, CL_Display::get_height(), CL_Color::white);
		}

		for (int y = 0; y < CL_Display::get_height(); y+=2)
		{
			CL_Display::draw_line(0, y, CL_Display::get_width(), y, CL_Color::white);
		}

		CL_Display::draw_line(CL_Display::get_width()-1, 0, CL_Display::get_width()-1, CL_Display::get_height()-1, CL_Color::blue);
		CL_Display::draw_line(0, CL_Display::get_height()-1, CL_Display::get_width()-1, CL_Display::get_height()-1, CL_Color::blue);
		CL_Display::draw_line(0, 0, CL_Display::get_width()-1, 0, CL_Color::blue);
		CL_Display::draw_line(0, 0, 0, CL_Display::get_height()-1, CL_Color::blue);

		surf.draw((rand()%(CL_Display::get_width()/2))*2, (rand()%(CL_Display::get_height()/2))*2);

		CL_Display::flip();
		CL_System::keep_alive(100);
	}
*/
}
