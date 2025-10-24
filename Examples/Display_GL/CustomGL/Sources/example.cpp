/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#include "precomp.h"
#include "example.h"

// The start of the Application
int Example::start(const std::vector<CL_String> &args)
{
	quit = false;

	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib Custom OpenGL Example");
	desc.set_size(CL_Size(640, 480), true);
	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &Example::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Example::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	gc.set_map_mode(cl_user_projection);

	// Run until someone presses escape
	while (!quit)
	{

		CL_Mat4f perp = CL_Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 100000.0f);
		gc.set_projection(perp);
		gc.flush_batcher();

		// Note, use "CL_GraphicContext_GL1", if you use the GL1 target
		CL_GraphicContext_GL1 gl_gc(gc);
		gl_gc.set_active();

		// Note, use "cl1" insteal of "cl", if you use the GL1 target

		cl1MatrixMode(CL_MODELVIEW);							// Select The Modelview Matrix
		cl1Clear(CL_COLOR_BUFFER_BIT | CL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		cl1LoadIdentity();									// Reset The Current Modelview Matrix
		cl1Translatef(-1.5f,0.0f,-6.0f);						// Move Left 1.5 Units And Into The Screen 6.0
		cl1Begin(CL_TRIANGLES);								// Drawing Using Triangles
			cl1Color3f(1.0f,0.0f,0.0f);						// Set The Color To Red
			cl1Vertex3f( 0.0f, 1.0f, 0.0f);					// Top
			cl1Color3f(0.0f,1.0f,0.0f);						// Set The Color To Green
			cl1Vertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
			cl1Color3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
			cl1Vertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
		cl1End();											// Finished Drawing The Triangle
		cl1Translatef(3.0f,0.0f,0.0f);						// Move Right 3 Units
		cl1Color3f(0.5f,0.5f,1.0f);							// Set The Color To Blue One Time Only
		cl1Begin(CL_QUADS);									// Draw A Quad
			cl1Vertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
			cl1Vertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
			cl1Vertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
			cl1Vertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
		cl1End();											// Done Drawing The Quad


		// Flip the display, showing on the screen what we have drawed
		// since last call to flip()
		window.flip(1);

		// This call processes user input and other events
		CL_KeepAlive::process();
	}

	return 0;
}

// A key was pressed
void Example::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Example::on_window_close()
{
	quit = true;
}


