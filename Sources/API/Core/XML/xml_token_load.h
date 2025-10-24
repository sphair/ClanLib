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

#ifndef header_xml_token_load
#define header_xml_token_load

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

#include "../System/sharedptr.h"
#include "xml_token.h"
#include <string>

class CL_XMLTokenLoad_Generic;
class CL_XMLTokenString;

//: XML token in a XML file.
//- !group=Core/XML!
//- !header=core.h!
class CL_API_CORE CL_XMLTokenLoad : public CL_XMLToken
{
//! Construction:
public:
	CL_XMLTokenLoad();
	CL_XMLTokenLoad(const CL_XMLTokenLoad &copy);
	
	virtual ~CL_XMLTokenLoad();
	
//! Attributes:
public:

	//: Returns the token type.
	virtual TokenType get_type() const;
	
	//: Returns the token variant.
	virtual TokenVariant get_variant() const;
	
	//: Returns the name of the token.
	virtual std::string get_name() const;
	
	//: Returns the value of the token.
	virtual std::string get_value() const;
	
	//: Returns number of attributes.
	virtual int get_attributes_number() const;

	//: Returns the attribute name and value for attribute with specified index.
	virtual std::pair<std::string, std::string> get_attribute(int attribute_num) const;

	const std::pair<CL_XMLTokenString, CL_XMLTokenString> & get_attribute_fast(int attribute_num) const;

//! Operations:
public:
	//: Sets the type of the token.
	void set_type(TokenType type);

	//: Sets if token ends with a slash.
	void set_variant(TokenVariant variant);

	//: Sets the name of the token.
	void set_name(const CL_XMLTokenString & name);

	//: Sets the value of the token.
	void set_value(const CL_XMLTokenString & value);
	
	//: Attaches attribute to token.
	void set_attribute(const CL_XMLTokenString & name, const CL_XMLTokenString & value);

//! Implementation:
private:
	CL_SharedPtr<CL_XMLTokenLoad_Generic> impl;
};

#endif
