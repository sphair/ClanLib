#include <ClanLib/display.h>
#include <ClanLib/core.h>

#include "player.h"
#include "scene.h"

Player::Player(Scene *player_scene)
{
	scene = player_scene;
	
	surface = CL_Surface("resources/pacman.tga");
	surface.set_alignment(origin_bottom_center);
	pos = CL_Point(100, 200);

	float scale = get_walk_map_depth(pos.x, pos.y);
	surface.set_scale(scale, scale);
	
	width = surface.get_width();
	height = surface.get_height();
	
	slot_key_down = CL_Keyboard::sig_key_down().connect(this, &Player::on_key_down);

	scene->add_object(this);
}

void Player::on_key_down(const CL_InputEvent &key)
{
	CL_Point new_pos = pos;
	
	switch(key.id)
	{
	case CL_KEY_UP:
		surface.set_angle(270.0f); 
		new_pos.y-=3;
		break;
	case CL_KEY_DOWN:
		surface.set_angle(90.0f); 
		new_pos.y+=3;
		break;
	case CL_KEY_LEFT:
		surface.set_angle(180.0f); 
		new_pos.x-=3;
		break;
	case CL_KEY_RIGHT:
		surface.set_angle(0.0f); 
		new_pos.x+=3;
		break;
	}
	
	if(check_walk_map(new_pos.x - width / 4, new_pos.y))
	{
		if(check_walk_map(new_pos.x + width / 4, new_pos.y))
		{
			z_value = pos.y;
			pos = new_pos;
			float scale = get_walk_map_depth(new_pos.x, new_pos.y);
			surface.set_scale(scale, scale); 
			
			scene->update_objects();
		}
	}
}

bool Player::check_walk_map(int x, int y)
{
	CL_PixelBuffer *walkmap = &(scene->walk_buffer);
	
	if(x < 0 || x >= walkmap->get_width() || y < 0 || y >= walkmap->get_height())
		return false;
		
	unsigned int *data = (unsigned int *) ((char *)walkmap->get_data() + y * walkmap->get_pitch());
	return ((data[x] & 0xff) != 0);
}

float Player::get_walk_map_depth(int x, int y)
{
	CL_PixelBuffer *walkmap = &(scene->walk_buffer);
	
	if(x < 0 || x >= walkmap->get_width() || y < 0 || y >= walkmap->get_height())
		return false;
		
	unsigned int *data = (unsigned int *) ((char *)walkmap->get_data() + y * walkmap->get_pitch());
	int color = data[x] >> 24;
	return (color / 100.0f);
}
