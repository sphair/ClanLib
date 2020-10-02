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
*/

#include <functional>
#include <chrono>

namespace clan
{
	class Animation
	{
	public:
		Animation(float from, float to, const std::function<void(float)> &setter, int duration, const std::function<float(float)> &easing, std::function<void()> animation_end) :
			from(from), to(to), setter(setter), duration(duration), easing(easing), animation_end(animation_end)
		{
		}

		float from = 0.0f;
		float to = 1.0f;
		std::function<void(float)> setter;
		int duration = 400;
		std::function<float(float)> easing;
		std::function<void()> animation_end;

		std::chrono::steady_clock::time_point start_time;
	};
}
