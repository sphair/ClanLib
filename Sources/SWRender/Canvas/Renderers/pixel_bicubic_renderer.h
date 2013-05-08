/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include <emmintrin.h>

namespace clan
{

class PixelBicubicRenderer
{
public:
	PixelBicubicRenderer();
	~PixelBicubicRenderer();

	void set_dest(unsigned int *data, int width, int height);
	void set_src(unsigned int *data, int width, int height);
	void set_core(int core, int num_cores);

	void render(int x, int y, int zoom_number, int zoom_denominator);

private:
	/// \brief Perform cubic scaling
	///
	/// Since we are magnifying from one discrete coordinate system to another, the magnification factor, r, must
	/// be a rational number, so we can represent it in a program with a pair of integer variables, n and d, such that r = n/d
	/// 
	/// a is a spline parameter such that -1 <= a <= 0
	void scale(float a, int n, int d, int in_width, int in_pitch, int in_height, const unsigned int *in_data, int out_width, int out_pitch, int out_height, unsigned int *out_data);

	static int find_first_line_for_core(int y_start, int core, int num_cores);
	static int get_larger_out_dimension(int out_width, int out_height);
	void prepare(float a, int n, int d, int in_width, int out_width, int out_height);
	void reset();
	void *aligned_alloc(int size);
	void aligned_free(void *ptr);

	__m128 *get_h();
	int *get_L();
	__m128 *get_c(int i);

	inline static float C0(float t, float a);
	inline static float C1(float t, float a);
	inline static float C2(float t, float a);
	inline static float C3(float t, float a);

	std::vector<int> L_vector;
	float *c_vector[4];
	float *h_vector;

	unsigned int *dest;
	int dest_width;
	int dest_height;

	unsigned int *src;
	int src_width;
	int src_height;

	int core;
	int num_cores;
};

}
