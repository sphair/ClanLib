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
**    Magnus Norddahl
**    Harry Storbacka
**    Mark Page
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/IOData/virtual_directory.h"
#include "../../Core/Signals/signal_v0.h"
#include "texture.h"
#include "../Image/image_import_description.h"
#include <vector>
#include <map>

namespace clan
{

class GraphicContext;
class DisposableObject;
class SharedGCData_Impl;
class MutexSection;
class FontManager;

/// \brief Shared Graphic Context Data
class SharedGCData
{
public:

	/// \brief Add ref
	static void add_ref();

	/// \brief Release ref
	static void release_ref();

	/// \brief Add a provider
	static void add_provider(GraphicContextProvider *provider);

	/// \brief Remove a provider
	static void remove_provider(GraphicContextProvider *provider);

	/// \brief Get a provider
	///
	/// \param mutex_section : Returns the mutex section for the provider lock
	///
	/// \return NULL = None available
	static GraphicContextProvider *get_provider(std::unique_ptr<MutexSection> &mutex_section);

	/// \brief Get the providers
	///
	/// \param mutex_section : Returns the mutex section for the provider lock
	///
	/// \return NULL = None available
	static std::vector<GraphicContextProvider*> &get_gc_providers(std::unique_ptr<MutexSection> &mutex_section);

	/// \brief Add disposable
	///
	/// \param disposable = Disposable Object
	static void add_disposable(DisposableObject *disposable);

	/// \brief Remove disposable
	///
	/// \param disposable = Disposable Object
	static void remove_disposable(DisposableObject *disposable);

	/// \brief Get texture
	///
	/// \param gc = Graphic Context
	/// \param hash = hash
	///
	/// \return Texture ( test with .isnull() )
	static Texture get_texture(const std::string &hash);

	/// \brief Add texture to the shared texture cache
	///
	/// \param texture = Texture
	/// \param hash = hash
	static void add_texture(Texture &texture, const std::string &hash);

	/// \brief Remove texture to the shared texture cache
	///
		/// \param texture = Texture
	static void remove_texture(Texture &texture);

	/// \brief Remove and expired textures from the shared texture cache
	///
	/// This function should only be used by the Texture_Impl destructor
	static void remove_expired_texture();

	/// \brief Remove texture to the shared texture cache
	///
	/// \param hash = hash
	static void remove_texture(const std::string &hash);

	/// \brief Unload all textures
	static void unload_all_textures();

	/// \brief Get the shared font manager
	static FontManager get_font_manager();

private:
	SharedGCData();
	~SharedGCData();

	std::shared_ptr<SharedGCData_Impl> impl;
};

}

/// \}

