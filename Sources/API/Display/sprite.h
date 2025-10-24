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

//! clanDisplay="Sprites"
//! header=display.h

#ifndef header_sprite
#define header_sprite

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "surface.h"
#include "blend_func.h"
#include "../Core/Math/origin.h"

#include "../Core/Math/size.h"
#include "../Core/Math/rect.h"
#include "../Core/Resources/resource.h"

class CL_ResourceManager;
class CL_SpriteDescription;
class CL_Sprite_Generic;
class CL_GraphicContext;

//: This class handles displaying sprites on the screen. 
//- !group=Display/Sprites!
//- !header=display.h!
//- <p>There are two overviews describing the sprite system in ClanLib.
//- The <a href="../../Overview/sprites_overview.html">sprites overview</a>, and the
//- <a href="../../Overview/sprites_resources.html">sprites resources overview.</a></p>
class CL_API_DISPLAY CL_Sprite
{ 
//! Enums:
public:
	enum ShowOnFinish
	{
		show_blank, show_last_frame, show_first_frame
	};
	
//! Construction:
public:
	//: Constructs a sprite.
	//param resource_id: Resource name of a sprite resource.
	//param manager: Resource manager used to load resource.
	//param sprite_description: Sprite description used to construct sprite.
	//param pack_texture: If true, will try to pack sprite into as few textures as possible.
	CL_Sprite(
		const std::string &resource_id,
		CL_ResourceManager *manager);

	CL_Sprite(const CL_SpriteDescription &sprite_description, bool pack_texture = true);

	CL_Sprite(const CL_SpriteDescription &sprite_description, bool pack_texture, unsigned int min_width, unsigned int min_height);

	CL_Sprite(const CL_Sprite &sprite);

	CL_Sprite();

	virtual ~CL_Sprite();

//! Attributes:
public:
	//: Returns current angle in degrees.
	float get_angle() const;

	//: Returns the current rotational yaw angle in degrees.
	float get_angle_yaw() const;

	//: Returns the current rotational pitch angle in degrees.
	float get_angle_pitch() const;

	//: Returns the base angle in degrees - angle added to any rotation set with set_rotate() or rotate().
	float get_base_angle() const;

	//: Returns scale for x and y.
	//- <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>
	void get_scale(float &x, float &y) const;

	//: Returns current alpha.
	//- <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>
	float get_alpha() const;

	//: Returns current color.
	//- <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>
	void get_color(float &red, float &green, float &blue, float &alpha) const;
	CL_Color get_color() const {float r,g,b,a; get_color(r,g,b,a); return CL_Color(int(r*255.0f),int(g*255.0f),int(b*255.0f),int(a*255.0f));}

	//: Returns blending functions.
	void get_blend_func(CL_BlendFunc &src, CL_BlendFunc &dest) const;

	//: Returns translation hotspot.
	void get_alignment(CL_Origin &origin, int &x, int &y) const;

	//: Returns rotation hotspot.
	void get_rotation_hotspot(CL_Origin &origin, int &x, int &y) const;

	//: Returns current frame in animation. 0 is first frame.
	int get_current_frame() const;

	//: Returns number of frames in animation.
	int get_frame_count() const;

	//: Returns the delay of a frame. 0 is first frame.
	float get_frame_delay(int frame) const;

	//: Returns the translation offset of a frame. 0 is first frame.
	CL_Point get_frame_offset(int frame) const;
	
	//: Returns the size of a frame. 0 is first frame.
	CL_Size get_frame_size(int frame) const;

	//: Return the width of the current frame, shortcut for
	//: 'get_frame_size(get_current_frame()).width'
	int get_width() const;

	//: Return the height of the current frame, shortcut for
	//: 'get_frame_size(get_current_frame()).height'
	int get_height() const;

	//: Returns the surface of a frame. 0 is first frame.
	//- <p> Returns an empty surface if frame is invalid. </p>
	CL_Surface get_frame_surface(int frame) const;

	//: Returns the surface of a frame and its position. 0 is first frame.
	//- <p> Returns an empty surface if frame is invalid. </p>
	CL_Surface get_frame_surface(int frame, CL_Rect &surface_rect) const;

	//: Returns a CL_PixelBuffer with the same dimensions
	//: as this sprite.
	//- <p> Returns an empty pixelbuffer if frame is invalid. </p>
	CL_PixelBuffer get_frame_pixeldata(int frame) const;

	//: Returns the attached id (if exists).
	int get_id() const;
	
	//: Returns true if this is an unattached sprite.
	bool is_null() const;
	
	//: Returns true if animation is played in loop (more than once).
	bool is_play_loop() const;

	//: Returns true if animation is played in from right to left (starts at end).
	bool is_play_backward() const;

	//: Returns true if animation is played in ping-pong mode.
	bool is_play_pingpong() const;

	//: Returns an enum for what is shown when the animation is finished.
	//- <p> If show_blank is returned, nothing is shown when animation is done. </p>
	ShowOnFinish get_show_on_finish() const;

	//: Returns true if animation is finished.
	//- <p> When an animation is finished, it will no longer show, unless ShowLastFrame is set
	//- to anything other than show_blank </p>
	bool is_finished() const;

	//: Returns true if animation has looped in the last update cycle
	bool is_looping() const;

	//: Resource owning this sprite, if any.
	CL_Resource resource;

//! Operations:
public:
	//: Copy assignment operator.
	CL_Sprite &operator =(const CL_Sprite &copy);

	//: Return true if the CL_Sprite is valid and useable
	operator bool () const;

	//: Sets the image data from another sprite.
	//- Use this to change the look of your sprite.
	void set_image_data(const CL_Sprite &image_source);
	
	//: Draw sprite on graphic context.
	//param x, y: Anchor position of where to render sprite. Actual rendering position depends on the anchor and the alignment mode.
	//param gc: Graphic context on which to render upon. If null, will use CL_Display's current graphic context.
	//param dest: Rectangle to draw sprite in.
	void draw(
		float x,
		float y,
		CL_GraphicContext *gc = 0);

	void draw(
		const CL_Rect &dest,
		CL_GraphicContext *gc = 0);

	//: sub pixel accuracy versions

	void draw(
		const CL_Rectf &dest,
		CL_GraphicContext *gc = 0);

	void draw(const CL_Surface_DrawParams1 & params1,
		CL_GraphicContext *gc = 0);

	void draw_subpixel(
		float x,
		float y,
		CL_GraphicContext *gc = 0);


	//: Calculate draw information and send it back.  Returns false if sprite isn't currently drawn
	//- This allows you to tweak the settings and call draw() for specific effects.

	bool setup_draw_params(float x, float y, CL_Surface_DrawParams1 & params1, bool sub_pixel_accuracy);
	
	//: Call this function to update the animation.
	//- Returns the time elapsed parameter, the automatic calculated one if you used 0.
	//param time_elapsed: milliseconds since last update. Use 0 for automatic time calculation.
	float update(float time_elapsed = 0);

	//: Set absolute rotation angle in degrees.
	void set_angle(float angle);
	
	//: Set absolute rotation pitch angle in degrees.
	void set_angle_pitch(float angle);
	
	//: Set absolute rotation yaw angle in degrees.
	void set_angle_yaw(float angle);

	//: Add angle in degrees to current angle.
	void rotate(float angle);
	
	//: Add angle in degrees to current pitch angle.
	void rotate_pitch(float angle);
	
	//: Add angle in degrees to current yaw angle.
	void rotate_yaw(float angle);

	//: Sets the base angle in degrees - angle added to any rotation set with set_rotate() or rotate().
	void set_base_angle(float angle);

	//: Set scale for x and y directions individually.
	//- <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>
	void set_scale(float x, float y);

	//: Sets transparency.
	//- <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>
	void set_alpha(float alpha);

	//: Sets the color.
	//- <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>
	void set_color(float r, float g, float b, float a = 1.0f);
	void set_color(const CL_Color& c) {set_color(float(c.get_red())/255.0f,float(c.get_green())/255.0f,float(c.get_blue())/255.0f,float(c.get_alpha())/255.0f);}

	//: Sets blending functions.
	void set_blend_func(CL_BlendFunc src, CL_BlendFunc dest);

	//: Sets blending functions.
	void set_blend_func_separate(CL_BlendFunc src, CL_BlendFunc dest,
                                     CL_BlendFunc src_alpha, CL_BlendFunc dest_alpha);

	//: Sets translation hotspot.
	void set_alignment(CL_Origin origin, int x = 0, int y = 0);

	//: Sets rotation hotspot.
	void set_rotation_hotspot(CL_Origin origin, int x = 0, int y = 0);

	//: Sets current animation frame. 0 is first frame.
	//: It will cap to available range.
	void set_frame(unsigned int frame);

	//: Sets the delay of a specific frame. 0 is first frame.
	void set_frame_delay(int frame, float delay);

	//: Sets the translate offset of a specific frame. 0 is first frame.
	void set_frame_offset(int frame, CL_Point offset);
	
	//: Sets an attached id.
	void set_id(int id);
	
	//: Finish animation.
	//- <p> When an animation is finished, it will no longer show, unless ShowLastFrame is set to anything
	//- other than show_blank </p>
	void finish();

	//: Restart animation.
	void restart();

	//: Set to true if animation should loop, false otherwise.
	void set_play_loop(bool loop = true);

	//: Set to true if animation should loop, false otherwise.
	void set_play_pingpong(bool pingpong = true);

	//: Set to true if animation should go backward (start at end).
	void set_play_backward(bool backward = true);

	//: Set what is shown when the animation is finished.
	void set_show_on_finish(CL_Sprite::ShowOnFinish show_on_finish);

	//: Adds a frame to the animation
	void add_frame(CL_Surface surface, const CL_Rect& rect = CL_Rect(0,0,0,0), float delay = 0.06f, CL_Point offset = CL_Point(0, 0));

//! Signals:
public:
	CL_Signal_v0 &sig_animation_finished();
	
//! Implementation:
private:
	
	CL_Sprite_Generic *impl;

	void createFromDescription(const CL_SpriteDescription &spritedescription, bool pack_texture, unsigned int min_width, unsigned int min_height);
};

#endif
