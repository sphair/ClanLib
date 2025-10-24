
#include "GDI/precomp.h"
#include "draw_image.h"
#ifdef _MSC_VER
#include <emmintrin.h>
#endif

#define alpha_component(a) (((a)&0xff000000)>>24)
#define red_component(a) (((a)&0x00ff0000)>>16)
#define green_component(a) (((a)&0x0000ff00)>>8)
#define blue_component(a) ((a)&0x000000ff)

/* Temporary disabled the GDI assembly, as it does not render the GUI example correctly on Vista */
#ifdef _M_IX86_DISABLED
//#ifdef _M_IX86

void cl_draw_image_noscale_white(CL_DrawImageParams *params)
{
	const int start_src_x			= params->src_x+params->delta_x;
	const int start_src_y			= params->src_y+params->delta_y;

	const unsigned int	*src_line	= params->src_data  + start_src_y    * params->src_buffer_width  + start_src_x;

	unsigned int		*dest_line	= params->dest_data + params->dest_y * params->dest_buffer_width + params->start_x;

	const int blits_per_line		= (params->end_x-params->start_x) / 2; // how many 64 bits blits to process in a line
	
	const int dest_line_incr_bytes	= params->dest_buffer_width * params->num_cores * 4;
	const int src_line_incr_bytes	= params->src_buffer_width * params->num_cores * 4;

	const int num_lines				= (params->end_y-params->dest_y+params->num_cores-1) / params->num_cores;

	if(num_lines<1 || blits_per_line<1)
		return;

	#pragma pack(16)
	static const __int16 _ff[]={0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100};
	static const __int16 _7f[]={0x007f, 0x007f, 0x007f, 0x007f, 0x007f, 0x007f, 0x007f, 0x007f};
	const void* ff = &_ff;
	#pragma pack()

	__asm
	{
		MOV			esi, src_line;
		MOV			edi, dest_line;
		MOV			edx, num_lines		// EDX: Line count

		// XMM0 = 0, high part of unpacked bytes (unpacked to words)
		PXOR		xmm0, xmm0	

		// XMM1 : 8x 00FF, for inverse alpha 
		MOVLPS     	xmm1, [_ff] 
		MOVHPS     	xmm1, [_ff] 

		// XMM7 : 8x 007F, for round up RGB channels before shift right
		MOVLPS     	xmm7, [_7f] 
		MOVHPS     	xmm7, [_7f] 

	BLEND_Y:
		XOR			eax, eax			// EAX blit byte offset from start of line
		MOV			ecx, blits_per_line	// blit counter

	BLEND_X:

		// XMM2 : Src Pixel s0, s1
		movlpd 		xmm2, [esi+eax]		// XMM2 = 0000000000000000 AARRGGBB AARRGGBB
		punpcklbw	xmm2, xmm0			// XMM2 = 00AA 00RR 00GG 00BB 00AA 00RR 00GG 00BB

		// XMM3 : Dst Pixel d0, d1
		movlpd 		xmm3, [edi+eax]		// XMM3 = 0000000000000000 AARRGGBB AARRGGBB
		PUNPCKLBW	xmm3, xmm0			// XMM3 = 00AA 00RR 00GG 00BB 00AA 00RR 00GG 00BB
		
		// XMM4 : Src Alpha Channel for s0, s1
		pshuflw 	xmm4, xmm2, 0xFF	// XMM4 = 00AB00110011001100AA00AA00AA00AA  
		pshufhw  	xmm4, xmm4, 0xFF	// XMM4 = 00AB00AB00AB00AB00AA00AA00AA00AA

		// XMM5 : Src Inverse Alpha Channel for d0, d1
		movapd		xmm5, xmm1
		psubw		xmm5, xmm4

		// XMM4 = XMM4 * XMM2, Src Blend = Src pix * Src Alpha
		pmullw		xmm4, xmm2

		// XMM5 = XMM5 * XMM3, Dst Blend = Dst pix * Inv Src Alpha
		pmullw		xmm5, xmm3

		// XMM5 = XMM5 + XMM4, Sum Blend
		paddw		xmm5, xmm4
		paddw		xmm5, xmm7 // round up
		
		// Pack and write back
		psrlw		xmm5, 8
		packuswb	xmm6, xmm5 
		movhpd 		[edi+eax],xmm6	// dst <- high qword of XMM3

		// Next blit (8 bytes ahead)
		ADD			eax, 8		
		DEC			ecx				// blit counter
		JNZ			BLEND_X

		// Next Source and Dest lines
		MOV			eax, src_line_incr_bytes
		MOV			ebx, dest_line_incr_bytes
		ADD			esi, eax		
		ADD			edi, ebx	

		DEC			edx				// line counter
		JNZ			BLEND_Y

	}
}

#else

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
#endif 

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

	int start_src_x = (params->src_x<<16)+params->delta_x*scale_x;
	int start_src_y = (params->src_y<<16)+params->delta_y*scale_y;

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

	int start_src_x = (params->src_x<<16)+params->delta_x*scale_x;
	int start_src_y = (params->src_y<<16)+params->delta_y*scale_y;

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
