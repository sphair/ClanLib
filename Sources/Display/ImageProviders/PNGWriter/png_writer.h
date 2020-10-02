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

#include "API/Core/IOData/iodevice.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/System/databuffer.h"

namespace clan
{
	class PNGWriter
	{
	public:
		static void save(IODevice iodevice, PixelBuffer image);
		
	private:
		PNGWriter(IODevice iodevice, PixelBuffer image);
		void save();

		void write_magic();
		void write_headers();
		void write_data();
		
		void write_chunk(const char name[4], const void *data, int size);
		
		IODevice device;
		PixelBuffer image;
	};
	
	class PNGCRC32
	{
	public:
		static unsigned long crc(const char name[4], const void *data, int len)
		{
			static PNGCRC32 impl;
			
			const unsigned char *buf = reinterpret_cast<const unsigned char*>(data);
			
			unsigned int c = 0xffffffff;

			for (int n = 0; n < 4; n++)
				c = impl.crc_table[(c ^ name[n]) & 0xff] ^ (c >> 8);

			for (int n = 0; n < len; n++)
				c = impl.crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);

			return c ^ 0xffffffff;
		}
		
	private:
		unsigned int crc_table[256];
		
		PNGCRC32()
		{
			for (unsigned int n = 0; n < 256; n++)
			{
				unsigned int c = n;
				for (unsigned int k = 0; k < 8; k++)
				{
					if ((c & 1) == 1)
						c = 0xedb88320 ^ (c >> 1);
					else
						c = c >> 1;
				}
				crc_table[n] = c;
			}
		}
	};
}
