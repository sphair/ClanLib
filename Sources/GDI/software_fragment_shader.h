
#pragma once

#include "pixel_pipeline.h"

class CL_SoftwareFragmentShader
{
public:
	int x_start, x_end;
	float x_offset;
	float dx_varying[CL_PixelPipeline::num_varying];
	float cur_varying[CL_PixelPipeline::num_varying];
	CL_Size sampler0_size;
	unsigned int *sampler0_data;
	unsigned int *colorbuffer0_line;

	unsigned int get_sampler0_color(float s, float t)
	{
		int tx = texture_clamp(s, sampler0_size.width);
		int ty = texture_clamp(t, sampler0_size.height);
		return sampler0_data[tx+ty*sampler0_size.width];
	}

	int texture_clamp(float v, int size)
	{
		int t = static_cast<int>(v*size)%size;
		if (t < 0)
			t += size;
		return t;
	}

	void shade_and_blend()
	{
		unsigned char *line = reinterpret_cast<unsigned char*>(colorbuffer0_line+x_start);
		int length = x_end-x_start;

		const int sampler_width = sampler0_size.width;
		const int sampler_height = sampler0_size.height;

		int cur_varying_0i = (int)(cur_varying[0] * sampler_width * 65536);
		int cur_varying_1i = (int)(cur_varying[1] * sampler_height * 65536);
		unsigned int cur_varying_2i = (unsigned int)(cur_varying[2] * 256);
		unsigned int cur_varying_3i = (unsigned int)(cur_varying[3] * 256);
		unsigned int cur_varying_4i = (unsigned int)(cur_varying[4] * 256);
		unsigned int cur_varying_5i = (unsigned int)(cur_varying[5] * 256);

		const int dx_varying_0i = (int)(dx_varying[0] * sampler_width * 65536);
		const int dx_varying_1i = (int)(dx_varying[1] * sampler_height * 65536);
		const unsigned int dx_varying_2i = (unsigned int)(dx_varying[2] * 256);
		const unsigned int dx_varying_3i = (unsigned int)(dx_varying[3] * 256);
		const unsigned int dx_varying_4i = (unsigned int)(dx_varying[4] * 256);
		const unsigned int dx_varying_5i = (unsigned int)(dx_varying[5] * 256);

		const int sampler_widthi = sampler_width << 16;

		if (dx_varying_0i >= 0 && dx_varying_1i == 0 && dx_varying_2i == 0 && dx_varying_3i == 0 && dx_varying_4i == 0 && dx_varying_5i == 0)
		{
			while ((cur_varying_0i) < 0) cur_varying_0i += sampler_width; while ((cur_varying_0i) >= sampler_widthi) cur_varying_0i -= sampler_widthi;

			const int texture_clamp_y = cur_varying_1i >> 16;
			unsigned int *line_ptr = (sampler0_data + texture_clamp_y*sampler_width);

			if (cur_varying_2i == 256 && cur_varying_3i == 256 && cur_varying_4i == 256 && cur_varying_5i == 256)
			{
				// Specialization white, no rotation

				for (int i = 0; i < length; i++)
				{
					while ((cur_varying_0i) < 0) cur_varying_0i += sampler_widthi;
					const int texture_clamp_x = cur_varying_0i >> 16;
					unsigned char *read_ptr = reinterpret_cast<unsigned char*> (line_ptr + texture_clamp_x);

					const unsigned int sblue = *(read_ptr++);
					const unsigned int sgreen = *(read_ptr++);
					const unsigned int sred = *(read_ptr++);
					const unsigned int salpha = *(read_ptr);
					const unsigned int neg_alpha = 255 - salpha;

					*line = (*line * neg_alpha + sblue * salpha) >> 8; ++line;
					*line = (*line * neg_alpha + sgreen * salpha) >> 8; ++line;
					*line = (*line * neg_alpha + sred * salpha) >> 8; ++line;
					*line = (*line * neg_alpha + salpha * salpha) >> 8; ++line;

					cur_varying_0i += dx_varying_0i;
				}
			}
			else
			{
				// Specialization colored, no rotation

				for (int i = 0; i < length; i++)
				{
					while ((cur_varying_0i) < 0) cur_varying_0i += sampler_widthi;
					const int texture_clamp_x = cur_varying_0i >> 16;
					unsigned char *read_ptr = reinterpret_cast<unsigned char*> (line_ptr + texture_clamp_x);

					const unsigned int sblue = (*(read_ptr++) * cur_varying_4i) >> 8;
					const unsigned int sgreen = (*(read_ptr++) * cur_varying_3i) >> 8;
					const unsigned int sred = (*(read_ptr++) * cur_varying_2i) >> 8;
					const unsigned int salpha = (*(read_ptr) * cur_varying_5i) >> 8;
					const unsigned int neg_alpha = 255 - salpha;

					*line = (*line * neg_alpha + sblue * salpha) >> 8; ++line;
					*line = (*line * neg_alpha + sgreen * salpha) >> 8; ++line;
					*line = (*line * neg_alpha + sred * salpha) >> 8; ++line;
					*line = (*line * neg_alpha + salpha * salpha) >> 8; ++line;

					cur_varying_0i += dx_varying_0i;
				}
			}
		}
		else
		{
			for (int i = 0; i < length; i++)
			{
				// Possible optimisation - Do: (v-floorf(v))*size;

				while ((cur_varying_0i) < 0) cur_varying_0i += sampler_widthi; while ((cur_varying_0i) >= sampler_widthi) cur_varying_0i -= sampler_widthi;

				const int texture_clamp_x = cur_varying_0i >> 16;
				const int texture_clamp_y = cur_varying_1i >> 16;

				unsigned char *read_ptr = reinterpret_cast<unsigned char*> (sampler0_data + texture_clamp_x + texture_clamp_y*sampler_width);
				const unsigned int sblue = (*(read_ptr++) * cur_varying_4i) >> 8;
				const unsigned int sgreen = (*(read_ptr++) * cur_varying_3i) >> 8;
				const unsigned int sred = (*(read_ptr++) * cur_varying_2i) >> 8;
				const unsigned int salpha = (*(read_ptr) * cur_varying_5i) >> 8;
				const unsigned int neg_alpha = 255 - salpha;

				*line = (*line * neg_alpha + sblue * salpha) >> 8; ++line;
				*line = (*line * neg_alpha + sgreen * salpha) >> 8; ++line;
				*line = (*line * neg_alpha + sred * salpha) >> 8; ++line;
				*line = (*line * neg_alpha + salpha * salpha) >> 8; ++line;

				cur_varying_0i += dx_varying_0i;
				cur_varying_1i += dx_varying_1i;
				cur_varying_2i += dx_varying_2i;
				cur_varying_3i += dx_varying_3i;
				cur_varying_4i += dx_varying_4i;
				cur_varying_5i += dx_varying_5i;
			}
		}
	}
};
