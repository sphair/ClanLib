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

#pragma once

#include <map>

namespace clan
{
	class DisplayMessageQueue_Win32;
	class DisplayMessageQueue_X11;
	class ImageProviderType;

	class SetupDisplay
	{
	public:
		static void start();

#ifdef WIN32
		static DisplayMessageQueue_Win32* get_message_queue();
#elif !defined(__APPLE__) && !defined(__ANDROID__)
		static DisplayMessageQueue_X11* get_message_queue();
#endif
		static std::map<std::string, ImageProviderType *> *get_image_provider_factory_types();
	};
}
