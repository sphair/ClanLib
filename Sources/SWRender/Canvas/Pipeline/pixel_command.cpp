/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "SWRender/precomp.h"
#include "API/SWRender/pixel_command.h"
#include "pixel_pipeline.h"

void *CL_PixelCommand::operator new(size_t s, CL_PixelPipeline *p)
{
	char *d = (char *) p->alloc_command(s+sizeof(CL_PixelPipeline *));
	CL_PixelPipeline **pcopy = (CL_PixelPipeline **) d;
	*pcopy = p;
	d += sizeof(CL_PixelPipeline *);
	return d;
}

void CL_PixelCommand::operator delete(void *obj, CL_PixelPipeline *p)
{
	if (obj)
	{
		char *d = (char *) obj;
		d -= sizeof(CL_PixelPipeline *);
		p->free_command(d);
	}
}

void CL_PixelCommand::operator delete(void *obj)
{
	if (obj)
	{
		char *d = (char *) obj;
		d -= sizeof(CL_PixelPipeline *);
		CL_PixelPipeline **pcopy = (CL_PixelPipeline **) d;
		(*pcopy)->free_command(d);
	}
}

int CL_PixelCommand::find_first_line_for_core(int y_start, int core, int num_cores)
{
	int y = y_start / num_cores;
	y *= num_cores;
	y += core;
	if (y < y_start)
		y += num_cores;
	return y;
}
