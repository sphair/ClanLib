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
**    Mark Page
*/

#pragma once

class CL_SharedGCData_Impl
{
public:
	CL_SharedGCData_Impl();
	~CL_SharedGCData_Impl();

	std::vector<CL_GraphicContextProvider*> &get_gc_providers();

	void dispose_objects();
	void add_disposable(CL_DisposableObject *disposable);
	void remove_disposable(CL_DisposableObject *disposable);

	CL_Signal_v0 &func_gc_destruction_imminent();
	CL_Texture load_texture(CL_GraphicContext &gc, const CL_String &filename, const CL_VirtualDirectory &virtual_directory, const CL_ImageImportDescription &import_desc);
	bool add_texture(CL_Texture &texture, const CL_String &filename, const CL_VirtualDirectory &virtual_directory, const CL_ImageImportDescription &import_desc);
	void unload_texture(const CL_String &filename, const CL_VirtualDirectory &virtual_directory, const CL_ImageImportDescription &import_desc);
	void unload_all_textures();

	int reference_count;

private:

	struct SharedTextureMap
	{
		SharedTextureMap(CL_String key, const CL_Texture &texture) : key(key), texture_impl(texture.get_impl())
		{
		}
		CL_String key;
		CL_WeakPtr<CL_Texture_Impl> texture_impl;
	};
	std::vector<SharedTextureMap> textures;

	CL_Signal_v0 sig_destruction_imminent;

	std::vector<CL_GraphicContextProvider*> graphic_context_providers;
	std::vector<CL_DisposableObject*> disposable_objects;

};
