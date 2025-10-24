/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#ifndef header_holder_component
#define header_holder_component

class HolderComponent_Impl;
class CallbackInfo;

class CL_API_GUI HolderComponent : public CL_GUIComponent
{
//! Construction:
public:
	HolderComponent(CL_GUIComponent *parent);
	
	virtual ~HolderComponent() {};

//! Attributes:
public:
	bool marked_for_delete();

	CL_ComponentAnchorPoint get_anchor_tl();

	CL_ComponentAnchorPoint get_anchor_br();

	CallbackInfo get_callback_info();

//! Operations:
public:
	void on_process_message(CL_GUIMessage &msg);

	void set_selected(bool value);

	CL_DomElement to_element(CL_DomDocument &doc);

	void set_anchor_tl(CL_ComponentAnchorPoint);
	void set_anchor_br(CL_ComponentAnchorPoint);

//! Events:
public:

//! Implementation:
private:
	CL_SharedPtr<HolderComponent_Impl> impl;
};

#endif
