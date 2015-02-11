/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Mark Page
*/

#include "Display/precomp.h"
#include "setup_display.h"
#include "API/Display/ImageProviders/provider_type_register.h"
#include "API/Display/ImageProviders/targa_provider.h"
#include "API/Display/ImageProviders/jpeg_provider.h"
#include "API/Display/ImageProviders/png_provider.h"
#include "API/Display/Resources/display_cache.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/xml_resource_manager.h"
#include "API/Core/Resources/xml_resource_document.h"
#include "API/Core/Resources/file_resource_manager.h"
#include "API/Core/Resources/file_resource_document.h"
#include "Display/Resources/XML/xml_display_cache.h"
#include "Display/Resources/file_display_cache.h"
#include "../Core/System/setup_core.h"

#ifndef WIN32
#ifndef __APPLE__
#include <X11/Xlib.h>
#endif
#endif

namespace clan
{

	class SetupDisplay_Impl : public SetupModule
	{
	public:
		SetupDisplay_Impl();
		virtual ~SetupDisplay_Impl();

		static void add_cache_factory_xml(ResourceManager &manager, const XMLResourceDocument &doc);
		static void add_cache_factory_file(ResourceManager &manager, const FileResourceDocument &doc);

		std::unique_ptr<ProviderType_Register<JPEGProvider> > jpeg_provider;
		std::unique_ptr<ProviderType_Register<JPEGProvider> > jpg_provider;
		std::unique_ptr<ProviderType_Register<PNGProvider> > png_provider;
		std::unique_ptr<ProviderType_Register<TargaProvider> > targa_provider;
		std::unique_ptr<ProviderType_Register<TargaProvider> > tga_provider;
	};

	/////////////////////////////////////////////////////////////////////////////
	// SetupDisplay Construction:

	void SetupDisplay::start()
	{
		std::lock_guard<std::recursive_mutex> lock(SetupCore::instance.mutex);

		if (SetupCore::instance.module_display)
			return;

		SetupCore::start();	// Display depends on core.
		SetupCore::instance.module_display = clan::make_unique<SetupDisplay_Impl>();
	}


	SetupDisplay_Impl::SetupDisplay_Impl()
	{
#ifdef WIN32
		SetProcessDPIAware();
#endif

#ifndef WIN32
#ifndef __APPLE__
		// The XInitThreads() function initializes Xlib support for concurrent threads.
		// This function must be the first Xlib function a multi-threaded program calls, and it must complete before any other Xlib call is made.
		XInitThreads();
#endif
#endif
		jpeg_provider = clan::make_unique<ProviderType_Register<JPEGProvider> >("jpeg");
		jpg_provider = clan::make_unique<ProviderType_Register<JPEGProvider> >("jpg");
		png_provider = clan::make_unique<ProviderType_Register<PNGProvider> >("png");
		targa_provider = clan::make_unique<ProviderType_Register<TargaProvider> >("targa");
		tga_provider = clan::make_unique<ProviderType_Register<TargaProvider> >("tga");

		XMLResourceManager::add_cache_factory(std::function<void(ResourceManager &, const XMLResourceDocument &)>(&SetupDisplay_Impl::add_cache_factory_xml));
		FileResourceManager::add_cache_factory(std::function<void(ResourceManager &, const FileResourceDocument &)>(&SetupDisplay_Impl::add_cache_factory_file));
	}

	SetupDisplay_Impl::~SetupDisplay_Impl()
	{
	}

	void SetupDisplay_Impl::add_cache_factory_xml(ResourceManager &manager, const XMLResourceDocument &doc)
	{
		DisplayCache::set(manager, std::shared_ptr<DisplayCache>(new XMLDisplayCache(doc)));
	}

	void SetupDisplay_Impl::add_cache_factory_file(ResourceManager &manager, const FileResourceDocument &doc)
	{
		DisplayCache::set(manager, std::shared_ptr<DisplayCache>(new FileDisplayCache(doc)));
	}

}

