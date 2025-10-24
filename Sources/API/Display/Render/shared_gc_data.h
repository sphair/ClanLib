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

class CL_GraphicContext;
class CL_DisposableObject;
class CL_SharedGCData_Impl;

/// \brief Shared Graphic Context Data
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_SharedGCData
{
public:

	/// \brief Add ref
	static void add_ref();

	/// \brief Release ref
	static void release_ref();

	/// \brief Get Instance
	///
	/// \return instance
	static CL_SharedGCData *get_instance();

	static std::vector<CL_GraphicContextProvider*> &get_gc_providers();

	/// \brief Dispose objects
	static void dispose_objects();

	/// \brief Add disposable
	///
	/// \param disposable = Disposable Object
	static void add_disposable(CL_DisposableObject *disposable);

	/// \brief Remove disposable
	///
	/// \param disposable = Disposable Object
	static void remove_disposable(CL_DisposableObject *disposable);

	/// \brief Func gc destruction imminent
	///
	/// \return Signal_v0
	static CL_Signal_v0 &func_gc_destruction_imminent();

	/// \brief Load texture
	///
	/// \param gc = Graphic Context
	/// \param filename = String
	/// \param virtual_directory = Virtual Directory
	///
	/// \return Texture
	static CL_Texture load_texture(CL_GraphicContext &gc, const CL_String &filename, const CL_VirtualDirectory &virtual_directory = CL_VirtualDirectory(), const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	/// \brief Unload texture
	///
	/// \param filename = String
	/// \param virtual_directory = Virtual Directory
	static void unload_texture(const CL_String &filename, const CL_VirtualDirectory &virtual_directory = CL_VirtualDirectory(), const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	/// \brief Unload all textures
	static void unload_all_textures();

private:
	CL_SharedGCData();
	~CL_SharedGCData();

	CL_SharedPtr<CL_SharedGCData_Impl> impl;
};

/// \}

