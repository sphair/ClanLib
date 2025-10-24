/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Resources/resource_data_session.h"
#include "../../Core/Math/origin.h"
#include "color.h"
#include "../Image/image_import_description.h"
#include "../Render/texture.h"

class CL_GraphicContext;
class CL_VirtualDirectory;
class CL_ResourceManager;
class CL_Rect;
class CL_Size;
class CL_Rectf;
class CL_Image_Impl;
class CL_Texture;
class CL_Subtexture;
class CL_PixelBuffer;

/// \brief Image class.
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Image
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	CL_Image();

	/// \brief Constructs an image from a texture.
	///
	/// \param context = Graphic Context
	/// \param texture = Texture to get image data from
	/// \param rect = Position and size in texture to get image data from
	CL_Image(CL_GraphicContext &context, CL_Texture texture, CL_Rect rect);

	/// \brief Constructs an image from a subtexture.
	///
	/// \param context = Graphic Context
	/// \param sub_texture = Subtexture to get image data from
	CL_Image(CL_GraphicContext &context, CL_Subtexture &sub_texture);

	/// \brief Constructs a Image from a pixelbuffer.
	///
	/// \param gc = Graphic Context
	/// \param pixelbuffer = Pixelbuffer to get image data from
	/// \param rect = pixelbuffer rect
	CL_Image(CL_GraphicContext &gc, const CL_PixelBuffer &pixelbuffer, const CL_Rect &rect);

	/// \brief Constructs a Image
	///
	/// \param context = Graphic Context
	/// \param filename Filename of image to load
	/// \param import_desc = Image Import Description
	CL_Image(CL_GraphicContext &context, const CL_StringRef &filename, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	/// \brief Constructs a Image
	///
	/// \param context = Graphic Context
	/// \param filename Filename of image to load
	/// \param dir = Virtual directory to load filename from
	/// \param import_desc = Image Import Description
	CL_Image(CL_GraphicContext &context, const CL_StringRef &filename, CL_VirtualDirectory &dir, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	/// \brief Constructs a Image
	///
	/// \param context = Graphic Context
	/// \param resource_id Resource name of the image resource
	/// \param resources Resource manager used to load resource
	/// \param import_desc = Image Import Description
	CL_Image(CL_GraphicContext &context, const CL_StringRef &resource_id, CL_ResourceManager *resources, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	virtual ~CL_Image();	
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns x scale.
	/** <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>*/
	float get_scale_x() const;

	/// \brief Returns y scale.
	/** <p> 1.0f is normal scale, 2.0f is twice the size, etc. </p>*/
	float get_scale_y() const;

	/// \brief Returns current alpha.
	/** <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>*/
	float get_alpha() const;

	/// \brief Returns current color.
	/** <p> Alpha 0.0f is full transparency, and 1.0f is full visibility (solid). </p>*/
	CL_Colorf get_color() const;

	/// \brief Returns translation hot-spot.
	void get_alignment(CL_Origin &origin, int &x, int &y) const;

	/// \brief Get Size
	///
	/// \return size
	CL_Size get_size() const;

	/// \brief Return the width of the image.
	int get_width() const;

	/// \brief Return the height of the image.
	int get_height() const;

/// \}
/// \name Operators
/// \{
public:
	/// \brief Equality operator
	bool operator==(const CL_Image &other) const
	{
		return impl==other.impl;
	}

	/// \brief Inequality operator
	bool operator!=(const CL_Image &other) const
	{
		return impl!=other.impl;
	}

	/// \brief Less than operator
	bool operator<(const CL_Image &other) const
	{
		return impl < other.impl;
	}
/// \}

/// \name Operations
/// \{
public:
	/// \brief Draw image on graphic context.
	///
	/// \param x, y Anchor position of where to render image. Actual rendering position depends on the anchor and the alignment mode.
	/// \param gc Graphic context on which to render upon.
	void draw(
		CL_GraphicContext &gc,
		float x,
		float y) const;

	/// \brief Draw image on graphic context.
	///
	/// \param x, y Anchor position of where to render image. Actual rendering position depends on the anchor and the alignment mode.
	/// \param gc Graphic context on which to render upon.
	void draw(
		CL_GraphicContext &gc,
		int x,
		int y) const;

	/// \brief Draw image on graphic context.
	///
	/// \param gc Graphic context on which to render upon.
	/// \param src Source rectangle to draw. Use this is draw only part of the image.
	/// \param dest Rectangle to draw image in.
	void draw(
		CL_GraphicContext &gc,
		const CL_Rectf &src,
		const CL_Rectf &dest) const;

	/// \brief Draw image on graphic context.
	///
	/// \param gc Graphic context on which to render upon.
	/// \param dest Rectangle to draw image in.
	void draw(
		CL_GraphicContext &gc,
		const CL_Rectf &dest) const;

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

	/// \brief Sets translation hotspot.
	void set_alignment(CL_Origin origin, int x = 0, int y = 0);

	void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t);

	/// \brief Set to true if a linear filter should be used for scaling up and down, false if a nearest-point filter should be used.
	void set_linear_filter(bool linear_filter = true);

	/// \brief Upload image to sub texture.
	///
	/// \param image Image to upload.
	/// \param level Mipmap level-of-detail number.
	void set_subimage(
		int x,
		int y,
		const CL_PixelBuffer &image,
		const CL_Rect &src_rect,
		int level = 0);
/// \}

/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_Image_Impl> impl;
/// \}
};

/// \}
