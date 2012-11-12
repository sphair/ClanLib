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
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"

namespace clan
{

class Sprite;
class PixelBuffer;
class ImageView_Impl;

/// \brief Frame component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI ImageView : public GUIComponent
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a ImageView
	///
	/// \param parent = GUIComponent
	ImageView(GUIComponent *parent);

	virtual ~ImageView();

/// \}
/// \name Attributes
/// \{
public:
	using GUIComponent::get_named_item;

	/// \brief Find the child ImageView with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static ImageView *get_named_item(GUIComponent *reference_component, const std::string &id);

	/// \brief Get Sprite
	///
	/// \return sprite
	Sprite get_sprite() const;

	/// \brief Get Image
	///
	/// \return image
	Image get_image() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Set sprite
	///
	/// \param image = Sprite
	void set_sprite(const Sprite &image);

	/// \brief Set image
	///
	/// \param image = Sprite
	void set_image(const Image &image);

	/// \brief Set image
	///
	/// \param image = Pixel Buffer
	void set_image(const PixelBuffer &image);

	/// \brief Set scale to fit
	void set_scale_to_fit();

	/// \brief Set scale
	///
	/// \param x = value
	/// \param y = value
	void set_scale(float x, float y);

/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<ImageView_Impl> impl;
/// \}
};

}

/// \}
