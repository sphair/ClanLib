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
**    Harry Storbacka
*/

#pragma once


#include "API/Core/CSS/css_document.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/gui_component.h"
#include "API/Core/Signals/callback_2.h"
#include <vector>
#include <map>


class CL_GUIMessage;
class CL_GUITheme;
class CL_GUIWindowManager;
class CL_GUIComponent_Impl;
class CL_InputEvent;
class CL_InputState;


class CL_GUIXMLLoaderVersion_1_0
{
/// \name Construction
/// \{

public:
	CL_GUIXMLLoaderVersion_1_0(CL_GUIComponent *component, CL_GUILayout &layout);

	~CL_GUIXMLLoaderVersion_1_0();


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	void load(CL_DomDocument &doc);

	void set_create_custom_callback(CL_Callback_2<CL_GUIComponent*, CL_GUIComponent*, CL_String> *callback);


/// \}
/// \name Implementation
/// \{

private:
	void load(CL_DomElement &element, CL_GUIComponent *parent);

	CL_GUIComponent *component;
	CL_GUILayout layout;

	CL_Callback_2<CL_GUIComponent*, CL_GUIComponent*, CL_String> *create_custom_callback;

	int dialog_width;
	int dialog_height;
	CL_String dialog_title;

/// \}
};


