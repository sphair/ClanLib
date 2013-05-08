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
*/

#include "Display/precomp.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/disposable_object.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/Font/font_manager.h"
#include <algorithm>
#include "shared_gc_data_impl.h"

namespace clan
{

SharedGCData_Impl::SharedGCData_Impl() : reference_count(0)
{
}

SharedGCData_Impl::~SharedGCData_Impl()
{
}

void SharedGCData_Impl::unload_all_textures()
{
	textures.clear();
}

std::vector<GraphicContextProvider*> &SharedGCData_Impl::get_gc_providers()
{
	return graphic_context_providers;
}

void SharedGCData_Impl::add_provider(GraphicContextProvider *provider)
{
	graphic_context_providers.push_back(provider);
}
void SharedGCData_Impl::remove_provider(GraphicContextProvider *provider)
{
	if (graphic_context_providers.size() == 1)
		dispose_objects();

	for (std::vector<GraphicContextProvider*>::iterator it=graphic_context_providers.begin(); it != graphic_context_providers.end(); ++it)
	{
		if (provider == (*it))
		{
			graphic_context_providers.erase(it);
			break;
		}
	}
}

GraphicContextProvider *SharedGCData_Impl::get_provider()
{
	if (graphic_context_providers.empty())
		return NULL;
	return graphic_context_providers.back();
}

void SharedGCData_Impl::dispose_objects()
{
	font_manager.flush_cache();

	std::vector<DisposableObject*>::iterator it;
	for (it = disposable_objects.begin(); it != disposable_objects.end(); ++it)
		(*it)->dispose();

}

void SharedGCData_Impl::add_disposable(DisposableObject *disposable)
{
	disposable_objects.push_back(disposable);
}

void SharedGCData_Impl::remove_disposable(DisposableObject *disposable)
{
	std::vector<DisposableObject*>::iterator it;
	it = std::find(disposable_objects.begin(), disposable_objects.end(), disposable);
	if (it != disposable_objects.end())
		disposable_objects.erase(it);
}

void SharedGCData_Impl::add_texture(Texture &texture, const std::string &hash)
{
	// Search for an existing hash to replace
	for (std::vector<SharedTextureMap>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if (it->hash == hash)
		{
			it->texture_impl = texture.get_impl();
			return;
		}
	}

	textures.push_back(SharedTextureMap(hash, texture));

}

void SharedGCData_Impl::remove_texture(Texture &texture)
{
	Texture_Impl *texture_impl = texture.get_impl().lock().get();
	std::vector<SharedTextureMap>::iterator it;
	for (it=textures.begin(); it<textures.end(); ++it)
	{
		if ((*it).texture_impl.expired())
			throw Exception("Unexpected texture cache expiry");
		
		if ((*it).texture_impl.lock().get() == texture_impl)
		{
			textures.erase(it);
			break;
		}
	}
}

void SharedGCData_Impl::remove_expired_texture()
{
	std::vector<SharedTextureMap>::iterator it;
	for (it=textures.begin(); it<textures.end(); ++it)
	{
		if ((*it).texture_impl.expired())
		{
			textures.erase(it);
			break;
		}
	}
}

void SharedGCData_Impl::remove_texture(const std::string &hash)
{
	std::vector<SharedTextureMap>::iterator it;
	for (it=textures.begin(); it<textures.end(); ++it)
	{
		if ((*it).hash == hash)
		{
			textures.erase(it);
			break;
		}
	}
}
Texture SharedGCData_Impl::get_texture(const std::string &hash)
{
	std::vector<SharedTextureMap>::size_type i;
	for (i=0; i<textures.size(); i++)
	{
		if (textures[i].hash == hash)
		{
			std::shared_ptr<Texture_Impl> texture_impl = textures[i].texture_impl.lock();
			return Texture(texture_impl);
		}
	}
	return Texture();	// Not found
}

}
