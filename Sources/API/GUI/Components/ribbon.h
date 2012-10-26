/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#pragma once

#include "../gui_component.h"
#include "../gui_theme_part.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Core/Signals/callback_1.h"
#include "../../Display/Font/font.h"

namespace clan
{

class RibbonMenu;
class RibbonPage;
class RibbonSection;
class PushButton;

/// \brief Ribbon component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI Ribbon : public GUIComponent
{
/// \name Construction
/// \{
public:
	Ribbon(GUIComponent *container);
	~Ribbon();

/// \}
/// \name Attributes
/// \{
public:
	RibbonMenu *get_menu() { return menu; }

	Size get_preferred_size() const;

/// \}
/// \name Operations
/// \{
public:
	void add_page(RibbonPage *page);

/// \}
/// \name Implementation
/// \{
private:
	void on_menu_button_clicked();
	void on_render(Canvas &canvas, const Rect &clip_rect);
	void on_resized();
	bool on_input_pressed(const InputEvent &e);
	bool on_input_released(const InputEvent &e);
	bool on_input_pointer_moved(const InputEvent &e);
	void paint_tabs(Canvas &canvas, const Rect &clip_rect);

	PushButton *menu_button;
	RibbonMenu *menu;
	std::vector<RibbonPage *> pages;
	Font font_tab;
	GUIThemePart part_background;
	GUIThemePart part_tab;
	GUIThemePart part_tab_background;
	unsigned int current_page_index;
/// \}
};

}
