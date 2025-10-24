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
**    Magnus Norddahl
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/disposable_object.h"
#include "API/Display/Render/shared_gc_data.h"
#include <algorithm>

#include "shared_gc_data_impl.h"

CL_SharedGCData_Impl::CL_SharedGCData_Impl() : reference_count(0)
{
}

CL_SharedGCData_Impl::~CL_SharedGCData_Impl()
{
}

CL_Signal_v0 &CL_SharedGCData_Impl::func_gc_destruction_imminent()
{
	return sig_destruction_imminent;
}

CL_Texture CL_SharedGCData_Impl::load_texture(CL_GraphicContext &gc, const CL_String &filename, const CL_VirtualDirectory &virtual_directory, const CL_ImageImportDescription &import_desc)
{
	CL_String alpha_code = "A";
	if (import_desc.get_premultiply_alpha())
		alpha_code = "P";

	CL_String key = virtual_directory.get_identifier() + filename + alpha_code;

	std::vector<SharedTextureMap>::size_type i;
	for (i=0; i<textures.size(); i++)
	{
		if (textures[i].key == key)
		{
			// Texture is no longer valid
			if (textures[i].texture_impl.expired())
			{
				// Remove from the cache
				unload_texture(filename, virtual_directory, import_desc);
				break;
			}
			CL_SharedPtr<CL_Texture_Impl> texture_impl = textures[i].texture_impl.lock();
			return CL_Texture(texture_impl);
		}
	}

	CL_Texture texture = CL_Texture(gc, filename, virtual_directory, import_desc);
	textures.push_back(SharedTextureMap(key, texture));
	return texture;
}

bool CL_SharedGCData_Impl::add_texture(CL_Texture &texture, const CL_String &filename, const CL_VirtualDirectory &virtual_directory, const CL_ImageImportDescription &import_desc)
{
	CL_String alpha_code = "A";
	if (import_desc.get_premultiply_alpha())
		alpha_code = "P";

	CL_String key = virtual_directory.get_identifier() + filename + alpha_code;

	for (std::vector<SharedTextureMap>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if (it->key == key)
		{
			// Texture is no longer valid
			if (it->texture_impl.expired())
			{
				// Remove from the cache
				unload_texture(filename, virtual_directory, import_desc);
				break;
			}
			return false;
		}
	}

	textures.push_back(SharedTextureMap(key, texture));
	return true;
}

void CL_SharedGCData_Impl::unload_texture(const CL_String &filename, const CL_VirtualDirectory &virtual_directory, const CL_ImageImportDescription &import_desc)
{
	CL_String alpha_code = "A";
	if (import_desc.get_premultiply_alpha())
		alpha_code = "P";

	CL_String key = virtual_directory.get_identifier() + filename + alpha_code;

	std::vector<SharedTextureMap>::iterator it;
	for (it=textures.begin(); it<textures.end(); ++it)
	{
		if ((*it).key == key)
		{
			textures.erase(it);
			break;
		}
	}
}

void CL_SharedGCData_Impl::unload_all_textures()
{
	textures.clear();
}

std::vector<CL_GraphicContextProvider*> &CL_SharedGCData_Impl::get_gc_providers()
{
	return graphic_context_providers;
}

void CL_SharedGCData_Impl::dispose_objects()
{
	std::vector<CL_DisposableObject*>::iterator it;
	for (it = disposable_objects.begin(); it != disposable_objects.end(); ++it)
		(*it)->dispose();

}

void CL_SharedGCData_Impl::add_disposable(CL_DisposableObject *disposable)
{
	disposable_objects.push_back(disposable);
}

void CL_SharedGCData_Impl::remove_disposable(CL_DisposableObject *disposable)
{
	std::vector<CL_DisposableObject*>::iterator it;
	it = std::find(disposable_objects.begin(), disposable_objects.end(), disposable);
	if (it != disposable_objects.end())
		disposable_objects.erase(it);
}
