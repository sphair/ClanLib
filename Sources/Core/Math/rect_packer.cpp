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
**    Kenneth Gangstoe
*/

#include "Core/precomp.h"
#include "API/Core/Math/rect_packer.h"
#include "rect_packer_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_RectPacker Construction:

CL_RectPacker::CL_RectPacker()
{
}

CL_RectPacker::CL_RectPacker(const CL_Size &max_group_size, AllocationPolicy policy)
: impl(new CL_RectPacker_Impl(max_group_size))
{
	set_allocation_policy(policy);
}

CL_RectPacker::~CL_RectPacker()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_RectPacker Attributes:

void CL_RectPacker::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_RectPacker is null");
}

CL_RectPacker::AllocationPolicy CL_RectPacker::get_allocation_policy() const
{
	return impl->allocation_policy;
}

CL_Size CL_RectPacker::get_max_group_size() const
{
	return impl->max_group_size;
}

int CL_RectPacker::get_total_rect_count() const
{
	return impl->get_total_rect_count();
}

int CL_RectPacker::get_rect_count(unsigned int group_index) const
{
	return impl->get_rect_count(group_index);
}

int CL_RectPacker::get_group_count() const
{
	return impl->root_nodes.size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_RectPacker Operations:

void CL_RectPacker::set_allocation_policy(AllocationPolicy policy)
{
	impl->allocation_policy = policy;
}

CL_RectPacker::AllocatedRect CL_RectPacker::add(const CL_Size &size)
{
	return impl->add_new_node(size);
}
