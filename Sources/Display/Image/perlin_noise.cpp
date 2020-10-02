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
*/

#include "Display/precomp.h"
#include "API/Display/Image/perlin_noise.h"
#include <cstdlib>

// This perlin noise code is based from ideas from numerious sources, including
// The original perlin noise example code
// Ken Perlin's Improving Noise paper
// Stafan Gustavson Noise1234 Perlin noise class
// John Ratcliff Perlin noise class
// And snippits of others found in various forums

// http://mrl.nyu.edu/~perlin/paper445.pdf - 6t5-15t4+10t3
#define cl_s_curve(t) ( t * t * t * ( t * ( t * 6.0f - 15.0f ) + 10.0f ) )

#define cl_floor_to_int(value) ( ((value)>0.0f) ? ((int)value) : ((int)value-1 ) )
#define cl_lerp(t, a, b) ((a) + (t)*((b)-(a)))

#define permutation_table_size	256
#define permutation_table_mask	(0xff)

#define cl_period_mask_x	permutation_table_mask
#define cl_period_mask_y	permutation_table_mask
#define cl_period_mask_z	permutation_table_mask
#define cl_period_mask_w	permutation_table_mask

namespace clan
{
	class PerlinNoise_PixelWriter
	{
	public:
		virtual void line_end() = 0;
		virtual void write_pixel(float value) = 0;
	};

	class PerlinNoise_PixelWriter_RGBA8 : public PerlinNoise_PixelWriter
	{
	public:
		PerlinNoise_PixelWriter_RGBA8(PixelBuffer &pbuff)
			: pitch(pbuff.get_pitch() / pbuff.get_bytes_per_pixel()),
			current_ptr((uint32_t *)pbuff.get_data()),
			line_start_ptr(current_ptr)
		{
		}

		void line_end() override
		{
			line_start_ptr += pitch;
			current_ptr = line_start_ptr;
		}

		void write_pixel(float value) override
		{
			int color = (int)((value*128.0f) + 128.0f);
			if (color > 255)
				color = 255;
			if (color < 0)
				color = 0;

			*(current_ptr++) = color << 24 | color << 16 | color << 8 | color;
		}
	private:
		int pitch;
		uint32_t *current_ptr;
		uint32_t *line_start_ptr;
	};

	class PerlinNoise_PixelWriter_RGB8 : public PerlinNoise_PixelWriter
	{
	public:
		PerlinNoise_PixelWriter_RGB8(PixelBuffer &pbuff)
			: pitch(pbuff.get_pitch()),
			current_ptr((uint8_t *)pbuff.get_data()),
			line_start_ptr(current_ptr)
		{
		}

		void line_end() override
		{
			line_start_ptr += pitch;
			current_ptr = line_start_ptr;
		}

		void write_pixel(float value) override
		{
			int color = (int)((value*128.0f) + 128.0f);
			if (color > 255)
				color = 255;
			if (color < 0)
				color = 0;

			*(current_ptr++) = color;
			*(current_ptr++) = color;
			*(current_ptr++) = color;
		}
	private:
		int pitch;
		uint8_t *current_ptr;
		uint8_t *line_start_ptr;
	};

	class PerlinNoise_PixelWriter_R8 : public PerlinNoise_PixelWriter
	{
	public:
		PerlinNoise_PixelWriter_R8(PixelBuffer &pbuff)
			: pitch(pbuff.get_pitch()),
			current_ptr((uint8_t *)pbuff.get_data()),
			line_start_ptr(current_ptr)
		{
		}

		void line_end() override
		{
			line_start_ptr += pitch;
			current_ptr = line_start_ptr;
		}

		void write_pixel(float value) override
		{
			int color = (int)((value*128.0f) + 128.0f);
			if (color > 255)
				color = 255;
			if (color < 0)
				color = 0;

			*(current_ptr++) = color;
		}
	private:
		int pitch;
		uint8_t *current_ptr;
		uint8_t *line_start_ptr;
	};

	class PerlinNoise_PixelWriter_R32f : public PerlinNoise_PixelWriter
	{
	public:
		PerlinNoise_PixelWriter_R32f(PixelBuffer &pbuff)
			: pitch(pbuff.get_pitch() / sizeof(float)),
			current_ptr((float *)pbuff.get_data()),
			line_start_ptr(current_ptr)
		{
		}

		void line_end() override
		{
			line_start_ptr += pitch;
			current_ptr = line_start_ptr;
		}

		void write_pixel(float value) override
		{
			*(current_ptr++) = value;
		}
	private:
		int pitch;
		float *current_ptr;
		float *line_start_ptr;
	};

	class PerlinNoise_Impl
	{
	public:
		PerlinNoise_Impl()
		{
		}

		void set_permutations(const unsigned char *table, unsigned int size);

		PixelBuffer create_noise4d(float start_x, float end_x, float start_y, float end_y, float z_position, float w_position);
		PixelBuffer create_noise3d(float start_x, float end_x, float start_y, float end_y, float z_position);
		PixelBuffer create_noise2d(float start_x, float end_x, float start_y, float end_y);
		PixelBuffer create_noise1d(float start_x, float end_x);

	public:
		TextureFormat texture_format = TextureFormat::rgb8;
		float amplitude = 1.0f;
		int width = 256;
		int height = 256;
		int octaves = 1;

	private:
		void create_noise4d(PerlinNoise_PixelWriter &writer, float start_x, float end_x, float start_y, float end_y, float z_position, float w_position);
		void create_noise3d(PerlinNoise_PixelWriter &writer, float start_x, float end_x, float start_y, float end_y, float z_position);
		void create_noise2d(PerlinNoise_PixelWriter &writer, float start_x, float end_x, float start_y, float end_y);
		void create_noise1d(PerlinNoise_PixelWriter &writer, float start_x, float end_x);

		inline float gradient_1d(int permutation_value, float x);
		inline float gradient_2d(int permutation_value, float x, float y);
		inline float gradient_3d(int permutation_value, float x, float y, float z);
		inline float gradient_4d(int permutation_value, float x, float y, float z, float t);

		float noise_1d(float x);
		float noise_2d(float x, float y);
		float noise_3d(float x, float y, float z);
		float noise_4d(float x, float y, float z, float w);

		void setup();

		bool permutation_table_set = false;

		unsigned char permutation_table[permutation_table_size * 2];	// Table duplicated at permutation_table_size

	};

	PerlinNoise::PerlinNoise() : impl(std::make_shared<PerlinNoise_Impl>())
	{
	}

	PerlinNoise::~PerlinNoise()
	{
	}

	void PerlinNoise::set_permutations(const unsigned char *table, unsigned int size)
	{
		impl->set_permutations(table, size);
	}

	PixelBuffer PerlinNoise::create_noise1d(float start_x, float end_x)
	{
		return impl->create_noise1d(start_x, end_x);
	}

	PixelBuffer PerlinNoise::create_noise2d(float start_x, float end_x, float start_y, float end_y)
	{
		return impl->create_noise2d(start_x, end_x, start_y, end_y);
	}

	PixelBuffer PerlinNoise::create_noise3d(float start_x, float end_x, float start_y, float end_y, float z_position)
	{
		return impl->create_noise3d(start_x, end_x, start_y, end_y, z_position);
	}

	PixelBuffer PerlinNoise::create_noise4d(float start_x, float end_x, float start_y, float end_y, float z_position, float w_position)
	{
		return impl->create_noise4d(start_x, end_x, start_y, end_y, z_position, w_position);
	}

	Size PerlinNoise::get_size() const
	{
		return Size(impl->width, impl->height);
	}

	TextureFormat PerlinNoise::get_format() const
	{
		return impl->texture_format;
	}

	float PerlinNoise::get_amplitude() const
	{
		return impl->amplitude;
	}

	int PerlinNoise::get_octaves() const
	{
		return impl->octaves;
	}

	void PerlinNoise::set_size(int width, int height)
	{
		impl->width = width;
		impl->height = height;
	}

	void PerlinNoise::set_size(const Size &size)
	{
		impl->width = size.width;
		impl->height = size.height;
	}

	void PerlinNoise::set_format(TextureFormat texture_format)
	{
		impl->texture_format = texture_format;
	}

	void PerlinNoise::set_amplitude(float amplitude)
	{
		impl->amplitude = amplitude;
	}
	void PerlinNoise::set_octaves(int octaves)
	{
		impl->octaves = octaves;
	}

	float PerlinNoise_Impl::gradient_1d(int permutation_value, float x)
	{
		// Find gradient between -8.0f and 8.0f (excluding 0.0f)
		float gradient = 1.0f + (permutation_value & 7);
		if (permutation_value & 8)
			gradient = -gradient;
		return gradient * x;
	}

	float PerlinNoise_Impl::gradient_2d(int permutation_value, float x, float y)
	{
		float u, v;
		if (permutation_value & 4)
		{
			u = y;
			v = x;
		}
		else
		{
			u = x;
			v = y;
		}

		if (permutation_value & 1)
		{
			u = -u;
		}

		if (permutation_value & 2)
		{
			v = -v;
		}

		return u + (2.0f * v);
	}

	float PerlinNoise_Impl::gradient_3d(int permutation_value, float x, float y, float z)
	{
		// (1,1,0),(-1,1,0),(1,-1,0),(-1,-1,0),
		// (1,0,1),(-1,0,1),(1,0,-1),(-1,0,-1),
		// (0,1,1),(0,-1,1),(0,1,-1),(0,-1,-1)
		// To  avoid  the  cost  of  dividing  by  12,  we  pad  to  16  gradient 
		// directions,  adding  an  extra  (1,1,0),(-1,1,0),(0,-1,1)  and  (0,-1,-1). 
		// These  form  a  regular  tetrahedron,

		float u, v;
		permutation_value = permutation_value & 15;	// Interested in only 16 permutations (12 + 4 repeated)

		if (permutation_value & 8)
		{
			u = y;
		}
		else
		{
			u = x;
		}

		if (permutation_value & 4)
		{

			if (permutation_value >= 12)
			{
				v = x;
			}
			else
			{
				v = z;
			}
		}
		else
		{
			v = y;
		}


		if (permutation_value & 1)
		{
			u = -u;
		}

		if (permutation_value & 2)
		{
			v = -v;
		}

		return u + v;
	}

	float PerlinNoise_Impl::gradient_4d(int permutation_value, float x, float y, float z, float t)
	{
		float u, v, w;
		permutation_value = permutation_value & 31;	// Interested in only 31 permutations

		if (permutation_value < 24)
		{
			u = x;
		}
		else
		{
			u = y;
		}

		if (permutation_value < 16)
		{
			v = y;
		}
		else
		{
			v = z;
		}

		if (permutation_value < 8)
		{
			w = z;
		}
		else
		{
			w = t;
		}

		if (permutation_value & 1)
		{
			u = -u;
		}

		if (permutation_value & 2)
		{
			v = -v;
		}

		if (permutation_value & 4)
		{
			w = -w;
		}

		return u + v + w;
	}

	float PerlinNoise_Impl::noise_1d(float x)
	{
		int ix0, ix1;
		float fx0, fx1;
		float s, n0, n1;

		ix0 = cl_floor_to_int(x);
		fx0 = x - ix0;
		fx1 = fx0 - 1.0f;
		ix1 = (ix0 + 1) & cl_period_mask_x;
		ix0 = ix0 & cl_period_mask_x;

		s = cl_s_curve(fx0);

		n0 = gradient_1d(permutation_table[ix0], fx0);
		n1 = gradient_1d(permutation_table[ix1], fx1);
		return (cl_lerp(s, n0, n1));
	}

	float PerlinNoise_Impl::noise_2d(float x, float y)
	{
		int ix0, iy0, ix1, iy1;
		float fx0, fy0, fx1, fy1;
		float s, t, nx0, nx1, n0, n1;

		ix0 = cl_floor_to_int(x);
		iy0 = cl_floor_to_int(y);
		fx0 = x - ix0;
		fy0 = y - iy0;
		fx1 = fx0 - 1.0f;
		fy1 = fy0 - 1.0f;

		ix1 = (ix0 + 1) & cl_period_mask_x;
		iy1 = (iy0 + 1) & cl_period_mask_y;
		ix0 = ix0 & cl_period_mask_x;
		iy0 = iy0 & cl_period_mask_y;

		t = cl_s_curve(fy0);
		s = cl_s_curve(fx0);

		nx0 = gradient_2d(permutation_table[ix0 + permutation_table[iy0]], fx0, fy0);
		nx1 = gradient_2d(permutation_table[ix0 + permutation_table[iy1]], fx0, fy1);
		n0 = cl_lerp(t, nx0, nx1);

		nx0 = gradient_2d(permutation_table[ix1 + permutation_table[iy0]], fx1, fy0);
		nx1 = gradient_2d(permutation_table[ix1 + permutation_table[iy1]], fx1, fy1);
		n1 = cl_lerp(t, nx0, nx1);

		return (cl_lerp(s, n0, n1));
	}

	float PerlinNoise_Impl::noise_3d(float x, float y, float z)
	{
		int ix0, iy0, ix1, iy1, iz0, iz1;
		float fx0, fy0, fz0, fx1, fy1, fz1;
		float s, t, r;
		float nxy0, nxy1, nx0, nx1, n0, n1;

		ix0 = cl_floor_to_int(x);
		iy0 = cl_floor_to_int(y);
		iz0 = cl_floor_to_int(z);
		fx0 = x - ix0;
		fy0 = y - iy0;
		fz0 = z - iz0;
		fx1 = fx0 - 1.0f;
		fy1 = fy0 - 1.0f;
		fz1 = fz0 - 1.0f;
		ix1 = (ix0 + 1) & cl_period_mask_x;
		iy1 = (iy0 + 1) & cl_period_mask_y;
		iz1 = (iz0 + 1) & cl_period_mask_z;
		ix0 = ix0 & cl_period_mask_x;
		iy0 = iy0 & cl_period_mask_y;
		iz0 = iz0 & cl_period_mask_z;

		r = cl_s_curve(fz0);
		t = cl_s_curve(fy0);
		s = cl_s_curve(fx0);

		nxy0 = gradient_3d(permutation_table[ix0 + permutation_table[iy0 + permutation_table[iz0]]], fx0, fy0, fz0);
		nxy1 = gradient_3d(permutation_table[ix0 + permutation_table[iy0 + permutation_table[iz1]]], fx0, fy0, fz1);
		nx0 = cl_lerp(r, nxy0, nxy1);

		nxy0 = gradient_3d(permutation_table[ix0 + permutation_table[iy1 + permutation_table[iz0]]], fx0, fy1, fz0);
		nxy1 = gradient_3d(permutation_table[ix0 + permutation_table[iy1 + permutation_table[iz1]]], fx0, fy1, fz1);
		nx1 = cl_lerp(r, nxy0, nxy1);

		n0 = cl_lerp(t, nx0, nx1);

		nxy0 = gradient_3d(permutation_table[ix1 + permutation_table[iy0 + permutation_table[iz0]]], fx1, fy0, fz0);
		nxy1 = gradient_3d(permutation_table[ix1 + permutation_table[iy0 + permutation_table[iz1]]], fx1, fy0, fz1);
		nx0 = cl_lerp(r, nxy0, nxy1);

		nxy0 = gradient_3d(permutation_table[ix1 + permutation_table[iy1 + permutation_table[iz0]]], fx1, fy1, fz0);
		nxy1 = gradient_3d(permutation_table[ix1 + permutation_table[iy1 + permutation_table[iz1]]], fx1, fy1, fz1);
		nx1 = cl_lerp(r, nxy0, nxy1);

		n1 = cl_lerp(t, nx0, nx1);

		return (cl_lerp(s, n0, n1));
	}

	float PerlinNoise_Impl::noise_4d(float x, float y, float z, float w)
	{
		int ix0, iy0, iz0, iw0, ix1, iy1, iz1, iw1;
		float fx0, fy0, fz0, fw0, fx1, fy1, fz1, fw1;
		float s, t, r, q;
		float nxyz0, nxyz1, nxy0, nxy1, nx0, nx1, n0, n1;

		ix0 = cl_floor_to_int(x);
		iy0 = cl_floor_to_int(y);
		iz0 = cl_floor_to_int(z);
		iw0 = cl_floor_to_int(w);
		fx0 = x - ix0;
		fy0 = y - iy0;
		fz0 = z - iz0;
		fw0 = w - iw0;
		fx1 = fx0 - 1.0f;
		fy1 = fy0 - 1.0f;
		fz1 = fz0 - 1.0f;
		fw1 = fw0 - 1.0f;
		ix1 = (ix0 + 1) & cl_period_mask_x;
		iy1 = (iy0 + 1) & cl_period_mask_y;
		iz1 = (iz0 + 1) & cl_period_mask_z;
		iw1 = (iw0 + 1) & cl_period_mask_w;
		ix0 = ix0 & cl_period_mask_x;
		iy0 = iy0 & cl_period_mask_y;
		iz0 = iz0 & cl_period_mask_z;
		iw0 = iw0 & cl_period_mask_w;

		q = cl_s_curve(fw0);
		r = cl_s_curve(fz0);
		t = cl_s_curve(fy0);
		s = cl_s_curve(fx0);

		nxyz0 = gradient_4d(permutation_table[ix0 + permutation_table[iy0 + permutation_table[iz0 + permutation_table[iw0]]]], fx0, fy0, fz0, fw0);
		nxyz1 = gradient_4d(permutation_table[ix0 + permutation_table[iy0 + permutation_table[iz0 + permutation_table[iw1]]]], fx0, fy0, fz0, fw1);
		nxy0 = cl_lerp(q, nxyz0, nxyz1);

		nxyz0 = gradient_4d(permutation_table[ix0 + permutation_table[iy0 + permutation_table[iz1 + permutation_table[iw0]]]], fx0, fy0, fz1, fw0);
		nxyz1 = gradient_4d(permutation_table[ix0 + permutation_table[iy0 + permutation_table[iz1 + permutation_table[iw1]]]], fx0, fy0, fz1, fw1);
		nxy1 = cl_lerp(q, nxyz0, nxyz1);

		nx0 = cl_lerp(r, nxy0, nxy1);

		nxyz0 = gradient_4d(permutation_table[ix0 + permutation_table[iy1 + permutation_table[iz0 + permutation_table[iw0]]]], fx0, fy1, fz0, fw0);
		nxyz1 = gradient_4d(permutation_table[ix0 + permutation_table[iy1 + permutation_table[iz0 + permutation_table[iw1]]]], fx0, fy1, fz0, fw1);
		nxy0 = cl_lerp(q, nxyz0, nxyz1);

		nxyz0 = gradient_4d(permutation_table[ix0 + permutation_table[iy1 + permutation_table[iz1 + permutation_table[iw0]]]], fx0, fy1, fz1, fw0);
		nxyz1 = gradient_4d(permutation_table[ix0 + permutation_table[iy1 + permutation_table[iz1 + permutation_table[iw1]]]], fx0, fy1, fz1, fw1);
		nxy1 = cl_lerp(q, nxyz0, nxyz1);

		nx1 = cl_lerp(r, nxy0, nxy1);

		n0 = cl_lerp(t, nx0, nx1);

		nxyz0 = gradient_4d(permutation_table[ix1 + permutation_table[iy0 + permutation_table[iz0 + permutation_table[iw0]]]], fx1, fy0, fz0, fw0);
		nxyz1 = gradient_4d(permutation_table[ix1 + permutation_table[iy0 + permutation_table[iz0 + permutation_table[iw1]]]], fx1, fy0, fz0, fw1);
		nxy0 = cl_lerp(q, nxyz0, nxyz1);

		nxyz0 = gradient_4d(permutation_table[ix1 + permutation_table[iy0 + permutation_table[iz1 + permutation_table[iw0]]]], fx1, fy0, fz1, fw0);
		nxyz1 = gradient_4d(permutation_table[ix1 + permutation_table[iy0 + permutation_table[iz1 + permutation_table[iw1]]]], fx1, fy0, fz1, fw1);
		nxy1 = cl_lerp(q, nxyz0, nxyz1);

		nx0 = cl_lerp(r, nxy0, nxy1);

		nxyz0 = gradient_4d(permutation_table[ix1 + permutation_table[iy1 + permutation_table[iz0 + permutation_table[iw0]]]], fx1, fy1, fz0, fw0);
		nxyz1 = gradient_4d(permutation_table[ix1 + permutation_table[iy1 + permutation_table[iz0 + permutation_table[iw1]]]], fx1, fy1, fz0, fw1);
		nxy0 = cl_lerp(q, nxyz0, nxyz1);

		nxyz0 = gradient_4d(permutation_table[ix1 + permutation_table[iy1 + permutation_table[iz1 + permutation_table[iw0]]]], fx1, fy1, fz1, fw0);
		nxyz1 = gradient_4d(permutation_table[ix1 + permutation_table[iy1 + permutation_table[iz1 + permutation_table[iw1]]]], fx1, fy1, fz1, fw1);
		nxy1 = cl_lerp(q, nxyz0, nxyz1);

		nx1 = cl_lerp(r, nxy0, nxy1);

		n1 = cl_lerp(t, nx0, nx1);

		return (cl_lerp(s, n0, n1));
	}

	void PerlinNoise_Impl::set_permutations(const unsigned char *table, unsigned int size)
	{
		if ((size == 0) || (table == nullptr))
		{
			permutation_table_set = false;
			return;
		}

		unsigned char *dest = permutation_table;
		unsigned int dest_size = permutation_table_size;

		while (dest_size > 0)
		{
			unsigned int size_to_copy = size;
			if (size_to_copy > dest_size)
				size_to_copy = dest_size;

			memcpy(dest, table, size_to_copy);
			dest += size_to_copy;
		}

		// Mirror the table
		memcpy(&permutation_table[permutation_table_size], &permutation_table[0], permutation_table_size);

		permutation_table_set = true;

	}

	void PerlinNoise_Impl::setup()
	{
		if (!permutation_table_set)
		{
			for (int cnt = 0; cnt < permutation_table_size; cnt++)
			{
				permutation_table[cnt] = rand();
			}
			memcpy(&permutation_table[permutation_table_size], &permutation_table[0], permutation_table_size);

			permutation_table_set = true;
		}
	}

	PixelBuffer PerlinNoise_Impl::create_noise2d(float start_x, float end_x, float start_y, float end_y)
	{
		setup();

		if (texture_format == TextureFormat::rgba8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_RGBA8 writer(pbuff);
			create_noise2d(writer, start_x, end_x, start_y, end_y);
			return pbuff;
		}
		if (texture_format == TextureFormat::rgb8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_RGB8 writer(pbuff);
			create_noise2d(writer, start_x, end_x, start_y, end_y);
			return pbuff;
		}
		if (texture_format == TextureFormat::r8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_R8 writer(pbuff);
			create_noise2d(writer, start_x, end_x, start_y, end_y);
			return pbuff;
		}
		if (texture_format == TextureFormat::r32f)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_R32f writer(pbuff);
			create_noise2d(writer, start_x, end_x, start_y, end_y);
			return pbuff;
		}

		throw Exception("texture format is not supported");

	}

	void PerlinNoise_Impl::create_noise2d(PerlinNoise_PixelWriter &writer, float start_x, float end_x, float start_y, float end_y)
	{
		float size_x = end_x - start_x;
		float size_y = end_y - start_y;
		float fheight = (float)height;
		float fwidth = (float)width;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float result = 0.0f;
				float current_amplitude = amplitude;
				float value_x = start_x + (((float)x) * size_x) / fwidth;
				float value_y = start_y + (((float)y) * size_y) / fheight;

				for (int i = 0; i < octaves; i++)
				{
					result += current_amplitude * noise_2d(value_x, value_y);
					value_x *= 2.0f;
					value_y *= 2.0f;
					current_amplitude *= 0.5f;
				}

				writer.write_pixel(result);
			}
			writer.line_end();
		}
	}

	PixelBuffer PerlinNoise_Impl::create_noise1d(float start_x, float end_x)
	{
		setup();

		if (texture_format == TextureFormat::rgba8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_RGBA8 writer(pbuff);
			create_noise1d(writer, start_x, end_x);
			return pbuff;
		}
		if (texture_format == TextureFormat::rgb8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_RGB8 writer(pbuff);
			create_noise1d(writer, start_x, end_x);
			return pbuff;
		}
		if (texture_format == TextureFormat::r8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_R8 writer(pbuff);
			create_noise1d(writer, start_x, end_x);
			return pbuff;
		}
		if (texture_format == TextureFormat::r32f)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_R32f writer(pbuff);
			create_noise1d(writer, start_x, end_x);
			return pbuff;
		}

		throw Exception("texture format is not supported");

	}

	void PerlinNoise_Impl::create_noise1d(PerlinNoise_PixelWriter &writer, float start_x, float end_x)
	{
		float size_x = end_x - start_x;
		float fwidth = (float)width;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float result = 0.0f;
				float current_amplitude = amplitude;
				float value_x = start_x + (((float)x) * size_x) / fwidth;

				for (int i = 0; i < octaves; i++)
				{
					result += current_amplitude * noise_1d(value_x);
					value_x *= 2.0f;
					current_amplitude *= 0.5f;
				}

				writer.write_pixel(result);
			}
			writer.line_end();
		}
	}

	PixelBuffer PerlinNoise_Impl::create_noise3d(float start_x, float end_x, float start_y, float end_y, float z_position)
	{
		setup();

		if (texture_format == TextureFormat::rgba8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_RGBA8 writer(pbuff);
			create_noise3d(writer, start_x, end_x, start_y, end_y, z_position);
			return pbuff;
		}
		if (texture_format == TextureFormat::rgb8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_RGB8 writer(pbuff);
			create_noise3d(writer, start_x, end_x, start_y, end_y, z_position);
			return pbuff;
		}
		if (texture_format == TextureFormat::r8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_R8 writer(pbuff);
			create_noise3d(writer, start_x, end_x, start_y, end_y, z_position);
			return pbuff;
		}
		if (texture_format == TextureFormat::r32f)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_R32f writer(pbuff);
			create_noise3d(writer, start_x, end_x, start_y, end_y, z_position);
			return pbuff;
		}

		throw Exception("sized format is not supported");

	}

	void PerlinNoise_Impl::create_noise3d(PerlinNoise_PixelWriter &writer, float start_x, float end_x, float start_y, float end_y, float z_position)
	{
		float size_x = end_x - start_x;
		float size_y = end_y - start_y;
		float fheight = (float)height;
		float fwidth = (float)width;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float result = 0.0f;
				float current_amplitude = amplitude;
				float value_x = start_x + (((float)x) * size_x) / fwidth;
				float value_y = start_y + (((float)y) * size_y) / fheight;
				float value_z = z_position;

				for (int i = 0; i < octaves; i++)
				{
					result += current_amplitude * noise_3d(value_x, value_y, value_z);
					value_x *= 2.0f;
					value_y *= 2.0f;
					value_z *= 2.0f;
					current_amplitude *= 0.5f;
				}

				writer.write_pixel(result);
			}
			writer.line_end();
		}
	}

	PixelBuffer PerlinNoise_Impl::create_noise4d(float start_x, float end_x, float start_y, float end_y, float z_position, float w_position)
	{
		setup();

		if (texture_format == TextureFormat::rgba8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_RGBA8 writer(pbuff);
			create_noise4d(writer, start_x, end_x, start_y, end_y, z_position, w_position);
			return pbuff;
		}
		if (texture_format == TextureFormat::rgb8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_RGB8 writer(pbuff);
			create_noise4d(writer, start_x, end_x, start_y, end_y, z_position, w_position);
			return pbuff;
		}
		if (texture_format == TextureFormat::r8)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_R8 writer(pbuff);
			create_noise4d(writer, start_x, end_x, start_y, end_y, z_position, w_position);
			return pbuff;
		}
		if (texture_format == TextureFormat::r32f)
		{
			PixelBuffer pbuff(width, height, texture_format);
			PerlinNoise_PixelWriter_R32f writer(pbuff);
			create_noise4d(writer, start_x, end_x, start_y, end_y, z_position, w_position);
			return pbuff;
		}

		throw Exception("sized format is not supported");

	}

	void PerlinNoise_Impl::create_noise4d(PerlinNoise_PixelWriter &writer, float start_x, float end_x, float start_y, float end_y, float z_position, float w_position)
	{
		float size_x = end_x - start_x;
		float size_y = end_y - start_y;
		float fheight = (float)height;
		float fwidth = (float)width;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float result = 0.0f;
				float current_amplitude = amplitude;
				float value_x = start_x + (((float)x) * size_x) / fwidth;
				float value_y = start_y + (((float)y) * size_y) / fheight;
				float value_z = z_position;
				float value_w = w_position;

				for (int i = 0; i < octaves; i++)
				{
					result += current_amplitude * noise_4d(value_x, value_y, value_z, value_w);
					value_x *= 2.0f;
					value_y *= 2.0f;
					value_z *= 2.0f;
					value_w *= 2.0f;
					current_amplitude *= 0.5f;
				}

				writer.write_pixel(result);
			}
			writer.line_end();
		}
	}
}
