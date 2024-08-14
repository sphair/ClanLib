/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "API/Core/Resources/file_resource_manager.h"
#include "API/Core/Resources/file_resource_document.h"
#include "Display/Resources/file_display_cache.h"
#include "../Core/System/setup_core.h"

#ifdef WIN32
#include "Platform/Win32/display_message_queue_win32.h"
#elif defined(__APPLE__)
#include "Platform/Cocoa/display_message_queue_cocoa.h"
#elif !defined(__ANDROID__)
#include "Platform/X11/display_message_queue_x11.h"
#endif

#if !defined __ANDROID__ && ! defined __APPLE__ && ! defined WIN32
#include <X11/Xlib.h>
#endif

namespace clan
{
	class ImageProviderType;
	class SetupDisplay_Impl : public SetupModule
	{
	public:
		SetupDisplay_Impl();
		~SetupDisplay_Impl() override;

		static void add_cache_factory_file(ResourceManager &manager, const FileResourceDocument &doc);

		static SetupDisplay_Impl *instance;

		/// \brief Map of the class factories for each provider type.
		std::map<std::string, ImageProviderType *> image_provider_factory_types;

		ProviderType_Register<JPEGProvider> *jpeg_provider = nullptr;
		ProviderType_Register<JPEGProvider> *jpg_provider = nullptr;
		ProviderType_Register<PNGProvider> *png_provider = nullptr;
		ProviderType_Register<TargaProvider> *targa_provider = nullptr;
		ProviderType_Register<TargaProvider> *tga_provider = nullptr;

#if defined(WIN32)
		DisplayMessageQueue_Win32 message_queue;
#elif defined(__APPLE__)
		DisplayMessageQueue_Cocoa message_queue;
#elif !defined(__ANDROID__)
		DisplayMessageQueue_X11 message_queue;
#endif
	};
	SetupDisplay_Impl *SetupDisplay_Impl::instance = nullptr;

	void SetupDisplay::start()
	{
		std::lock_guard<std::recursive_mutex> lock(SetupCore::instance.mutex);

		if (SetupCore::instance.module_display)
			return;

		SetupCore::start();	// Display depends on core.
		SetupCore::instance.module_display = std::make_unique<SetupDisplay_Impl>();
	}

	SetupDisplay_Impl::SetupDisplay_Impl()
	{
		instance = this;
#if defined(WIN32) && !defined(__MINGW32__) // FIXME: Broken on MinGW (does not link)
		SetProcessDPIAware();
#endif

#if !defined __ANDROID__ && ! defined __APPLE__ && ! defined WIN32
		// The XInitThreads() function initializes Xlib support for concurrent threads.
		// This function must be the first Xlib function a multi-threaded program calls, and it must complete before any other Xlib call is made.
		XInitThreads();
#endif
		jpeg_provider = new ProviderType_Register<JPEGProvider>("jpeg");
		jpg_provider = new ProviderType_Register<JPEGProvider>("jpg");
		png_provider = new ProviderType_Register<PNGProvider>("png");
		targa_provider = new ProviderType_Register<TargaProvider>("targa");
		tga_provider = new ProviderType_Register<TargaProvider>("tga");

		FileResourceManager::add_cache_factory(std::function<void(ResourceManager &, const FileResourceDocument &)>(&SetupDisplay_Impl::add_cache_factory_file));
	}

	SetupDisplay_Impl::~SetupDisplay_Impl()
	{
		delete jpeg_provider;
		delete jpg_provider;
		delete png_provider;
		delete targa_provider;
		delete tga_provider;

		instance = nullptr;
	}

	void SetupDisplay_Impl::add_cache_factory_file(ResourceManager &manager, const FileResourceDocument &doc)
	{
		DisplayCache::set(manager, std::make_shared<FileDisplayCache>(doc));
	}

#ifdef WIN32
	DisplayMessageQueue_Win32 *SetupDisplay::get_message_queue()
	{
		if (!SetupDisplay_Impl::instance)
			start();
		return &SetupDisplay_Impl::instance->message_queue;
	}
#elif !defined(__APPLE__) && !defined(__ANDROID__)
	DisplayMessageQueue_X11* SetupDisplay::get_message_queue()
	{
		if (!SetupDisplay_Impl::instance)
			start();
		return &SetupDisplay_Impl::instance->message_queue;
	}
#endif
	std::map<std::string, ImageProviderType *> *SetupDisplay::get_image_provider_factory_types()
	{
		if (!SetupDisplay_Impl::instance)
			start();
		return &SetupDisplay_Impl::instance->image_provider_factory_types;
	}
}
