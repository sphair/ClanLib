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
**    Kenneth Gangstoe
**    Mark Page
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../api_display.h"
#include <memory>
#include "../../Core/Math/origin.h"
#include "../../Core/Signals/signal_v0.h"
#include "../../Core/IOData/file_system.h"
#include "../Render/graphic_context.h"
#include "../Image/image_import_description.h"
#include "color.h"

namespace clan
{

class ResourceManager;
class SpriteDescription;
class Sprite_Impl;
class CollisionOutline;
class Canvas;

/// \brief Sprite class.
class CL_API_DISPLAY Sprite
{
/// \name Enums
/// \{
public:
	enum ShowOnFinish
	{
		show_blank, show_last_frame, show_first_frame
	};
/// \}

/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	Sprite();

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	/// \param fullname = String Ref
	Sprite(GraphicContext &gc, const std::string &fullname, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	/// \param filename = String Ref
	/// \param dir = Virtual Directory
	Sprite(GraphicContext &gc, const std::string &filename, const FileSystem &file_system, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	/// \param resource_id = String Ref
	/// \param resources = Resource Manager
	Sprite(GraphicContext &gc, const std::string &resource_id, ResourceManager *resources, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	/// \param file = IODevice
	/// \param image_type = String
	Sprite(GraphicContext &gc, IODevice &file, const std::string &image_type, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	/// \param description = Sprite Description
	Sprite(GraphicContext &gc, const SpriteDescription &description);

	virtual ~Sprite();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns current angle in degrees.
	Angle get_angle() const;

	/// \brief Returns the base angle in degrees - angle added to any rotation set with set_rotate() or rotate().
	Angle get_base_angle() const;

	/// \brief Returns scale for x and y.
	/** <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>*/
	void get_scale(float &x, float &y) const;

	/// \brief Returns current alpha.
	/** <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>*/
	float get_alpha() const;

	/// \brief Returns current color.
	/** <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>*/
	Colorf get_color() const;

	/// \brief Returns true if the sprite uses a linear filter for scaling up and down, false if a nearest-point filter is used.
	bool get_linear_filter() const;

	/// \brief Returns translation hot-spot.
	void get_alignment(Origin &origin, int &x, int &y) const;

	/// \brief Returns rotation hot-spot.
	void get_rotation_hotspot(Origin &origin, int &x, int &y) const;

	/// \brief Returns current frame in animation. 0 is first frame.
	int get_current_frame() const;

	/// \brief Returns number of frames in animation.
	int get_frame_count() const;

	/// \brief Returns the delay of a frame.
	///
	/// \param frame = The frame. 0 is first frame.
	/// \return The delay in milliseconds
	int get_frame_delay(int frame) const;

	/// \brief Returns the translation offset of a frame. 0 is first frame.
	Point get_frame_offset(int frame) const;

	/// \brief Returns the size of a frame. 0 is first frame.
	Size get_frame_size(int frame) const;

	/// \brief Return the width of the current frame, shortcut for 'get_frame_size(get_current_frame()).width'
	int get_width() const;

	/// \brief Return the height of the current frame, shortcut for 'get_frame_size(get_current_frame()).height'
	int get_height() const;

	/// \brief Return the size of the current frame, shortcut for 'get_frame_size(get_current_frame())'
	Size get_size() const;

	/// \brief Returns the attached id (if exists).
	int get_id() const;

	/// \brief Returns true if animation is played in loop (more than once).
	bool is_play_loop() const;

	/// \brief Returns true if animation is played in from right to left (starts at end).
	bool is_play_backward() const;

	/// \brief Returns true if animation is played in ping-pong mode.
	bool is_play_pingpong() const;

	/// \brief Returns an enum for what is shown when the animation is finished.
	/** <p> If show_blank is returned, nothing is shown when animation is done. </p>*/
	ShowOnFinish get_show_on_finish() const;

	/// \brief Returns true if animation is finished.
	/** <p> When an animation is finished, it will no longer show, unless ShowLastFrame is set
	    to anything other than show_blank </p>*/
	bool is_finished() const;

	/// \brief Returns true if animation has looped in the last update cycle
	bool is_looping() const;

/// \}

/// \name Operators
/// \{
public:
	/// \brief Equality operator
	bool operator==(const Sprite &other) const
	{
		return impl==other.impl;
	}

	/// \brief Inequality operator
	bool operator!=(const Sprite &other) const
	{
		return impl!=other.impl;
	}

	/// \brief Less than operator
	bool operator<(const Sprite &other) const
	{
		return impl < other.impl;
	}
/// \}
	
/// \name Operations
/// \{
public:
	/// \brief Copy assignment operator.
	Sprite &operator =(const Sprite &copy);

	/// \brief Sets the image data from another sprite, sharing animation state.
	/** Use this to change the look of your sprite.*/
	void set_image_data(const Sprite &image_source);

	/// \brief Copies all information from another sprite into this, not sharing animation state.
	/** This will not share animation state afterwards, like sprite1 = sprite2 would do. */
	Sprite clone() const;

	/// \brief Draw sprite on graphic context.
	///
	/// \param x, y Anchor position of where to render sprite. Actual rendering position depends on the anchor and the alignment mode.
	/// \param gc Graphic context on which to render upon. If null, will use Display's current graphic context.
	/// \param src Source rectangle to draw. Use this is draw only part of the sprite.
	/// \param dest Rectangle to draw sprite in.
	void draw(
		Canvas &canvas,
		float x,
		float y);

	void draw(
		Canvas &canvas,
		int x,
		int y);

	void draw(
		Canvas &canvas,
		const Rectf &src,
		const Rectf &dest);

	void draw(
		Canvas &canvas,
		const Rectf &dest);

	/// \brief Call this function to update the animation.
	///
	/// \param time_elapsed_ms = Time elapsed in milliseconds.
	void update(int time_elapsed_ms);

	/// \brief Set absolute rotation angle.
	void set_angle(Angle angle);

	/// \brief Add angle in degrees to current angle.
	void rotate(Angle angle);

	/// \brief Sets the base angle in degrees - angle added to any rotation set with set_rotate() or rotate().
	void set_base_angle(Angle angle);

	/// \brief Set scale for x and y directions individually.
	/** <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>*/
	void set_scale(float x, float y);

	/// \brief Sets transparency.
	/** <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>*/
	void set_alpha(float alpha);

	/// \brief Sets the color.
	/** <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>*/
	void set_color(const Colorf &color);

	/// \brief Set color
	///
	/// \param c = Color
	void set_color(const Color& c) {Colorf color; color.r = c.get_red() / 255.0f; color.g = c.get_green() / 255.0f; color.b = c.get_blue() / 255.0f; color.a = c.get_alpha() / 255.0f; set_color(color);}

	/// \brief Set to true if a linear filter should be used for scaling up and down, false if a nearest-point filter should be used.
	void set_linear_filter(bool linear_filter = true);

	/// \brief Sets translation hotspot.
	void set_alignment(Origin origin, int x = 0, int y = 0);

	/// \brief Sets rotation hotspot.
	void set_rotation_hotspot(Origin origin, int x = 0, int y = 0);

	/// \brief Sets current animation frame. 0 is first frame.
	/// \brief It will cap to available range.
	void set_frame(unsigned int frame);

	/// \brief Sets the delay for all frames.
	///
	/// \param delay_ms = Delay in milliseconds
	void set_delay(int delay_ms);

	/// \brief Sets the delay of a specific frame. 
	///
	/// \param frame = Frame number. 0 is first frame.
	/// \param delay_ms = Delay in milliseconds
	void set_frame_delay(int frame, int delay_ms);

	/// \brief Sets the translate offset of a specific frame. 0 is first frame.
	void set_frame_offset(int frame, Point offset);

	/// \brief Sets an attached id.
	void set_id(int id);

	/// \brief Finish animation.
	/** <p> When an animation is finished, it will no longer show, unless ShowLastFrame is set to anything
	    other than show_blank </p>*/
	void finish();

	/// \brief Restart animation.
	void restart();

	/// \brief Set to true if animation should loop, false otherwise.
	void set_play_loop(bool loop = true);

	/// \brief Set to true if animation should loop, false otherwise.
	void set_play_pingpong(bool pingpong = true);

	/// \brief Set to true if animation should go backward (start at end).
	void set_play_backward(bool backward = true);

	/// \brief Set what is shown when the animation is finished.
	void set_show_on_finish(Sprite::ShowOnFinish show_on_finish);
/// \}

/// \name Signals
/// \{
public:
	/// \brief Sig animation finished
	///
	/// \return Signal_v0
	Signal_v0 &sig_animation_finished();
/// \}

/// \name Implementation
/// \{
private:
	std::shared_ptr<Sprite_Impl> impl;
/// \}
};

}

/// \}
