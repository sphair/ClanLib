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

#include "Core/precomp.h"
#include "API/Core/Math/easing.h"
#include <algorithm>
#include <cmath>

namespace clan
{
	float Easing::linear(float t)
	{
		return t;
	}

	float Easing::easeout(float t)
	{
		return std::pow(t, 1.7f);
	}

	float Easing::easein(float t)
	{
		return std::pow(t, 0.48f);
	}

	float Easing::easeinout(float t)
	{
		if (t >= 1.0f)
		{
			return 1.0f;
		}
		else if (t <= 0.0f)
		{
			return 0.0f;
		}
		else
		{
			float q = 0.48f - t / 1.04f;
			float Q = std::sqrt(0.1734f + q * q);
			float x = Q - q;
			float X = std::pow(std::abs(x), 1.0f / 3.0f) * (x < 0.0f ? -1.0f : 1.0f);
			float y = -Q - q;
			float Y = std::pow(std::abs(y), 1.0f / 3.0f) * (y < 0.0f ? -1.0f : 1.0f);
			float n = X + Y + 0.5f;
			return (1.0f - n) * 3.0f * n * n + n * n * n;
		}
	}

	float Easing::backin(float t)
	{
		if (t >= 1.0f)
		{
			return 1.0f;
		}
		else if (t <= 0.0f)
		{
			return 0.0f;
		}
		else
		{
			float s = 1.70158f;
			return t * t * ((s + 1.0f) * t - s);
		}
	}

	float Easing::backout(float t)
	{
		if (t >= 1.0f)
		{
			return 1.0f;
		}
		else if (t <= 0.0f)
		{
			return 0.0f;
		}
		else
		{
			t = t - 1.0f;
			float s = 1.70158f;
			return t * t * ((s + 1.0f) * t + s) + 1.0f;
		}
	}

	float Easing::elastic(float t)
	{
		float pi = 3.14159265359f;
		return std::pow(2.0f, -10.0f * t) * std::sin((t - 0.075f) * (2.0f * pi) / 0.3f) + 1.0f;
	}

	float Easing::bounce(float t)
	{
		float s = 7.5625f;
		float p = 2.75f;
		if (t < (1.0f / p))
		{
			return s * t * t;
		}
		else if (t < (2.0f / p))
		{
			t -= (1.5f / p);
			return s * t * t + 0.75f;
		}
		else if (t < (2.5f / p))
		{
			t -= (2.25f / p);
			return s * t * t + 0.9375f;
		}
		else
		{
			t -= (2.625f / p);
			return s * t * t + 0.984375f;
		}
	}
}
