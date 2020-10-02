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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include <ClanLib/core.h>
#include <ClanLib/sound.h>
using namespace clan;

#include <cmath>

class TestApp
{
public:
	int main();

private:
	void do_test();

	static void unpack_16bit_stereo(short *input, int size, float *output[2]);
	static void unpack_16bit_mono(short *input, int size, float *output);
	static void unpack_8bit_stereo(unsigned char *input, int size, float *output[2]);
	static void unpack_8bit_mono(unsigned char *input, int size, float *output);
	static void unpack_float_mono(float *input, int size, float *output);
	static void unpack_float_stereo(float *input, int size, float *output[2]);
	static void pack_16bit_stereo(float *input[2], int size, short *output);
	static void pack_float_stereo(float *input[2], int size, float *output);
	static void copy_float(float *input, int size, float *output);
	static void multiply_float(float *channel, int size, float volume);
	static void set_float(float *channel, int size, float value);
	static void mix_one_to_one(float *input, int size, float *output, float volume);
	static void mix_one_to_many(float *input, int size, float **output, float *volume, int channels);
	static void mix_many_to_one(float **input, float *volume, int channels, int size, float *output);

	void check_16(short *aptr, short *bptr, int num);
	void check_float(float *aptr, float *bptr, int num);
	void fail();
};

