#include "precomp.h"
#include "texturegroup_component.h"

TextureGroupComponent::TextureGroupComponent(CL_GUIComponent *parent)
: CL_GUIComponent(parent),
  texture_group(0)
{
	set_type_name("texturegroupcomponent");

	set_clip_children(true);

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
	invalidate_rect();
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
