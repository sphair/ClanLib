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

//! clanCore="XML"
//! header=core.h

#ifndef header_dom_text
#define header_dom_text

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "dom_character_data.h"

//: DOM Text class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>The Text interface represents the textual content (termed character data
//- in XML) of an Element or Attr. If there is no markup inside an element's content,
//- the text is contained in a single object implementing the Text interface that is
//- the only child of the element. If there is markup, it is parsed into a list of
//- elements and Text nodes that form the list of children of the element.</p>
//- <p>When a document is first made available via the DOM, there is only one Text
//- node for each block of text. Users may create adjacent Text nodes that represent
//- the contents of a given element without any intervening markup, but should be aware
//- that there is no way to represent the separations between these nodes in XML or HTML,
//- so they will not (in general) persist between DOM editing sessions. The normalize()
//- method on Element merges any such adjacent Text objects into a single node for each
//- block of text; this is recommended before employing operations that depend on a
//- particular document structure, such as navigation with XPointers.</p>
class CL_API_CORE CL_DomText : public CL_DomCharacterData
{
//! Construction:
public:
	//: Constructs a DOM Text handle.
	CL_DomText();

	CL_DomText(CL_DomDocument &doc, const std::string &data);

	CL_DomText(const CL_SharedPtr<CL_DomNode_Generic> &impl);
	
	~CL_DomText();

//! Attributes:
public:

//! Operations:
public:
	//: Breaks this Text node into two Text nodes at the specified offset, keeping both in the tree as siblings.
	//- <p>node then only contains all the content up to the offset point. And a new Text node, which
	//- is inserted as the next sibling of this node, contains all the content at and after the offset
	//- point.</p>
	//param offset: The offset at which to split.
	//retval: The new Text node.
	CL_DomText splitText(unsigned long offset);

//! Implementation:
protected:
	CL_DomText(CL_DomDocument &doc, unsigned short node_type);
};

#endif
