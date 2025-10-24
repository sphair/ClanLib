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

/// \addtogroup clanSWRender_Display clanSWRender Display
/// \{

#pragma once

#include "api_swrender.h"

class CL_PixelThreadContext;
class CL_PixelPipeline;

/// \brief Interface for commands participating in the rendering pipeline
///
/// \xmlonly !group=SWRender/Display! !header=swrender.h! \endxmlonly
class CL_API_SWRender CL_PixelCommand
{
//!Operations
public:
	virtual ~CL_PixelCommand() { }

	/// \brief Called by each rendering thread in the pipeline to run the command
	virtual void run(CL_PixelThreadContext *context) = 0;

	void *operator new(size_t s, CL_PixelPipeline *p);
	void operator delete(void *obj, CL_PixelPipeline *p);
	void operator delete(void *obj);

	/// \brief Returns the first line to be rendered by the specified core
	static int find_first_line_for_core(int y_start, int core, int num_cores);
};

/// \}
