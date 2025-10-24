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

#pragma once

class GUIWindowManagerDirect_Impl;
class GUIWindowManagerProvider_Direct;
class GUIWindowManagerDirectWindow;

class CL_API_GUI GUIWindowManagerDirect : public CL_GUIWindowManager
{

public:

	/// \brief Constructs a null instance.
	GUIWindowManagerDirect();

	/// \brief Constructs a GUIWindowManagerTexture
	///
	/// \param display_window = Display Window
	GUIWindowManagerDirect(CL_DisplayWindow display_window);
	GUIWindowManagerDirect(const CL_GUIWindowManager &wm);
	~GUIWindowManagerDirect();

public:
	/// \brief Retrieves the provider.
	GUIWindowManagerProvider_Direct *get_provider() const;

	/// \brief Get the window textures
	///
	/// \param only_visible = True = Only return windows when they are visible
	/// \return The window textures
	std::vector<GUIWindowManagerDirectWindow> get_windows(bool only_visible = true) const;

public:
	/// \brief void func_input_intercept(const CL_InputEvent &input_event, const CL_InputState &input_state)
	CL_Callback_v2<CL_InputEvent &, CL_InputState &> &func_input_intercept();

private:

};
