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

#ifndef header_xml_token
#define header_xml_token

#if _MSC_VER > 1000
#pragma once
#endif

//#include <map>
#include <string>

//class CL_XMLToken_Generic;

//: XML token in a XML file.
//- !group=Core/XML!
//- !header=core.h!
class CL_XMLToken
{
//! Construction:
public:
//	CL_XMLToken();
//	
//	CL_XMLToken(const CL_XMLToken &copy);
	
	virtual ~CL_XMLToken() {};
	
//! Attributes:
public:
	enum TokenType
	{
		NULL_TOKEN                     = 0,
		ELEMENT_TOKEN                  = 1,
		TEXT_TOKEN                     = 3,
		CDATA_SECTION_TOKEN            = 4,
		ENTITY_REFERENCE_TOKEN         = 5,
		ENTITY_TOKEN                   = 6,
		PROCESSING_INSTRUCTION_TOKEN   = 7,
		COMMENT_TOKEN                  = 8,
		DOCUMENT_TYPE_TOKEN            = 10,
		NOTATION_TOKEN                 = 12
	};
	
	enum TokenVariant
	{
		BEGIN  = 1,
		END    = 2,
		SINGLE = 3
	};

	//: Returns the token type.
	virtual TokenType get_type() const = 0;
	
	//: Returns the token variant.
	virtual TokenVariant get_variant() const = 0;
	
	//: Returns the name of the token.
	virtual std::string get_name() const = 0;
	
	//: Returns the value of the token.
	virtual std::string get_value() const = 0;
	
/*
	//: Returns all the attributes attached to the token.
	const std::vector< std::pair<std::string, std::string> > &get_attributes() const;
*/
	virtual int get_attributes_number() const = 0;

	//: Returns the attribute name and value for attribute with specified index.
	virtual std::pair<std::string, std::string> get_attribute(int attribute_num) const = 0;

	//: Returns the value of a named attribute.
//	virtual std::string get_attribute(const std::string &name) const = 0;

//! Operations:
//public:
	//: Copy assignment operator.
//	CL_XMLToken &operator =(const CL_XMLToken &copy);

//	//: Sets the type of the token.
//	void set_type(TokenType type);
//
//	//: Sets if token ends with a slash.
//	void set_variant(TokenVariant variant);
//
//	//: Sets the name of the token.
//	void set_name(const std::string &name);
//
//	//: Sets the value of the token.
//	void set_value(const std::string &value);
//	
//	//: Attaches attribute to token.
//	void set_attribute(const std::string &name, const std::string &value);

//! Implementation:
//private:
//	CL_XMLToken_Generic *impl;
};

#endif
