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

#include "precomp.h"
#include "treeitem_generic.h"
#include "API/GUI/treeitem.h"
#include "API/GUI/component.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/surface.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeItem_Generic::CL_TreeItem_Generic(
	CL_TreeItem *self,
	CL_TreeNode *node)
: node(node), 
  sur_icon(0),
  delete_sur_icon(false),
  custom_height(0),
  text_margin(0),
  item(self)
{
	slot_mouse_down = item->sig_mouse_down().connect(
		this, &CL_TreeItem_Generic::on_mouse_down);

	items.reserve(8);
}

CL_TreeItem_Generic::~CL_TreeItem_Generic()
{
	if(sur_icon && delete_sur_icon)
		delete sur_icon;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_TreeItem_Generic::on_mouse_down(const CL_InputEvent &key)
{
//	item->sig_clicked()();
}
