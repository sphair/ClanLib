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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

int main(int argc, char** argv)
{
	TestApp program;
	program.main();
}

int TestApp::main()
{
	// Create a console window for text-output if not available
	ConsoleWindow console("Console");

	try
	{
		Console::write_line("ClanLib Test Suite:");
		Console::write_line("-------------------");
#ifdef WIN32
		Console::write_line("Target: WIN32");
#else
		Console::write_line("Target: LINUX");
#endif
		Console::write_line("For clanSound SSE functions");

		do_test();
		
		Console::write_line("All Tests Complete");
		console.display_close_message();
	}

	catch(Exception error)
	{
		Console::write_line("Exception caught:");
		Console::write_line(error.message);
		console.display_close_message();
		return -1;
	}

	return 0;
}

void TestApp::fail(void)
{
	throw Exception("Failed Test");
}

void TestApp::do_test()
{
	const int data_size = 555;
	float in_float_buffer1[data_size];
	float in_float_buffer2[data_size];
	float *in_float[2] = {in_float_buffer1, in_float_buffer2};
	short in_16_buffer1[data_size];
	short in_16_buffer2[data_size];
	short *in_16[2] = {in_16_buffer1, in_16_buffer2};
	unsigned char in_8_buffer1[data_size];
	unsigned char in_8_buffer2[data_size];
	unsigned char *in_8[2] = {in_8_buffer1, in_8_buffer2};
	float out_float_buffer1[data_size];
	float out_float_buffer2[data_size];
	float *out_float[2] = {out_float_buffer1, out_float_buffer2};
	short out_16_buffer1[data_size];
	short out_16_buffer2[data_size];
	short *out_16[2] = {out_16_buffer1, out_16_buffer2};
	unsigned char out_8_buffer1[data_size];
	unsigned char out_8_buffer2[data_size];
	unsigned char *out_8[2] = {out_8_buffer1, out_8_buffer2};

	float out2_float_buffer1[data_size];
	float out2_float_buffer2[data_size];
	float *out2_float[2] = {out2_float_buffer1, out2_float_buffer2};
	short out2_16_buffer1[data_size];
	short out2_16_buffer2[data_size];
	short *out2_16[2] = {out2_16_buffer1, out2_16_buffer2};
	unsigned char out2_8_buffer1[data_size];
	unsigned char out2_8_buffer2[data_size];
	unsigned char *out2_8[2] = {out2_8_buffer1, out2_8_buffer2};

	int random_number = 1234542;
	for (int cnt=0; cnt<data_size; cnt++)
	{
		random_number += 12231 * 111;
		in_8_buffer1[cnt] = ( unsigned char) (random_number >> 9);
		in_8_buffer2[cnt] = ( unsigned char) (random_number >> 7);
		in_16_buffer1[cnt] = ( short) (random_number >> 5);
		in_16_buffer2[cnt] = ( short) (random_number >> 3);
		float dummy;
		in_float_buffer1[cnt] = std::modf(( float (random_number)) / 13.223f, &dummy);
		in_float_buffer2[cnt] = std::modf(( float (random_number)) / 53.321f, &dummy);
	}

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	memcpy(out_float_buffer2, in_float_buffer2, sizeof(out_float_buffer2));
	unpack_16bit_stereo(in_16_buffer1, data_size, out_float);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	memcpy(out2_float_buffer2, in_float_buffer2, sizeof(out2_float_buffer2));
	SoundSSE::unpack_16bit_stereo(in_16_buffer1, data_size, out2_float);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);
	check_float(out_float_buffer2, out2_float_buffer2, data_size);

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	unpack_16bit_mono(in_16_buffer1, data_size, out_float_buffer1);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	SoundSSE::unpack_16bit_mono(in_16_buffer1, data_size, out2_float_buffer1);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	memcpy(out_float_buffer2, in_float_buffer2, sizeof(out_float_buffer2));
	unpack_8bit_stereo(in_8_buffer1, data_size, out_float);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	memcpy(out2_float_buffer2, in_float_buffer2, sizeof(out2_float_buffer2));
	SoundSSE::unpack_8bit_stereo(in_8_buffer1, data_size, out2_float);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);
	check_float(out_float_buffer2, out2_float_buffer2, data_size);

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	unpack_8bit_mono(in_8_buffer1, data_size, out_float_buffer1);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	SoundSSE::unpack_8bit_mono(in_8_buffer1, data_size, out2_float_buffer1);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	unpack_float_mono(in_float_buffer1, data_size, out_float_buffer1);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	SoundSSE::unpack_float_mono(in_float_buffer1, data_size, out2_float_buffer1);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	memcpy(out_float_buffer2, in_float_buffer2, sizeof(out_float_buffer2));
	unpack_float_stereo(in_float_buffer1, data_size, out_float);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	memcpy(out2_float_buffer2, in_float_buffer2, sizeof(out2_float_buffer2));
	SoundSSE::unpack_float_stereo(in_float_buffer1, data_size, out2_float);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);
	check_float(out_float_buffer2, out2_float_buffer2, data_size);

	memset(out_16_buffer1, 0, sizeof(out_16_buffer1));
	pack_16bit_stereo(in_float, data_size/2, out_16_buffer1);
	memset(out2_16_buffer1, 0, sizeof(out2_16_buffer1));
	SoundSSE::pack_16bit_stereo(in_float, data_size/2, out2_16_buffer1);
	check_16(out_16_buffer1, out2_16_buffer1, data_size);

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	pack_float_stereo(in_float, data_size/2, out_float_buffer1);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	SoundSSE::pack_float_stereo(in_float, data_size/2, out2_float_buffer1);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	copy_float(in_float_buffer1, data_size, out_float_buffer1);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	SoundSSE::copy_float(in_float_buffer1, data_size, out2_float_buffer1);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);

	memset(out_float_buffer1, 0, sizeof(out_float_buffer1));
	copy_float(in_float_buffer1, data_size, out_float_buffer1);
	multiply_float(out_float_buffer1, data_size, 0.34f);
	memset(out2_float_buffer1, 0, sizeof(out2_float_buffer1));
	copy_float(in_float_buffer1, data_size, out2_float_buffer1);
	SoundSSE::multiply_float(out2_float_buffer1, data_size, 0.34f);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	set_float(out_float_buffer1, data_size, 0.34f);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	SoundSSE::set_float(out2_float_buffer1, data_size, 0.34f);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	mix_one_to_one(in_float_buffer1, data_size, out_float_buffer1, 0.34f);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	SoundSSE::mix_one_to_one(in_float_buffer1, data_size, out2_float_buffer1, 0.34f);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);

	float volumes[2] = {0.34f, 0.82f};
	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	memcpy(out_float_buffer2, in_float_buffer2, sizeof(out_float_buffer2));
	mix_one_to_many(in_float_buffer1, data_size/2, out_float, volumes, 2);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	memcpy(out2_float_buffer2, in_float_buffer2, sizeof(out2_float_buffer2));
	SoundSSE::mix_one_to_many(in_float_buffer1, data_size/2, out2_float, volumes, 2);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);
	check_float(out_float_buffer2, out2_float_buffer2, data_size);

	memcpy(out_float_buffer1, in_float_buffer1, sizeof(out_float_buffer1));
	mix_many_to_one(in_float, volumes, 2, data_size, out_float_buffer1);
	memcpy(out2_float_buffer1, in_float_buffer1, sizeof(out2_float_buffer1));
	SoundSSE::mix_many_to_one(in_float, volumes, 2, data_size, out2_float_buffer1);
	check_float(out_float_buffer1, out2_float_buffer1, data_size);

}

void TestApp::check_float(float *aptr, float *bptr, int num)
{
	for(; num>0; num--)
	{
		float a = *(aptr++);
		float b = *(bptr++);
		if ((a - 0.0001) > b)
				fail();
		if ((a + 0.0001) < b)
				fail();
	}
}

void TestApp::check_16(short *aptr, short *bptr, int num)
{
	for(; num>0; num--)
	{
		short a = *(aptr++);
		short b = *(bptr++);
		if ((a - 1) > b)
				fail();
		if ((a + 1) < b)
				fail();
	}
}

void TestApp::unpack_16bit_stereo(short *input, int size, float *output[2])
{
	const int sse_size = 0;

	// unpack remaining
	for (int i = sse_size; i < size; i+=2)
	{
		output[0][i/2] = ((float) input[i]) / 32767.0f;
		output[1][i/2] = ((float) input[i+1]) / 32767.0f;
	}
}

void TestApp::unpack_16bit_mono(short *input, int size, float *output)
{
	const int sse_size = 0;


	// unpack remaining
	for (int i = sse_size; i < size; i++)
	{
		output[i] = ((float) input[i]) / 32767.0f;
	}
}

void TestApp::unpack_8bit_stereo(unsigned char *input, int size, float *output[2])
{
	const int sse_size = 0;

	// unpack remaining
	for (int i = sse_size; i < size; i+=2)
	{
		int value = input[i];
		output[0][i/2] = ((float) (value - 128)) / 128.0f;

		value = input[i+1];
		output[1][i/2] = ((float) (value - 128)) / 128.0f;
	}
}

void TestApp::unpack_8bit_mono(unsigned char *input, int size, float *output)
{
	const int sse_size = 0;

	// unpack remaining
	for (int i = sse_size; i < size; i++)
	{
		int value = input[i];
		output[i] = ((float) (value - 128)) / 128.0f;
	}
}

void TestApp::pack_16bit_stereo(float *input[2], int size, short *output)
{
	const int sse_size = 0;

	// Pack remaining
	for (int i = sse_size; i < size; i++)
	{
		output[i*2] = input[0][i]*32767;
		output[i*2 + 1] = input[1][i]*32767;
	}
}

void TestApp::pack_float_stereo(float *input[2], int size, float *output)
{
	const int sse_size = 0;

	// Pack remaining
	for (int i = sse_size; i < size; i++)
	{
		output[i*2] = input[0][i];
		output[i*2 + 1] = input[1][i];
	}
}

void TestApp::copy_float(float *input, int size, float *output)
{
	const int sse_size = 0;

	// Copy remaining
	for (int i = sse_size; i < size; i++)
		output[i] = input[i];
}

void TestApp::multiply_float(float *channel, int size, float volume)
{
	const int sse_size = 0;

	for (int i = sse_size; i < size; i++)
		channel[i] *= volume;
}

void TestApp::set_float(float *channel, int size, float value)
{
	const int sse_size = 0;

	for (int i = sse_size; i < size; i++)
		channel[i] = value;
}

void TestApp::mix_one_to_one(float *input, int size, float *output, float volume)
{
	const int sse_size = 0;

	for (int i = sse_size; i < size; i++)
	{
		output[i] += input[i] * volume;
	}
}

void TestApp::mix_one_to_many(float *input, int size, float **output, float *volume, int channels)
{
	const int sse_size = 0;

	for (int i = sse_size; i < size; i++)
	{
		float sample0 = input[i];
		for (int j = 0; j < channels; j++)
		{
			output[j][i] += sample0 * volume[j];
		}
	}
}

void TestApp::mix_many_to_one(float **input, float *volume, int channels, int size, float *output)
{
	const int sse_size = 0;

	for (int i = sse_size; i < size; i++)
	{
		float sample0 = output[i];
		for (int j = 0; j < channels; j++)
		{
			sample0 += input[j][i] * volume[j];
		}
		output[i] = sample0;
	}
}

void TestApp::unpack_float_stereo(float *input, int size, float *output[2])
{
	const int sse_size = 0;

	// unpack remaining
	for (int i = sse_size; i < size; i+=2)
	{
		output[0][i/2] = input[i];
		output[1][i/2] = input[i+1];
	}
}

void TestApp::unpack_float_mono(float *input, int size, float *output)
{
	const int sse_size = 0;
	// unpack remaining
	if(sse_size < size)
		memcpy(output, input, (size-sse_size)*sizeof(float));
}
