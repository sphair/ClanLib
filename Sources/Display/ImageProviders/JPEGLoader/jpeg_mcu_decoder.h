/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

namespace clan
{
	class JPEGLoader;

	class JPEGMCUDecoder
	{
	public:
		JPEGMCUDecoder(JPEGLoader *loader);
		~JPEGMCUDecoder();

		void decode(int block);
		int get_channel_count() const { return (int)channels.size(); }
		const unsigned char *get_channel(int c) const { return channels[c]; }

	private:
		void idct(short *inptr, unsigned char *outptr, int pitch, float *quantptr);
		void idct_sse(short *inptr, unsigned char *outptr, int pitch, float *quantptr);
		static inline unsigned char float_to_int(float v);

		JPEGLoader *loader;
		std::vector<unsigned char *> channels;
		std::vector<float *> quant;
	};
}
