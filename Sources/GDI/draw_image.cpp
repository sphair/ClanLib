
#include "GDI/precomp.h"
#include "draw_image.h"
#ifdef _MSC_VER
#include <emmintrin.h>
#endif

#define alpha_component(a) (((a)&0xff000000)>>24)
#define red_component(a) (((a)&0x00ff0000)>>16)
#define green_component(a) (((a)&0x0000ff00)>>8)
#define blue_component(a) ((a)&0x000000ff)

void cl_draw_image_noscale_white(CL_DrawImageParams *params)
{
	int start_src_x = params->src_x+params->delta_x;
	int start_src_y = params->src_y+params->delta_y;

	unsigned int *dest_line = params->dest_data+params->dest_y*params->dest_buffer_width+params->start_x;
	const unsigned int *src_line = params->src_data+start_src_y*params->src_buffer_width+start_src_x;

	int line_length = params->end_x-params->start_x;
	int dest_line_incr = params->dest_buffer_width * params->num_cores;
	int src_line_incr = params->src_buffer_width * params->num_cores;

	int num_lines = (params->end_y-params->dest_y+params->num_cores-1) / params->num_cores;
	for (int i = 0; i < num_lines; i++)
	{
		for (int x = 0; x < line_length; x++)
		{
#ifdef STUFF_THAT_DOESNT_WORK
			// __m128i spacked1 = _mm_loadu_si128(src_line+x);
			// __m128i spacked2 = _mm_loadu_si128(src_line+x+4);

			__m128i salpha = _mm_set_epi16(
				alpha_component(src_line[x+0]),
				alpha_component(src_line[x+1]),
				alpha_component(src_line[x+2]),
				alpha_component(src_line[x+3]),
				alpha_component(src_line[x+4]),
				alpha_component(src_line[x+5]),
				alpha_component(src_line[x+6]),
				alpha_component(src_line[x+7]));

			__m128i sred = _mm_set_epi16(
				red_component(src_line[x+0]),
				red_component(src_line[x+1]),
				red_component(src_line[x+2]),
				red_component(src_line[x+3]),
				red_component(src_line[x+4]),
				red_component(src_line[x+5]),
				red_component(src_line[x+6]),
				red_component(src_line[x+7]));

			__m128i sgreen = _mm_set_epi16(
				green_component(src_line[x+0]),
				green_component(src_line[x+1]),
				green_component(src_line[x+2]),
				green_component(src_line[x+3]),
				green_component(src_line[x+4]),
				green_component(src_line[x+5]),
				green_component(src_line[x+6]),
				green_component(src_line[x+7]));

			__m128i sblue = _mm_set_epi16(
				blue_component(src_line[x+0]),
				blue_component(src_line[x+1]),
				blue_component(src_line[x+2]),
				blue_component(src_line[x+3]),
				blue_component(src_line[x+4]),
				blue_component(src_line[x+5]),
				blue_component(src_line[x+6]),
				blue_component(src_line[x+7]));

//			__m128i dalpha = salpha;
//			__m128i dred = sred;
//			__m128i dgreen = sgreen;
//			__m128i dblue = sblue;

			__m128i dalpha = _mm_set_epi16(
				alpha_component(dest_line[x+0]),
				alpha_component(dest_line[x+1]),
				alpha_component(dest_line[x+2]),
				alpha_component(dest_line[x+3]),
				alpha_component(dest_line[x+4]),
				alpha_component(dest_line[x+5]),
				alpha_component(dest_line[x+6]),
				alpha_component(dest_line[x+7]));

			__m128i dred = _mm_set_epi16(
				red_component(dest_line[x+0]),
				red_component(dest_line[x+1]),
				red_component(dest_line[x+2]),
				red_component(dest_line[x+3]),
				red_component(dest_line[x+4]),
				red_component(dest_line[x+5]),
				red_component(dest_line[x+6]),
				red_component(dest_line[x+7]));

			__m128i dgreen = _mm_set_epi16(
				green_component(dest_line[x+0]),
				green_component(dest_line[x+1]),
				green_component(dest_line[x+2]),
				green_component(dest_line[x+3]),
				green_component(dest_line[x+4]),
				green_component(dest_line[x+5]),
				green_component(dest_line[x+6]),
				green_component(dest_line[x+7]));

			__m128i dblue = _mm_set_epi16(
				blue_component(dest_line[x+0]),
				blue_component(dest_line[x+1]),
				blue_component(dest_line[x+2]),
				blue_component(dest_line[x+3]),
				blue_component(dest_line[x+4]),
				blue_component(dest_line[x+5]),
				blue_component(dest_line[x+6]),
				blue_component(dest_line[x+7]));

			__m128i pos_salpha = salpha; // _mm_sll_epi16(salpha, 8);// * 256 / 255;
			__m128i neg_salpha = _mm_sub_epi16(_mm_set1_epi16(255), pos_salpha);

			sred = _mm_mulhi_epi16(sred, pos_salpha);
			sred = _mm_mullo_epi16(sred, pos_salpha);
			sgreen = _mm_mulhi_epi16(sgreen, pos_salpha);
			sgreen = _mm_mullo_epi16(sgreen, pos_salpha);
			sblue = _mm_mulhi_epi16(sblue, pos_salpha);
			sblue = _mm_mullo_epi16(sblue, pos_salpha);
			salpha = _mm_mulhi_epi16(salpha, pos_salpha);
			salpha = _mm_mullo_epi16(salpha, pos_salpha);

			dred = _mm_mulhi_epi16(dred, neg_salpha);
			dred = _mm_mullo_epi16(dred, neg_salpha);
			dgreen = _mm_mulhi_epi16(dgreen, neg_salpha);
			dgreen = _mm_mullo_epi16(dgreen, neg_salpha);
			dblue = _mm_mulhi_epi16(dblue, neg_salpha);
			dblue = _mm_mullo_epi16(dblue, neg_salpha);
			dalpha = _mm_mulhi_epi16(dalpha, neg_salpha);
			dalpha = _mm_mullo_epi16(dalpha, neg_salpha);

			dred = _mm_srli_epi16(_mm_add_epi16(dred, sred), 8);
			dgreen = _mm_srli_epi16(_mm_add_epi16(dred, sgreen), 8);
			dblue = _mm_srli_epi16(_mm_add_epi16(dred, sblue), 8);
			dalpha = _mm_srli_epi16(_mm_add_epi16(dred, salpha), 8);

			dest_line[x+0] = (_mm_extract_epi16(dalpha, 7)<<24) + (_mm_extract_epi16(dred, 7)<<16) + (_mm_extract_epi16(dgreen, 7)<<8) + _mm_extract_epi16(dblue, 7);
			dest_line[x+1] = (_mm_extract_epi16(dalpha, 6)<<24) + (_mm_extract_epi16(dred, 6)<<16) + (_mm_extract_epi16(dgreen, 6)<<8) + _mm_extract_epi16(dblue, 6);
			dest_line[x+2] = (_mm_extract_epi16(dalpha, 5)<<24) + (_mm_extract_epi16(dred, 5)<<16) + (_mm_extract_epi16(dgreen, 5)<<8) + _mm_extract_epi16(dblue, 5);
			dest_line[x+3] = (_mm_extract_epi16(dalpha, 4)<<24) + (_mm_extract_epi16(dred, 4)<<16) + (_mm_extract_epi16(dgreen, 4)<<8) + _mm_extract_epi16(dblue, 4);
			dest_line[x+4] = (_mm_extract_epi16(dalpha, 3)<<24) + (_mm_extract_epi16(dred, 3)<<16) + (_mm_extract_epi16(dgreen, 3)<<8) + _mm_extract_epi16(dblue, 3);
			dest_line[x+5] = (_mm_extract_epi16(dalpha, 2)<<24) + (_mm_extract_epi16(dred, 2)<<16) + (_mm_extract_epi16(dgreen, 2)<<8) + _mm_extract_epi16(dblue, 2);
			dest_line[x+6] = (_mm_extract_epi16(dalpha, 1)<<24) + (_mm_extract_epi16(dred, 1)<<16) + (_mm_extract_epi16(dgreen, 1)<<8) + _mm_extract_epi16(dblue, 1);
			dest_line[x+7] = (_mm_extract_epi16(dalpha, 0)<<24) + (_mm_extract_epi16(dred, 0)<<16) + (_mm_extract_epi16(dgreen, 0)<<8) + _mm_extract_epi16(dblue, 0);


/*
			__m128i s = _mm_set_epi32(
				red_component(src_line[x]),
				green_component(src_line[x]),
				blue_component(src_line[x])
				alpha_component(src_line[x]));

			__m128i s = _mm_set1_epi32(src_line[x]);
*/
#endif

			unsigned int sred = red_component(src_line[x]);
			unsigned int sgreen = green_component(src_line[x]);
			unsigned int sblue = blue_component(src_line[x]);
			unsigned int salpha = alpha_component(src_line[x]);
			unsigned int pos_salpha = salpha * 256 / 255;
			unsigned int neg_salpha = 256-pos_salpha;

			unsigned int dest_color = dest_line[x];
			unsigned int dred = red_component(dest_color);
			unsigned int dgreen = green_component(dest_color);
			unsigned int dblue = blue_component(dest_color);
			unsigned int dalpha = alpha_component(dest_color);

			unsigned int red = (dred * neg_salpha + sred * pos_salpha) >> 8;
			unsigned int green = (dgreen * neg_salpha + sgreen * pos_salpha) >> 8;
			unsigned int blue = (dblue * neg_salpha + sblue * pos_salpha) >> 8;
			unsigned int alpha = (dalpha * neg_salpha + salpha * pos_salpha) >> 8;
			dest_line[x] = (alpha<<24) + (red<<16) + (green<<8) + blue;
		}

		dest_line += dest_line_incr;
		src_line += src_line_incr;
	}
}

void cl_draw_image_noscale(CL_DrawImageParams *params)
{
	int start_src_x = params->src_x+params->delta_x;
	int start_src_y = params->src_y+params->delta_y;

	unsigned int *dest_line = params->dest_data+params->dest_y*params->dest_buffer_width+params->start_x;
	const unsigned int *src_line = params->src_data+start_src_y*params->src_buffer_width+start_src_x;

	int line_length = params->end_x-params->start_x;
	int dest_line_incr = params->dest_buffer_width * params->num_cores;
	int src_line_incr = params->src_buffer_width * params->num_cores;

	int num_lines = (params->end_y-params->dest_y+params->num_cores-1) / params->num_cores;
	for (int i = 0; i < num_lines; i++)
	{
		for (int x = 0; x < line_length; x++)
		{
			unsigned int sred = red_component(src_line[x]);
			unsigned int sgreen = green_component(src_line[x]);
			unsigned int sblue = blue_component(src_line[x]);
			unsigned int salpha = alpha_component(src_line[x]);

			sred = (sred * params->primred) >> 8;
			sgreen = (sgreen * params->primgreen) >> 8;
			sblue = (sblue * params->primblue) >> 8;
			salpha = (salpha * params->primalpha) >> 8;

			unsigned int pos_salpha = salpha * 256 / 255;
			unsigned int neg_salpha = 256-pos_salpha;

			unsigned int dest_color = dest_line[x];
			unsigned int dred = red_component(dest_color);
			unsigned int dgreen = green_component(dest_color);
			unsigned int dblue = blue_component(dest_color);
			unsigned int dalpha = alpha_component(dest_color);

			unsigned int red = (dred * neg_salpha + sred * pos_salpha) >> 8;
			unsigned int green = (dgreen * neg_salpha + sgreen * pos_salpha) >> 8;
			unsigned int blue = (dblue * neg_salpha + sblue * pos_salpha) >> 8;
			unsigned int alpha = (dalpha * neg_salpha + salpha * pos_salpha) >> 8;
			dest_line[x] = (alpha<<24) + (red<<16) + (green<<8) + blue;
		}

		dest_line += dest_line_incr;
		src_line += src_line_incr;
	}
}

void cl_draw_image_nearest_white(CL_DrawImageParams *params)
{
	int scale_x = (params->src_width<<16) / params->dest_width;
	int scale_y = (params->src_height<<16) / params->dest_height;

	int start_src_x = (params->src_x<<16)+params->delta_x*scale_x+32768;
	int start_src_y = (params->src_y<<16)+params->delta_y*scale_y+32768;

	unsigned int *dest_line = params->dest_data+params->dest_y*params->dest_buffer_width+params->start_x;

	int line_length = params->end_x-params->start_x;
	int dest_line_incr = params->dest_buffer_width * params->num_cores;
	int src_y = start_src_y;
	int src_y_incr = scale_y * params->num_cores;

	int num_lines = (params->end_y-params->dest_y+params->num_cores-1) / params->num_cores;
	for (int i = 0; i < num_lines; i++)
	{
		const unsigned int *src_line = params->src_data+(src_y>>16)*params->src_buffer_width;
		int sx = start_src_x;
		for (int x = 0; x < line_length; x++)
		{
			unsigned int sred = red_component(src_line[sx>>16]);
			unsigned int sgreen = green_component(src_line[sx>>16]);
			unsigned int sblue = blue_component(src_line[sx>>16]);
			unsigned int salpha = alpha_component(src_line[sx>>16]);

			unsigned int pos_salpha = salpha * 256 / 255;
			unsigned int neg_salpha = 256-pos_salpha;

			unsigned int dest_color = dest_line[x];
			unsigned int dred = red_component(dest_color);
			unsigned int dgreen = green_component(dest_color);
			unsigned int dblue = blue_component(dest_color);
			unsigned int dalpha = alpha_component(dest_color);

			unsigned red = (dred * neg_salpha + sred * pos_salpha) >> 8;
			unsigned green = (dgreen * neg_salpha + sgreen * pos_salpha) >> 8;
			unsigned blue = (dblue * neg_salpha + sblue * pos_salpha) >> 8;
			unsigned alpha = (dalpha * neg_salpha + salpha * pos_salpha) >> 8;
			dest_line[x] = (alpha<<24) + (red<<16) + (green<<8) + blue;
			sx += scale_x;
		}

		dest_line += dest_line_incr;
		src_y += src_y_incr;
	}
}

void cl_draw_image_nearest(CL_DrawImageParams *params)
{
	int scale_x = (params->src_width<<16) / params->dest_width;
	int scale_y = (params->src_height<<16) / params->dest_height;

	int start_src_x = (params->src_x<<16)+params->delta_x*scale_x+32768;
	int start_src_y = (params->src_y<<16)+params->delta_y*scale_y+32768;

	unsigned int *dest_line = params->dest_data+params->dest_y*params->dest_buffer_width+params->start_x;

	int line_length = params->end_x-params->start_x;
	int dest_line_incr = params->dest_buffer_width * params->num_cores;
	int src_y = start_src_y;
	int src_y_incr = scale_y * params->num_cores;

	int num_lines = (params->end_y-params->dest_y+params->num_cores-1) / params->num_cores;
	for (int i = 0; i < num_lines; i++)
	{
		const unsigned int *src_line = params->src_data+(src_y>>16)*params->src_buffer_width;
		int sx = start_src_x;
		for (int x = 0; x < line_length; x++)
		{
			unsigned int sred = red_component(src_line[sx>>16]);
			unsigned int sgreen = green_component(src_line[sx>>16]);
			unsigned int sblue = blue_component(src_line[sx>>16]);
			unsigned int salpha = alpha_component(src_line[sx>>16]);

			sred = (sred * params->primred) >> 8;
			sgreen = (sgreen * params->primgreen) >> 8;
			sblue = (sblue * params->primblue) >> 8;
			salpha = (salpha * params->primalpha) >> 8;

			unsigned int pos_salpha = salpha * 256 / 255;
			unsigned int neg_salpha = 256-pos_salpha;

			unsigned int dest_color = dest_line[x];
			unsigned int dred = red_component(dest_color);
			unsigned int dgreen = green_component(dest_color);
			unsigned int dblue = blue_component(dest_color);
			unsigned int dalpha = alpha_component(dest_color);

			unsigned red = (dred * neg_salpha + sred * pos_salpha) >> 8;
			unsigned green = (dgreen * neg_salpha + sgreen * pos_salpha) >> 8;
			unsigned blue = (dblue * neg_salpha + sblue * pos_salpha) >> 8;
			unsigned alpha = (dalpha * neg_salpha + salpha * pos_salpha) >> 8;
			dest_line[x] = (alpha<<24) + (red<<16) + (green<<8) + blue;
			sx += scale_x;
		}

		dest_line += dest_line_incr;
		src_y += src_y_incr;
	}
}
