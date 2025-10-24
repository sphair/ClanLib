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
**    Harry Storbacka
*/

/// \addtogroup clanDisplay_Input clanDisplay Input
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"

/// \brief Occlusion query provider.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_OcclusionQueryProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_OcclusionQueryProvider() { return; }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if the GPU is ready to return the result.
	virtual bool is_result_ready() const = 0;

	/// \brief Returns the result of the occlusion query.
	virtual int get_result() const = 0;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Start occlusion query.
	virtual void begin() = 0;

	/// \brief Finish occlusion query.
	virtual void end() = 0;

	/// \brief Create occlusion query object.
	virtual void create() = 0;

	/// \brief Destroy occlusion query provider.
	virtual void destroy() = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
