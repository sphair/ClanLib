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
**    Magnus Norddahl
*/

#pragma once

#include "PropertyComponent/property_item_popup.h"

class AnchorPopup : public PropertyItemPopup
{
public:
	AnchorPopup(clan::ComponentAnchorPoint tl, clan::ComponentAnchorPoint br, clan::GUIComponent *parent);
	~AnchorPopup();

	clan::ComponentAnchorPoint get_anchor_tl() const { return cap_tl; }
	clan::ComponentAnchorPoint get_anchor_br() const { return cap_br; }

private:
	void on_resized();
	void on_anchoring_changed(clan::RadioButton *rb);

	clan::ComponentAnchorPoint cap_tl, cap_br;
	clan::Label *label_tl;
	clan::Label *label_br;
	clan::RadioButton *tl;
	clan::RadioButton *tr;
	clan::RadioButton *bl;
	clan::RadioButton *br;
	clan::RadioButton *scale;
	clan::RadioButton *br_tl;
	clan::RadioButton *br_tr;
	clan::RadioButton *br_bl;
	clan::RadioButton *br_br;
	clan::RadioButton *br_scale;
};
