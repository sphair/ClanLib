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
*/

#pragma once

#include <list>
#include "API/Core/Math/vec3.h"
#include "API/Sound/soundbuffer.h"
#include "API/Sound/soundbuffer_session.h"

namespace clan
{
	class AudioWorld_Impl;

	class AudioObject_Impl
	{
	public:
		AudioObject_Impl(AudioWorld_Impl *world);
		~AudioObject_Impl();

		AudioWorld_Impl *world;
		std::list<AudioObject_Impl *>::iterator it;

		Vec3f position;
		float attenuation_begin;
		float attenuation_end;
		float volume;
		bool looping;
		bool ambience;
		SoundBuffer sound;
		SoundBuffer_Session session;
	};
}
