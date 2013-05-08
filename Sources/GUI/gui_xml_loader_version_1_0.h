/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Harry Storbacka
*/

#pragma once

#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/gui_component.h"
#include "API/Core/Signals/callback_2.h"
#include <vector>
#include <map>

namespace clan
{

class GUIMessage;
class GUITheme;
class GUIWindowManager;
class GUIComponent_Impl;
class InputEvent;

class GUIXMLLoaderVersion_1_0
{
/// \name Construction
/// \{

public:
	GUIXMLLoaderVersion_1_0(GUIComponent *component, GUILayout &layout);

	~GUIXMLLoaderVersion_1_0();


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	void load(DomDocument &doc);

	void set_create_custom_callback(Callback_2<GUIComponent*, GUIComponent*, std::string> *callback);


/// \}
/// \name Implementation
/// \{

private:
	void load(DomElement &element, GUIComponent *parent);

	GUIComponent *component;
	GUILayout layout;

	Callback_2<GUIComponent*, GUIComponent*, std::string> *create_custom_callback;

	int dialog_width;
	int dialog_height;
	std::string dialog_title;

/// \}
};

}
