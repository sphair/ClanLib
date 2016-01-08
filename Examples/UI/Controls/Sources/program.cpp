/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "program.h"
#include "../../../ThemeAero/Sources/theme.h"

using namespace clan;

clan::ApplicationInstance<Program> clanapp;

Program::Program()
{
	clan::Application::use_timeout_timing(std::numeric_limits<int>::max());	// The update() loop is not required for this application

	// We support all display targets, in order listed here
	clan::D3DTarget::enable();
	//clan::OpenGLTarget::enable();

	// Create a source for our resources
	FileResourceDocument doc(FileSystem("../../ThemeAero"));
	ResourceManager resources = FileResourceManager::create(doc);

	// Mark this thread as the UI thread
	ui_thread = UIThread(resources);

	// Create root view and window:
	DisplayWindowDescription desc;
	desc.set_title("Controls Example");
	desc.set_allow_resize(true);
	desc.set_size(Sizef(400, 500), false);
	form = std::make_shared<Form>(desc);

	// Exit run loop when close is clicked.
	// We have to store the return Slot because if it is destroyed the lambda function is disconnected from the signal.
	slots.connect(form->sig_close(), [&](CloseEvent &e) { RunLoop::exit(); });

	// Make our window visible
	form->show();
}

bool Program::update()
{
	return true;
}
