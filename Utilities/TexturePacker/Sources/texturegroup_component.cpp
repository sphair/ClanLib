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
*/

#include "precomp.h"
#include "texturegroup_component.h"

TextureGroupComponent::TextureGroupComponent(CL_GUIComponent *parent)
: CL_GUIComponent(parent),
  texture_group(0)
{
	set_type_name("texturegroupcomponent");

	component_texture = new CL_GUIComponent(this);

	scroll_horizontal = new CL_ScrollBar(this);
	scroll_horizontal->set_horizontal();
	scroll_vertical = new CL_ScrollBar(this);
	scroll_vertical->set_vertical();

	func_render().set(this, &TextureGroupComponent::on_render);
	func_style_changed().set(this, &TextureGroupComponent::on_style_changed);
	func_resized().set(this, &TextureGroupComponent::on_resized);

	component_texture->func_render().set(this, &TextureGroupComponent::on_render_texture);

	scroll_horizontal->func_scroll().set(this, &TextureGroupComponent::on_scroll);
	scroll_vertical->func_scroll().set(this, &TextureGroupComponent::on_scroll);

	on_resized();
}

void TextureGroupComponent::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Draw::fill(gc, CL_Rect(0, 0, get_size()), CL_Colorf::lightgray);
	set_clip_children(true);	// Note, this was moved out of the constructor because at that stage, the geometry is not set
}

void TextureGroupComponent::on_render_texture(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	if(texture_group)
	{
		std::vector<CL_Texture> textures = texture_group->get_textures();

		if(textures.size() > 0)
		{
			CL_Draw::fill(gc, CL_Rect(0, 0, component_texture->get_size()), CL_Colorf::black);

			gc.set_texture(0, textures[0]);
			int xpos = scroll_horizontal->get_position();
			int ypos = scroll_vertical->get_position();
			CL_Size texture_size = texture_group->get_texture_sizes();
			CL_Draw::texture(gc, CL_Rect(-xpos, -ypos, texture_size));
			gc.reset_texture(0);
		}
	}
}

void TextureGroupComponent::on_style_changed()
{
}

void TextureGroupComponent::on_scroll()
{
	request_repaint();
}

void TextureGroupComponent::on_resized()
{
	int scrollbar_size = scroll_horizontal->get_preferred_width();

	CL_Rect geometry = get_geometry();

	scroll_horizontal->set_geometry(CL_Rect(0, get_height() - scrollbar_size, get_width() - scrollbar_size, get_height()));
	scroll_vertical->set_geometry(CL_Rect(get_width() - scrollbar_size, 0, get_width(), get_height() - scrollbar_size));

	if(texture_group)
	{
		CL_Size texture_size = texture_group->get_texture_sizes();

		CL_Rect texture_area = CL_Rect(0, 0, geometry.get_width() - scrollbar_size, geometry.get_height() - scrollbar_size);
		if(texture_area.get_width() > texture_size.width)
			texture_area.set_width(texture_size.width);
		if(texture_area.get_height() > texture_size.height)
			texture_area.set_height(texture_size.height);
		component_texture->set_geometry(texture_area);

		if(geometry.get_width() < texture_size.width)
		{
			int scrollbar_range = texture_size.width - geometry.get_width() + scrollbar_size;
			scroll_horizontal->set_enabled(true);
			scroll_horizontal->set_ranges(0, scrollbar_range, 1, 1);
		}
		else
		{
			scroll_horizontal->set_enabled(false);
			scroll_horizontal->set_position(0);
		}

		if(geometry.get_height() < texture_size.height)
		{
			int scrollbar_range = texture_size.height - geometry.get_height() + scrollbar_size;
			scroll_vertical->set_enabled(true);
			scroll_vertical->set_ranges(0, scrollbar_range, 1, 1);
		}
		else
		{
			scroll_vertical->set_enabled(false);
			scroll_vertical->set_position(0);
		}
	}
	else
	{
		CL_Rect texture_area = CL_Rect(0, 0, geometry.get_width() - scrollbar_size, geometry.get_height() - scrollbar_size);
		component_texture->set_geometry(texture_area);

		scroll_horizontal->set_enabled(false);
		scroll_horizontal->set_position(0);
		scroll_vertical->set_enabled(false);
		scroll_vertical->set_position(0);
	}
}

void TextureGroupComponent::set_texturegroup(CL_TextureGroup *group)
{	
	texture_group = group;

	on_resized();
}
