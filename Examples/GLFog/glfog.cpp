//------------------------------------------------------
//
// FOG By Johan 'Mace' GLysing		-		mace-x@usa.net
//
// Converted to ClanLib by Kenneth Gangstoe
//
//------------------------------------------------------

#include <stdio.h>
#include <ClanLib/gl.h>

#include "glfog.h"
using namespace std;

//------------------------------------------------------
// Todo note: These data should be put into class

// BASIC APPLICATION STUFF
static GLint i, j;													// For loops
static GLint winW = 640, winH = 480;								// Window width and height

// AUTO SWITCH
static GLboolean useTimer = GL_TRUE;								// Auto change mode every 25th second
static GLint timer = 0;												// The timer

// LIGHTS
static GLint fogMode = 0;											// Different patterns
static GLint lights = 10;											// Number of lights
static GLfloat lightX[10], lightY[10], lightZ[10], lightT[10];
// LIGHT REFLECTIVE COLORS
static const GLubyte lightR[]  = { 255, 1,   1,   255, 255, 1,   150, 1,   255, 255 };
static const GLubyte lightG[]  = { 1,   255, 1,   1,   255, 255, 1,   150, 150, 1   };
static const GLubyte lightB[]  = { 1,   1,   255, 255, 1,   255, 255, 255, 1,   100 };
// LIGHT COLORS
static const GLubyte lightR2[] = { 255, 100, 100, 255, 255, 100, 150, 50,  255, 255 };
static const GLubyte lightG2[] = { 100, 255, 100, 100, 255, 255, 100, 150, 150, 50  };
static const GLubyte lightB2[] = { 100, 100, 255, 255, 100, 255, 255, 255, 50,  100 };

// FOG
static GLint fogW = 5, fogH = 8, fogNum;							// Width/Height of fog cloud and w*h
static GLfloat fogX[100], fogY[100], fogZ[100], fogANG[100];		// Fog coords and angle
static GLubyte fogC[100][3];										// Fog colors
static GLint fogCLR = 0;											// Colors of fog cloud
static const GLint fogR[] = { 50, 50, 10, 10, 50, 50, 10 };
static const GLint fogG[] = { 50, 10, 50, 10, 10, 50, 50 };
static const GLint fogB[] = { 50, 10, 10, 50, 50, 10, 50 };

// DISPLAY LISTS
static GLuint Background, fogPuff, lightSphere;						// Display lists

// TIMER
static GLint startTime = 0, endTime = 0;

//------------------------------------------------------

FogApplication app;

//------------------------------------------------------

int FogApplication::main(int argc, char** argv)
{
	// Create a console window for text-output if not available
	CL_ConsoleWindow console("Console");
	console.redirect_stdio();

	try
	{
		CL_SetupCore setup_core;
		CL_SetupGL setup_gl;
		CL_SetupDisplay setup_display;

		CL_DisplayWindow window("GLFog", 640, 480);
		glstate = new CL_OpenGLState(window.get_gc());

		// Connect the Window close event
		CL_Slot slot_quit = window.sig_window_close().connect(this, &FogApplication::on_window_close);
		
		init();
		run();

		delete manager;
		delete texture[0];
		delete texture[1];
		delete texture[2];
	}
	catch (CL_Error err)
	{
		cout << "Exception caught: " << err.message.c_str() << endl;

		// Display console close message and wait for a key
		console.display_close_message();
	}

	return 0;
}

void FogApplication::init()
{
	init_gl();
	init_textures();
	init_display_lists();
	init_fog();
	init_lightpos();
}

void FogApplication::init_gl()
{
	glstate->set_active();

	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);

	glMatrixMode (GL_MODELVIEW);

	glShadeModel(GL_FLAT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	gluLookAt(0, 5, 10, 0, 0, 0, 0, 1, 0);
}

void FogApplication::init_textures()
{
	manager = new CL_ResourceManager("glfog.xml", false);

	texture[0] = new CL_OpenGLSurface("floor", manager);
	texture[1] = new CL_OpenGLSurface("fog", manager);
	texture[2] = new CL_OpenGLSurface("sphere", manager);
}

void FogApplication::init_fog()
{
	GLint i, w, h, num;
	GLfloat ft = 0, stepsX, stepsZ, split;

	// INIT FOG
	i = 0;
	stepsX = 3.0f;
	stepsZ = 2.0f;
	split = stepsX*(fogW/2);
	for(h = 0; h < fogH; h++)
	{
		for(w = 0; w < fogW; w++)
		{
			num = (h*fogW)+w; 
			fogX[num] = ((stepsX+ft)*w)-split;
			fogY[num] = 0;
			fogZ[num] = (stepsZ*h)-split;

			i += 41;
			if(i > 359) i -= 359;

			if(i < 50) ft = -0.25f;
			else if(i < 100) ft = -0.2f;
			else if(i < 150) ft = -0.12f;
			else if(i < 200) ft = 0.0f;
			else if(i < 250) ft = 0.12f;
			else if(i < 300) ft = 0.2f;
			else ft = 0.25f;

			fogANG[num] = i;
	
			fogC[num][0] = 255;
			fogC[num][1] = 255;
			fogC[num][2] = 255;
		}
	}

	fogNum = fogH*fogW;
}

void FogApplication::init_lightpos()
{
	GLint i;
	GLfloat ft = 0;

	ft = (6.28 / lights);
	for(i = 0; i < lights; i++)
	{
		lightX[i] = 5 * cos( 1.57 + (ft*i) );
		lightY[i] = 0;
		lightZ[i] = 5 * sin( 1.57 + (ft*i) );
		lightT[i] = 1.57+(ft*i);
	}
}

void FogApplication::init_display_lists()
{
	glstate->set_active();

	// RENDERS A BLACK BG QUAD IN 2D COORD SYSTEM
	Background = glGenLists(1);
	glNewList(Background, GL_COMPILE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, winW, 0, winH);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glColor3f(0, 0, 0);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(winW, 0);
			glVertex2f(winW, winH);
			glVertex2f(0, winH);
		glEnd();

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		texture[0]->bind();
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(-8, 0, -8);
			glTexCoord2f(0, 1); glVertex3f(-8, 0,  8);
			glTexCoord2f(1, 1); glVertex3f( 8, 0,  8);
			glTexCoord2f(1, 0); glVertex3f( 8, 0, -8);
		glEnd();
	glEndList();

	// FOG
	fogPuff = glGenLists(1);
	glNewList(fogPuff, GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(-3, 0, -3);
			glTexCoord2f(0, 1); glVertex3f(-3, 0,  3);
			glTexCoord2f(1, 1); glVertex3f( 3, 0,  3);
			glTexCoord2f(1, 0); glVertex3f( 3, 0, -3);
		glEnd();
	glEndList();

	// LIGHTSPHERES
	lightSphere = glGenLists(1);
	glNewList(lightSphere, GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(-0.5, 0, -0.5);
			glTexCoord2f(0, 1); glVertex3f(-0.5, 0,  0.5);
			glTexCoord2f(1, 1); glVertex3f( 0.5, 0,  0.5);
			glTexCoord2f(1, 0); glVertex3f( 0.5, 0, -0.5);
		glEnd();
	glEndList();
}

void FogApplication::draw()
{
	glstate->set_active();

	// RENDER THE BG
	glClear(GL_DEPTH_BUFFER_BIT);
	glCallList(Background);

	// RENDER THE FOG CLOUD
	texture[1]->bind();
	for(i = 0; i < fogNum; i++)
	{
		glColor3ub(fogC[i][0], fogC[i][1], fogC[i][2]);

		glPushMatrix();
			glTranslatef(fogX[i], fogY[i]+1, fogZ[i]);
			glRotatef(40, 1, 0, 0);
			glRotatef(fogANG[i], 0, 1, 0);
			glCallList(fogPuff);
		glPopMatrix();

		fogANG[i]++;
		if(fogANG[i] > 359) fogANG[i] = 0;
	}

	// LIGHT SPHERES
	texture[2]->bind();
	for(i = 0; i < lights; i++)
	{
		glColor3ub(lightR2[i], lightG2[i], lightB2[i]);
		glPushMatrix();
			glTranslatef(lightX[i], lightY[i]+1, lightZ[i]);
			glRotatef(60, 1, 0, 0);
			glCallList(lightSphere);

			if(fogMode == 1)
			{
				glTranslatef(0, -4, 0);
				glCallList(lightSphere);
				glTranslatef(0, -4, 0);
				glCallList(lightSphere);
				glTranslatef(0, -4, 0);
				glCallList(lightSphere);
			}
		glPopMatrix();
	}
}

void FogApplication::set_lights()
{
	GLint i, j;
	GLfloat vX, vY, vZ, L;
	GLint r[10], g[10], b[10], R, G, B;

	// CALCULATE THE FOG COLORS FROM THE LIGHT POSITIONS
	for(j = 0; j < fogNum; j++)
	{
		for(i = 0; i < lights; i++)
		{
			vX = lightX[i] - fogX[j];
			vY = lightY[i] - fogY[j];
			vZ = lightZ[i] - fogZ[j];
			L = sqrt(vX*vX + vY*vY + vZ*vZ);
			r[i] = (GLint) (lightR[i]/L);
			g[i] = (GLint) (lightG[i]/L);
			b[i] = (GLint) (lightB[i]/L);
		}

		R = 0;
		G = 0;
		B = 0;
		for(i = 0; i < lights; i++)
		{
			R += r[i];
			G += g[i];
			B += b[i];
		}
		R /= lights;
		G /= lights;
		B /= lights;

		if(R > 200) R = 200;
		if(G > 200) G = 200;
		if(B > 200) B = 200;

		fogC[j][0] = fogR[fogCLR]+(R-255);
		fogC[j][1] = fogG[fogCLR]+(G-255);
		fogC[j][2] = fogB[fogCLR]+(B-255);
	}

	// MOVE THE LIGHTS
	for(i = 0; i < lights; i++)
	{
		if(fogMode == 0)
		{
			lightX[i] = 5 * cos(lightT[i]);
			lightY[i] = 0.5 * cos((lightT[i]*2));
			lightZ[i] = 5 * sin(lightT[i]);
		}
		else if(fogMode == 1)
		{
			lightX[i] = 5 * cos(lightT[i]);
			lightY[i] = 1 * cos((lightT[i]*4));
			lightZ[i] = 5 * sin(lightT[i]);
		}
		else if(fogMode == 2)
		{
			lightX[i] = 3 * cos((lightT[i]*200));
			lightY[i] = 0;
			lightZ[i] = 3 * sin((lightT[i]*200));
		}
		else if(fogMode == 3)
		{
			lightX[i] = lightT[i] * cos(lightT[i]*4);
			lightY[i] = lightT[i];
			lightZ[i] = lightT[i] * sin(lightT[i]*4);
		}
		else if(fogMode == 4)
		{
			lightX[i] = 4 * cos((lightT[i]*2));
			lightY[i] = 2 * cos(lightT[i]);
			lightZ[i] = 3 * sin(lightT[i]);
		}

		lightT[i] += 0.034f;
		if(lightT[i] > 6.28) lightT[i] -= 6.28f;
	}
}

void FogApplication::check_timers()
{
	if(startTime == 0)
	{
		startTime = CL_System::get_time();
		set_lights();
	}
	else {
		endTime = CL_System::get_time();

		// UPDATE
		if(endTime - startTime > 0)
		{
			startTime = 0;

			if(useTimer)
			{
				timer++;
				if(timer > 450)
				{
					timer = 0;
					i = fogMode;
					j = fogCLR;
					fogMode = (rand() % 5);
					fogCLR = (rand() % 7);

					if(i == fogMode)
					{
						fogMode++;
						if(fogMode > 4) fogMode = 0;
					}

					if(j == fogCLR)
					{
						fogCLR++;
						if(fogCLR > 6) fogCLR = 0;
					}
				}
			}
		}
	}
}

void FogApplication::run()
{
	bool key_LEFT, key_RIGHT, key_UP, key_DOWN;
	bool key_F1, key_F3;
	key_LEFT = key_RIGHT = key_UP = key_DOWN = key_F1 = key_F3 = false;

	do {
		check_timers();
		draw();

		CL_Display::flip();
		CL_System::keep_alive(1);

		if(CL_Keyboard::get_keycode(CL_KEY_UP) && !key_UP) 
		{
			fogMode++;
			if(fogMode > 4) fogMode = 0;
		}
		key_UP = CL_Keyboard::get_keycode(CL_KEY_UP);
		
		if(CL_Keyboard::get_keycode(CL_KEY_DOWN) && !key_DOWN) 
		{
			fogMode--;
			if(fogMode < 0) fogMode = 4;
		}
		key_DOWN = CL_Keyboard::get_keycode(CL_KEY_DOWN);

		if(CL_Keyboard::get_keycode(CL_KEY_RIGHT) && !key_RIGHT) 
		{
			fogCLR++;
			if(fogCLR > 6) fogCLR = 0;
		}
		key_RIGHT = CL_Keyboard::get_keycode(CL_KEY_RIGHT);

		if(CL_Keyboard::get_keycode(CL_KEY_LEFT) && !key_LEFT) 
		{
			fogCLR--;
			if(fogCLR < 0) fogCLR = 6;
		}
		key_LEFT = CL_Keyboard::get_keycode(CL_KEY_LEFT);

		if(CL_Keyboard::get_keycode(CL_KEY_F1) && !key_F1) 
		{
			srand((GLint)CL_System::get_time());
			useTimer = !useTimer;
			timer = 450;
		}
		key_F1 = CL_Keyboard::get_keycode(CL_KEY_F1);

		if(CL_Keyboard::get_keycode(CL_KEY_F3) && !key_F3) 
		{
			if(lights == 3) lights = 6;
			else if(lights == 6) lights = 10;
			else if(lights == 10) lights = 3;
			init_lightpos();
		}
		key_F3 = CL_Keyboard::get_keycode(CL_KEY_F3);

		if(CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			quit = true;
	} while(quit == false);
}

void FogApplication::on_window_close()
{
	quit = true;
}
