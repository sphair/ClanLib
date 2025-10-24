/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

struct CL_DrawImageParams
{
	int dest_buffer_width;
	int dest_buffer_height;
	int src_buffer_width;
	int src_buffer_height;
	unsigned int *dest_data;
	const unsigned int *src_data;

	int primred;
	int primgreen;
	int primblue;
	int primalpha;

	int start_x;
	int end_x;
	int start_y;
	int end_y;

	int dest_y;
	int delta_x;
	int delta_y;

	int src_x;
	int src_y;
	int dest_width;
	int dest_height;
	int src_width;
	int src_height;

	int num_cores;
};

void cl_draw_image_noscale_white(CL_DrawImageParams *params);
void cl_draw_image_noscale(CL_DrawImageParams *params);
void cl_draw_image_nearest_white(CL_DrawImageParams *params);
void cl_draw_image_nearest(CL_DrawImageParams *params);
