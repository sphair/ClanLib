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

#ifndef header_dom_character_data
#define header_dom_character_data

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

#include "dom_node.h"

//: DOM Character Data class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>The CharacterData interface extends Node with a set of attributes and methods
//- for accessing character data in the DOM. For clarity this set is defined here
//- rather than on each object that uses these attributes and methods. No DOM objects
//- correspond directly to CharacterData, though Text and others do inherit the
//- interface from it.</p>
class CL_API_CORE CL_DomCharacterData : public CL_DomNode
{
//! Construction:
public:
	//: Constructs a DOM CharacterData handle.
	CL_DomCharacterData();

	CL_DomCharacterData(const CL_SharedPtr<CL_DomNode_Generic> &impl);
	
	~CL_DomCharacterData();
	
//! Attributes:
public:
	//: The number of characters that are available through data and the substringData method below.
	//- <p>This may have the value zero, i.e., CharacterData nodes may be empty.</p>
	unsigned long get_length();

//! Operations:
public:
	//: Extracts a range of data from the node.
	//param offset: Start offset of substring to extract.
	//param count: The number of characters to extract.
	//retval: The specified substring. If the sum of offset and count exceeds the length, then all characters to the end of the data are returned.
	std::string substring_data(unsigned long offset, unsigned long count);

	//: Append the string to the end of the character data of the node.
	void append_data(const std::string &arg);

	//: Insert a string at the specified character offset.
	//param offset: The character offset at which to insert.
	//param arg: The DOMString to insert.
	void insert_data(unsigned long offset, const std::string &arg);

	//: Remove a range of characters from the node.
	//param offset: The offset from which to remove characters.
	//param count: The number of characters to delete. If the sum of offset and count exceeds length then all characters from offset to the end of the data are deleted.
	void delete_data(unsigned long offset, unsigned long count);

	//: Replace the characters starting at the specified character offset with the specified string.
	//param offset: The offset from which to start replacing.
	//param count: The number of characters to replace. If the sum of offset and count exceeds length, then all characters to the end of the data are replaced (i.e., the effect is the same as a remove method call with the same range, followed by an append method invocation).
	//param arg: The DOMString with which the range must be replaced.
	void replace_data(unsigned long offset, unsigned long count, const std::string &arg);

//! Implementation:
protected:
	CL_DomCharacterData(CL_DomDocument &doc, unsigned short node_type);
};

#endif
