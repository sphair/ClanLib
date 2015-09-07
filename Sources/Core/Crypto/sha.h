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
**    Mark Page
*/

#pragma once

#include "API/Core/System/cl_platform.h"

namespace clan
{
	class ubyte128
	{
	public:
		ubyte128() {}
		ubyte128(const uint64_t &value) : high(0), low(value) {}
		ubyte128(const ubyte128 &value) : high(value.high), low(value.low) {}

		ubyte128 operator+(const uint64_t & value)
		{
			ubyte128 sum;
			sum.high = high;
			sum.low = low + value;
			if (sum.low < low)
				++sum.high;
			return sum;
		}

		ubyte128 operator+(const ubyte128 & rhs)
		{
			ubyte128 sum;
			sum.high = high + rhs.high;
			sum.low = low + rhs.low;
			if (sum.low < low)
				++sum.high;
			return sum;
		}
		ubyte128 operator-(const ubyte128 & rhs)
		{
			ubyte128 difference;
			difference.high = high - rhs.high;
			difference.low = low - rhs.low;
			if (difference.low > low)
				--difference.high;
			return difference;
		}

		uint64_t high;
		uint64_t low;
	};

	enum cl_sha_type
	{
		cl_sha_1,
		cl_sha_224,
		cl_sha_256,
		cl_sha_384,
		cl_sha_512,
		cl_sha_512_224,
		cl_sha_512_256
	};

	// Private class for the SHA classes
	class SHA
	{
	public:
		inline uint32_t leftrotate_uint32(uint32_t value, int shift) const
		{
			return (value << shift) | (value >> (32 - shift));
		}

		inline uint32_t rightrotate_uint32(uint32_t value, int shift) const
		{
			return (value >> shift) | (value << (32 - shift));
		}

		inline uint64_t leftrotate_uint64(uint64_t value, int shift) const
		{
			return (value << shift) | (value >> (64 - shift));
		}

		inline uint64_t rightrotate_uint64(uint64_t value, int shift) const
		{
			return (value >> shift) | (value << (64 - shift));
		}

		void to_hex_be(char *buffer, uint32_t value, bool uppercase) const;
		void to_hex_le(char *buffer, uint32_t value, bool uppercase) const;
		void to_hex64_be(char *buffer, uint64_t value, bool uppercase) const;
	};
}
