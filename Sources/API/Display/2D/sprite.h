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
**    Kenneth Gangstoe
**    Mark Page
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/Text/string_types.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Math/origin.h"
#include "../../Core/Signals/signal_v0.h"
#include "../../Core/IOData/virtual_directory.h"
#include "../../Core/Resources/resource_data_session.h"
#include "../Render/graphic_context.h"
#include "../Render/blend_mode.h"
#include "../Image/image_import_description.h"

#include "color.h"

class CL_ResourceManager;
class CL_SpriteDescription;
class CL_Sprite_Impl;

/// \brief Sprite class.
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Sprite
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
	CL_Sprite();

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	CL_Sprite(CL_GraphicContext &gc);

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	/// \param fullname = String Ref
	CL_Sprite(CL_GraphicContext &gc, const CL_StringRef &fullname, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	/// \param filename = String Ref
	/// \param dir = Virtual Directory
	CL_Sprite(CL_GraphicContext &gc, const CL_StringRef &filename, CL_VirtualDirectory &dir, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	/// \param resource_id = String Ref
	/// \param resources = Resource Manager
	CL_Sprite(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	/// \param file = IODevice
	/// \param image_type = String
	CL_Sprite(CL_GraphicContext &gc, CL_IODevice &file, const CL_String &image_type, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	/// \brief Constructs a Sprite
	///
	/// \param gc = Graphic Context
	/// \param description = Sprite Description
	CL_Sprite(CL_GraphicContext &gc, const CL_SpriteDescription &description);

	virtual ~CL_Sprite();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns current angle in degrees.
	CL_Angle get_angle() const;

	/// \brief Returns the base angle in degrees - angle added to any rotation set with set_rotate() or rotate().
	CL_Angle get_base_angle() const;

	/// \brief Returns scale for x and y.
	/** <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>*/
	void get_scale(float &x, float &y) const;

	/// \brief Returns current alpha.
	/** <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>*/
	float get_alpha() const;

	/// \brief Returns current color.
	/** <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>*/
	CL_Colorf get_color() const;

	/// \brief Returns true if the sprite uses a linear filter for scaling up and down, false if a nearest-point filter is used.
	bool get_linear_filter() const;

	/// \brief Returns translation hot-spot.
	void get_alignment(CL_Origin &origin, int &x, int &y) const;

	/// \brief Returns rotation hot-spot.
	void get_rotation_hotspot(CL_Origin &origin, int &x, int &y) const;

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
	CL_Point get_frame_offset(int frame) const;

	/// \brief Returns the size of a frame. 0 is first frame.
	CL_Size get_frame_size(int frame) const;

	/// \brief Return the width of the current frame, shortcut for 'get_frame_size(get_current_frame()).width'
	int get_width() const;

	/// \brief Return the height of the current frame, shortcut for 'get_frame_size(get_current_frame()).height'
	int get_height() const;

	/// \brief Return the size of the current frame, shortcut for 'get_frame_size(get_current_frame())'
	CL_Size get_size() const;

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
	bool operator==(const CL_Sprite &other) const
	{
		return impl==other.impl;
	}

	/// \brief Inequality operator
	bool operator!=(const CL_Sprite &other) const
	{
		return impl!=other.impl;
	}

	/// \brief Less than operator
	bool operator<(const CL_Sprite &other) const
	{
		return impl < other.impl;
	}
	/// \}
	
/// \name Operations
/// \{
public:
	/// \brief Copy assignment operator.
	CL_Sprite &operator =(const CL_Sprite &copy);

	/// \brief Sets the image data from another sprite, sharing animation state.
	/** Use this to change the look of your sprite.*/
	void set_image_data(const CL_Sprite &image_source);

	/// \brief Copies all information from another sprite into this, not sharing animation state.
	/** This will not share animation state afterwards, like sprite1 = sprite2 would do. */
	void clone(const CL_Sprite &source);

	/// \brief Draw sprite on graphic context.
	///
	/// \param x, y Anchor position of where to render sprite. Actual rendering position depends on the anchor and the alignment mode.
	/// \param gc Graphic context on which to render upon. If null, will use CL_Display's current graphic context.
	/// \param src Source rectangle to draw. Use this is draw only part of the sprite.
	/// \param dest Rectangle to draw sprite in.
	void draw(
		CL_GraphicContext &gc,
		float x,
		float y);

	void draw(
		CL_GraphicContext &gc,
		int x,
		int y);

	void draw(
		CL_GraphicContext &gc,
		const CL_Rectf &src,
		const CL_Rectf &dest);

	void draw(
		CL_GraphicContext &gc,
		const CL_Rectf &dest);

	/// \brief Call this function to update the animation.
	///
	/// \param time_elapsed_ms = Time elapsed in milliseconds. Use -1 for automatic time calculation
	/// \return time_elapsed milliseconds since last update
	int update(int time_elapsed_ms = -1);

	/// \brief Set absolute rotation angle.
	void set_angle(CL_Angle angle);

	/// \brief Set absolute rotation pitch angle.
	void set_angle_pitch(CL_Angle angle);

	/// \brief Set absolute rotation yaw angle.
	void set_angle_yaw(CL_Angle angle);

	/// \brief Add angle in degrees to current angle.
	void rotate(CL_Angle angle);

	/// \brief Add angle in degrees to current pitch angle.
	void rotate_pitch(CL_Angle angle);

	/// \brief Add angle in degrees to current yaw angle.
	void rotate_yaw(CL_Angle angle);

	/// \brief Sets the base angle in degrees - angle added to any rotation set with set_rotate() or rotate().
	void set_base_angle(CL_Angle angle);

	/// \brief Set scale for x and y directions individually.
	/** <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>*/
	void set_scale(float x, float y);

	/// \brief Sets transparency.
	/** <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>*/
	void set_alpha(float alpha);

	/// \brief Sets the color.
	/** <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>*/
	void set_color(const CL_Colorf &color);

	/// \brief Set color
	///
	/// \param c = Color
	void set_color(const CL_Color& c) {CL_Colorf color; color.r = c.get_red() / 255.0f; color.g = c.get_green() / 255.0f; color.b = c.get_blue() / 255.0f; color.a = c.get_alpha() / 255.0f; set_color(color);}

	/// \brief Set to true if a linear filter should be used for scaling up and down, false if a nearest-point filter should be used.
	void set_linear_filter(bool linear_filter = true);

	/// \brief Sets translation hotspot.
	void set_alignment(CL_Origin origin, int x = 0, int y = 0);

	/// \brief Sets rotation hotspot.
	void set_rotation_hotspot(CL_Origin origin, int x = 0, int y = 0);

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
	void set_frame_offset(int frame, CL_Point offset);

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
	void set_show_on_finish(CL_Sprite::ShowOnFinish show_on_finish);
/// \}

/// \name Signals
/// \{
public:

	/// \brief Sig animation finished
	///
	/// \return Signal_v0
	CL_Signal_v0 &sig_animation_finished();
/// \}

/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_Sprite_Impl> impl;
/// \}
};

/// \}
