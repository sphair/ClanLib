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
**    Mark Page
*/

/// \addtogroup clanSWRender_Display clanSWRender Display
/// \{

#pragma once

#include "api_swrender.h"
#include "../Core/System/sharedptr.h"
#include "../Display/Render/graphic_context.h"
#include "../Display/Render/program_object.h"
#include "software_program.h"

class CL_SWRenderProgramObjectProvider;

/// \brief SWRender Graphic Context
///
/// \xmlonly !group=SWRender/Display! !header=swrender.h! \endxmlonly
class CL_API_SWRender CL_ProgramObject_SWRender : public CL_ProgramObject
{
//!Construction
public:
	// \brief Create a null instance
	CL_ProgramObject_SWRender() {}

	/// \brief Create a SWRender specific graphics context
	///
	/// \param program = The software program
	/// \param is_sprite_program = true when the specified triangles will form rectangles (This is slightly faster)
	CL_ProgramObject_SWRender(CL_SoftwareProgram *program, bool is_sprite_program);

	/// \brief Create a SWRender specific program object
	///
	/// An exception is thrown if the program object is not a SWRender object
	CL_ProgramObject_SWRender(const CL_ProgramObject &program_object);

	~CL_ProgramObject_SWRender();

//!Attributes
public:

	/// \brief Get the attached program
	CL_SoftwareProgram *get_program() const;

	/// \brief Returns true if this program object was created as a sprite program
	bool is_sprite_program() const;

	/// \brief Retrieves the provider.
	CL_SWRenderProgramObjectProvider *get_provider() const;


//!Operations
public:

//!Implementation
private:

};

/// \}
