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
**    Harry Storbacka
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "graphic_context.h"

class CL_OcclusionQuery_Impl;
class CL_GraphicContext;
class CL_OcclusionQueryProvider;

/// \brief Graphics occlusion query class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_OcclusionQuery
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance.
	CL_OcclusionQuery();

	/// \brief Constructs an occlusion query object.
	CL_OcclusionQuery(CL_GraphicContext &context);

	virtual ~CL_OcclusionQuery();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the result of the occlusion query.
	int get_result();

	/// \brief Returns true if the GPU is ready to return the result.
	bool is_result_ready();

	/// \brief Get Provider
	///
	/// \return provider
	CL_OcclusionQueryProvider *get_provider() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Start occlusion query.
	void begin();

	/// \brief Finish occlusion query.
	void end();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_OcclusionQuery_Impl> impl;
/// \}
};

/// \}
