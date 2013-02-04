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
#include "../Core/Math/vec4.h"

namespace clan
{

class ComputeProgram;
class ComputeBuffer;
class ComputeSampler;
class ComputeKernel_Impl;

/// \brief Compute program kernel object
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_COMPUTE ComputeKernel
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	ComputeKernel();

	/// \brief Constructs a compute program kernel
	///
	/// \param program = Compute program containing the kernel function
	/// \param kernel_name = Name of kernel function
	ComputeKernel(ComputeProgram &program, const std::string &kernel_name);

	~ComputeKernel();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;
/// \}

/// \name Operations
/// \{
public:
	void set_arg(int index, const void *data, int size);
	void set_arg_int(int index, int value);
	void set_arg_float(int index, float value);
	void set_arg_vec2f(int index, const Vec2f &value);
	void set_arg_vec3f(int index, const Vec3f &value);
	void set_arg_vec4f(int index, const Vec4f &value);
	void set_arg_buffer(int index, ComputeBuffer &buffer);
	void set_arg_sampler(int index, ComputeSampler &sampler);
	void set_arg_null(int index);
/// \}

/// \name Implementation
/// \{
private:
	std::shared_ptr<ComputeKernel_Impl> impl;

	friend class ComputeCommandQueue;
/// \}
};

}

/// \}
