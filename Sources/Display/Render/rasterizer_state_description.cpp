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

#include "Display/precomp.h"
#include "API/Display/Render/rasterizer_state_description.h"

namespace clan
{

class RasterizerStateDescription_Impl
{
public:
	RasterizerStateDescription_Impl() : culled(), line_antialiasing(), face_cull_mode(), face_fill_mode(), front_face(), enable_scissor()
	{
		line_antialiasing = false;
		culled = false;
		face_cull_mode = cull_back;
		face_fill_mode = fill_polygon;
		front_face = face_counter_clockwise;
		enable_scissor = true;
	}

	bool operator==(const RasterizerStateDescription_Impl &other) const
	{
		return
			culled == other.culled &&
			line_antialiasing == other.line_antialiasing &&
			face_cull_mode == other.face_cull_mode &&
			face_fill_mode == other.face_fill_mode &&
			front_face == other.front_face &&
			enable_scissor == other.enable_scissor;
	}

	bool operator<(const RasterizerStateDescription_Impl &other) const
	{
		if (culled != other.culled)
			return culled < other.culled;
		else if (line_antialiasing != other.line_antialiasing)
			return line_antialiasing < other.line_antialiasing;
		else if (face_cull_mode != other.face_cull_mode)
			return face_cull_mode < other.face_cull_mode;
		else if (face_fill_mode != other.face_fill_mode)
			return face_fill_mode < other.face_fill_mode;
		else if (front_face != other.front_face)
			return front_face < other.front_face;
		else
			return enable_scissor < other.enable_scissor;
	}

	bool culled;
	bool line_antialiasing;
	CullMode face_cull_mode;
	FillMode face_fill_mode;
	FaceSide front_face;
	bool enable_scissor;
};

RasterizerStateDescription::RasterizerStateDescription()
: impl(new RasterizerStateDescription_Impl())
{
}

RasterizerStateDescription RasterizerStateDescription::clone() const
{
	RasterizerStateDescription copy;
	if (impl)
		copy.impl = std::shared_ptr<RasterizerStateDescription_Impl>(new RasterizerStateDescription_Impl(*impl.get()));
	return copy;
}

bool RasterizerStateDescription::get_culled() const
{
	return impl->culled;
}

bool RasterizerStateDescription::get_enable_line_antialiasing() const
{
	return impl->line_antialiasing;
}

CullMode RasterizerStateDescription::get_face_cull_mode() const
{
	return impl->face_cull_mode;
}

FillMode RasterizerStateDescription::get_face_fill_mode() const
{
	return impl->face_fill_mode;
}

FaceSide RasterizerStateDescription::get_front_face() const
{
	return impl->front_face;
}

bool RasterizerStateDescription::get_enable_scissor() const
{
	return impl->enable_scissor;
}

void RasterizerStateDescription::set_culled(bool value)
{
	impl->culled = value;
}

void RasterizerStateDescription::enable_line_antialiasing(bool enabled)
{
	impl->line_antialiasing = enabled;
}

void RasterizerStateDescription::set_face_cull_mode(CullMode value)
{
	impl->face_cull_mode = value;
}

void RasterizerStateDescription::set_face_fill_mode(FillMode value)
{
	impl->face_fill_mode = value;
}

void RasterizerStateDescription::set_front_face(FaceSide value)
{
	impl->front_face = value;
}

void RasterizerStateDescription::enable_scissor(bool enabled)
{
	impl->enable_scissor = enabled;
}

bool RasterizerStateDescription::operator==(const RasterizerStateDescription &other) const
{
	return impl == other.impl || *impl.get() == *other.impl.get();
}

bool RasterizerStateDescription::operator<(const RasterizerStateDescription &other) const
{
	return *impl.get() < *other.impl.get();
}

}
