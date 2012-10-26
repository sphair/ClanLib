/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

/// \addtogroup clanCompute_Compute clanCompute Compute
/// \{

#pragma once

#include "api_compute.h"
#include <memory>

namespace clan
{

class ComputeEvent;
class ComputeMemoryMap_Impl;

/// \brief Compute memory mapping
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_COMPUTE ComputeMemoryMap
{
/// \name Construction
/// \{
public:
	ComputeMemoryMap();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Mapped memory pointer
	///
	/// This function blocks if the memory has not been mapped yet.
	void *data();

	/// \brief Mapped memory pointer
	///
	/// This function blocks if the memory has not been mapped yet.
	template<typename Type>
	Type *data()
	{
		return static_cast<Type*>(data());
	}

	/// \brief Size of mapped memory
	size_t size() const;

/// \name Operations
/// \{
public:
	/// \brief Unmaps the memory
	ComputeEvent unmap();
/// \}

/// \name Implementation
/// \{
private:
	ComputeMemoryMap(ComputeMemoryMap_Impl *impl);

	std::shared_ptr<ComputeMemoryMap_Impl> impl;
	friend class ComputeCommandQueue;
/// \}
};

}

/// \}
