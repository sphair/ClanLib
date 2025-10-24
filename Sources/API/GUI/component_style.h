/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_component_style
#define header_component_style

#if _MSC_VER > 1000
#pragma once
#endif

class CL_Component;

//: Component Style interface.
//- !group=GUI/Framework!
//- !header=gui.h!
//- <p>The component style class is part of the style customization system of the
//- GUI. Each 'style' for a component is implemented by inheriating
//- CL_ComponentStyle and then attaching it to the component it is responsible
//- of visualizing.</p>
//- <p>When a component is constructed, it will contact its CL_StyleManager and
//- ask it to attach the component styles for its style to the component.
//- The general principle in the component style system is that a inheriated
//- CL_ComponentStyle object will hook itself into signals in the component it
//- is visualizing, thus keeping a kind of document/view relationship with its
//- component.</p>
//- <p>For source examples of how this is done, have a look at the GUIGL example.</p>
class CL_ComponentStyle
{
//! Construction:
public:
	//: Construct a component style.
	CL_ComponentStyle(CL_Component *component) : comp(component) { return; }

	//: Destructor.
	virtual ~CL_ComponentStyle() { return; }

//! Attributes:
public:
	//: Returns the component that is being styled.
	CL_Component *get_component() { return comp; }

	//: Returns the component that is being styled.
	const CL_Component *get_component() const { return comp; }

//! Implementation:
private:
	CL_Component *comp;
};

#endif
