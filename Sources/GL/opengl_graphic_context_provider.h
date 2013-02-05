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
**    Magnus Norddahl
*/

#pragma once

#include "API/Display/TargetProviders/graphic_context_provider.h"

namespace clan
{

class OpenGLGraphicContextProvider : public GraphicContextProvider
{
public:
	virtual int get_max_texture_coords() { return 0; }
	virtual void get_opengl_version(int &version_major, int &version_minor) { int version_release = 0; get_opengl_version(version_major, version_minor, version_release); }
	virtual void get_opengl_version(int &version_major, int &version_minor, int &version_release) = 0;
	virtual void get_opengl_shading_language_version(int &version_major, int &version_minor) { version_major = 0; version_minor = 0; }
	virtual std::string get_renderer_string() { return std::string(); }
	virtual std::string get_vendor_string() { return std::string(); }
	virtual std::vector<std::string> get_extensions() { return std::vector<std::string>(); }
};

}
