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
**    Magnus Norddahl
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"

class CL_Size;
class CL_Rect;
class CL_Texture;
class CL_Subtexture;
class CL_TextureGroup_Impl;
class CL_GraphicContext;

/// \brief Texture grouping class.
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_TextureGroup
{
/// \name Enumerations
/// \{
public:
	/// \brief Texture allocation policy.
	enum TextureAllocationPolicy
	{
		create_new_texture,
		search_previous_textures
	};

/// \}
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance
	CL_TextureGroup();

	/// \brief Constructs a texture group
	CL_TextureGroup(CL_GraphicContext &context, const CL_Size &texture_sizes);

	~CL_TextureGroup();

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the amount of sub-textures allocated in group.
	int get_subtexture_count() const;

	/// \brief Returns the amount of sub-textures for a specific texture index.
	int get_subtexture_count(unsigned int texture_index) const;

	/// \brief Returns the amount of textures used by group.
	int get_texture_count() const;

	/// \brief Returns the texture allocation policy.
	TextureAllocationPolicy get_texture_allocation_policy() const;

	/// \brief Returns the size of the textures used by this texture group.
	CL_Size get_texture_sizes() const;

	/// \brief Returns the textures.
	std::vector<CL_Texture> get_textures() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Allocate space for another sub texture.
	CL_Subtexture add(CL_GraphicContext &context, const CL_Size &size);

	/// \brief Deallocate space, from a previously allocated texture
	///
	/// Warning - It is advised to set TextureAllocationPolicy to search_previous_textures
	/// if using this function.  Also be aware of texture fragmentation.
	/// Empty textures are not removed.
	void remove(CL_Subtexture &subtexture);

	/// \brief Set the texture allocation policy.
	void set_texture_allocation_policy(TextureAllocationPolicy policy);

	/// \brief Insert an existing texture into the texture group
	///
	/// \param texture = Texture to insert
	/// \param texture_rect = Free space within the texture that the texture group can use
	void insert_texture(CL_Texture &texture, const CL_Rect &texture_rect);

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_TextureGroup_Impl> impl;
/// \}
};

/// \}
