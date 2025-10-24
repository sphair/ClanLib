#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "checkbox_zz.h"
#include "stylemanager_zz.h"

CL_CheckBox_ZZ::CL_CheckBox_ZZ(
	CL_CheckBox *checkbox,
	CL_StyleManager_ZZ *style)
:
	CL_ComponentStyle(checkbox),
	style(style),
	checkbox(checkbox)
{
	resources = style->get_resources();

	font = new CL_Font("CheckBox/font", resources);

	surface_left = new CL_Surface("CheckBox/left", resources);
	surface_right = new CL_Surface("CheckBox/right", resources);
	surface_ticker = new CL_Surface("CheckBox/ticker", resources);

	slot_paint = checkbox->sig_paint().connect_virtual(
		this, &CL_CheckBox_ZZ::on_paint);
	slot_get_preferred_size = checkbox->sig_get_preferred_size().connect(
		this, &CL_CheckBox_ZZ::on_get_preferred_size);
}

CL_CheckBox_ZZ::~CL_CheckBox_ZZ()
{
}

void CL_CheckBox_ZZ::on_get_preferred_size(CL_Size &size)
{
	size.width = font->get_width(checkbox->get_text()) + 28;
	size.height = 35;
}

void CL_CheckBox_ZZ::on_paint(CL_SlotParent_v0 &super)
{
	surface_left->draw(0, 0);
	surface_right->draw(16, 0);

	int checkbox_width = checkbox->get_width();
	int checkbox_height = checkbox->get_height();

	if(checkbox->is_down())
		surface_ticker->draw(-3, -10);

	int x_pos = 28;
	int text_height = font->get_height();
	int y_pos = (checkbox_height - text_height) / 2 - 4;

	font->draw(checkbox->get_screen_x() + x_pos, checkbox->get_screen_x() + y_pos, checkbox->get_text());
}
