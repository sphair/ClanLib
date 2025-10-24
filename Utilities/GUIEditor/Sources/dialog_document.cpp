/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Harry Storbacka
*/

#include "precomp.h"
#include "dialog_document.h"
#include "dialog_container.h"

DialogDocument::DialogDocument()
: next_free_id(1), container(0)
{
	CL_DomElement element_gui = doc.create_element("gui"); 
	element_gui.set_attribute("xmlns", namespace_uri);
	doc.append_child(element_gui);
	container = new DialogContainer(this, element_gui);
}

DialogDocument::~DialogDocument()
{
	delete container;
}

CL_String DialogDocument::namespace_uri = "http://clanlib.org/xmlns/gui-1.0";

void DialogDocument::load(const CL_StringRef &name)
{
	CL_DomDocument doc;
	CL_File file = CL_File(name, CL_File::open_existing, CL_File::access_read);
	doc.load(file);
	if (doc.get_document_element().get_local_name() != "gui" || doc.get_document_element().get_namespace_uri() != namespace_uri)
		throw CL_Exception(cl_format("%1 is not a valid GUI Definition XML file", name));

	delete container;
	next_free_id = 1;
	container = new DialogContainer(this, doc.get_document_element());
}

void DialogDocument::save(const CL_StringRef &name)
{
	CL_File file;
	file.open(name, CL_File::create_always, CL_File::access_write);
	doc.save(file);
}
