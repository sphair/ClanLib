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

#pragma once

#include "API/Display/System/timer.h"
#include "animation.h"
#include <algorithm>
#include <chrono>
#include <vector>

namespace clan
{
	class AnimationGroup
	{
	public:
		AnimationGroup()
		{
		}

		~AnimationGroup()
		{
			stop();
		}

		AnimationGroup(const AnimationGroup &) = delete;
		AnimationGroup &operator =(AnimationGroup &) = delete;

		void start(Animation animation)
		{
			if (active_animations.empty())
			{
				timer.func_expired() = [&]()
				{
					std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();

					auto it = active_animations.begin();
					while (it != active_animations.end())
					{
						long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - it->start_time).count();
						float t = clan::max(clan::min(static_cast<float>(elapsed) / it->duration, 1.0f), 0.0f);

						t = it->easing(t);
						it->setter(it->from * (1.0f - t) + it->to * t);

						if (t >= 1.0f)
						{
							if (it->animation_end)
								it->animation_end();
							it = active_animations.erase(it);
						}
						else
						{
							++it;
						}
					}

					if (active_animations.empty())
						timer.stop();
				};
			}
			timer.start(16, true);

			animation.start_time = std::chrono::steady_clock::now();
			active_animations.push_back(animation);
		}

		void stop()
		{
			timer.stop();
			active_animations.clear();
		}

	private:
		std::vector<Animation> active_animations;
		Timer timer;
	};
}
