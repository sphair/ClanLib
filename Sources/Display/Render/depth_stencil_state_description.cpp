/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "Display/precomp.h"
#include "API/Display/Render/depth_stencil_state_description.h"

namespace clan
{
	class DepthStencilStateDescription_Impl
	{
	public:
		DepthStencilStateDescription_Impl()
			: enable_stencil_test(),
			stencil_front_func(), stencil_front_ref(), stencil_front_mask(),
			stencil_back_func(), stencil_back_ref(), stencil_back_mask(),
			stencil_front_write_mask(), stencil_back_write_mask(),
			fail_front(), pass_depth_fail_front(), pass_depth_pass_front(),
			fail_back(), pass_depth_fail_back(), pass_depth_pass_back(),
			enable_depth_test(), enable_depth_write(), depth_func()
		{
			pass_depth_pass_front = stencil_incr;
			pass_depth_fail_front = stencil_keep;
			fail_front = stencil_keep;
			pass_depth_pass_back = stencil_keep;
			pass_depth_fail_back = stencil_keep;
			fail_back = stencil_keep;
			stencil_front_func = compare_lequal;
			stencil_back_func = compare_lequal;
			enable_stencil_test = false;
			stencil_front_ref = 0;
			stencil_back_ref = 0;
			stencil_front_write_mask = 255;
			stencil_back_write_mask = 255;
			stencil_front_mask = 255;
			stencil_back_mask = 255;
			depth_func = compare_lequal;
			enable_depth_write = true;
			enable_depth_test = false;
		}

		bool operator==(const DepthStencilStateDescription_Impl &other) const
		{
			return
				enable_stencil_test == other.enable_stencil_test &&
				stencil_front_func == other.stencil_front_func &&
				stencil_front_ref == other.stencil_front_ref &&
				stencil_front_mask == other.stencil_front_mask &&
				stencil_back_func == other.stencil_back_func &&
				stencil_back_ref == other.stencil_back_ref &&
				stencil_back_mask == other.stencil_back_mask &&
				stencil_front_write_mask == other.stencil_front_write_mask &&
				stencil_back_write_mask == other.stencil_back_write_mask &&
				fail_front == other.fail_front &&
				pass_depth_fail_front == other.pass_depth_fail_front &&
				pass_depth_pass_front == other.pass_depth_pass_front &&
				fail_back == other.fail_back &&
				pass_depth_fail_back == other.pass_depth_fail_back &&
				pass_depth_pass_back == other.pass_depth_pass_back &&
				enable_depth_test == other.enable_depth_test &&
				enable_depth_write == other.enable_depth_write &&
				depth_func == other.depth_func;
		}

		bool operator<(const DepthStencilStateDescription_Impl &other) const
		{
			if (enable_stencil_test != other.enable_stencil_test)
				return enable_stencil_test < other.enable_stencil_test;
			else if (stencil_front_func != other.stencil_front_func)
				return stencil_front_func < other.stencil_front_func;
			else if (stencil_front_ref != other.stencil_front_ref)
				return stencil_front_ref < other.stencil_front_ref;
			else if (stencil_front_mask != other.stencil_front_mask)
				return stencil_front_mask < other.stencil_front_mask;
			else if (stencil_back_func != other.stencil_back_func)
				return stencil_back_func < other.stencil_back_func;
			else if (stencil_back_ref != other.stencil_back_ref)
				return stencil_back_ref < other.stencil_back_ref;
			else if (stencil_back_mask != other.stencil_back_mask)
				return stencil_back_mask < other.stencil_back_mask;
			else if (stencil_front_write_mask != other.stencil_front_write_mask)
				return stencil_front_write_mask < other.stencil_front_write_mask;
			else if (stencil_back_write_mask != other.stencil_back_write_mask)
				return stencil_back_write_mask < other.stencil_back_write_mask;
			else if (fail_front != other.fail_front)
				return fail_front < other.fail_front;
			else if (pass_depth_fail_front != other.pass_depth_fail_front)
				return pass_depth_fail_front < other.pass_depth_fail_front;
			else if (pass_depth_pass_front != other.pass_depth_pass_front)
				return pass_depth_pass_front < other.pass_depth_pass_front;
			else if (fail_back != other.fail_back)
				return fail_back < other.fail_back;
			else if (pass_depth_fail_back != other.pass_depth_fail_back)
				return pass_depth_fail_back < other.pass_depth_fail_back;
			else if (pass_depth_pass_back != other.pass_depth_pass_back)
				return pass_depth_pass_back < other.pass_depth_pass_back;
			else if (enable_depth_test != other.enable_depth_test)
				return enable_depth_test < other.enable_depth_test;
			else if (enable_depth_write != other.enable_depth_write)
				return enable_depth_write < other.enable_depth_write;
			else
				return depth_func < other.depth_func;
		}

		bool enable_stencil_test;

		CompareFunction stencil_front_func;
		int stencil_front_ref;
		int stencil_front_mask;

		CompareFunction stencil_back_func;
		int stencil_back_ref;
		int stencil_back_mask;

		unsigned char stencil_front_write_mask;
		unsigned char stencil_back_write_mask;

		StencilOp fail_front;
		StencilOp pass_depth_fail_front;
		StencilOp pass_depth_pass_front;
		StencilOp fail_back;
		StencilOp pass_depth_fail_back;
		StencilOp pass_depth_pass_back;

		bool enable_depth_test;
		bool enable_depth_write;
		CompareFunction depth_func;
	};

	DepthStencilStateDescription::DepthStencilStateDescription()
		: impl(std::make_shared<DepthStencilStateDescription_Impl>())
	{
	}

	DepthStencilStateDescription DepthStencilStateDescription::clone() const
	{
		DepthStencilStateDescription copy;
		if (impl)
			copy.impl = std::shared_ptr<DepthStencilStateDescription_Impl>(new DepthStencilStateDescription_Impl(*impl.get()));
		return copy;
	}

	bool DepthStencilStateDescription::is_stencil_test_enabled() const
	{
		return impl->enable_stencil_test;
	}

	void DepthStencilStateDescription::get_stencil_compare_front(CompareFunction &out_front, int &out_front_ref, int &out_front_mask) const
	{
		out_front = impl->stencil_front_func;
		out_front_ref = impl->stencil_front_ref;
		out_front_mask = impl->stencil_front_mask;
	}

	void DepthStencilStateDescription::get_stencil_compare_back(CompareFunction &out_back, int &out_back_ref, int &out_back_mask) const
	{
		out_back = impl->stencil_back_func;
		out_back_ref = impl->stencil_back_ref;
		out_back_mask = impl->stencil_back_mask;
	}

	void DepthStencilStateDescription::get_stencil_write_mask(unsigned char &out_front_facing_mask, unsigned char &out_back_facing_mask) const
	{
		out_front_facing_mask = impl->stencil_front_write_mask;
		out_back_facing_mask = impl->stencil_back_write_mask;
	}

	void DepthStencilStateDescription::get_stencil_op_front(StencilOp &out_fail_front, StencilOp &out_pass_depth_fail_front, StencilOp &out_pass_depth_pass_front) const
	{
		out_fail_front = impl->fail_front;
		out_pass_depth_fail_front = impl->pass_depth_fail_front;
		out_pass_depth_pass_front = impl->pass_depth_pass_front;
	}

	void DepthStencilStateDescription::get_stencil_op_back(StencilOp &out_fail_back, StencilOp &out_pass_depth_fail_back, StencilOp &out_pass_depth_pass_back) const
	{
		out_fail_back = impl->fail_back;
		out_pass_depth_fail_back = impl->pass_depth_fail_back;
		out_pass_depth_pass_back = impl->pass_depth_pass_back;
	}

	bool DepthStencilStateDescription::is_depth_test_enabled() const
	{
		return impl->enable_depth_test;
	}

	bool DepthStencilStateDescription::is_depth_write_enabled() const
	{
		return impl->enable_depth_write;
	}

	CompareFunction DepthStencilStateDescription::get_depth_compare_function() const
	{
		return impl->depth_func;
	}

	void DepthStencilStateDescription::enable_stencil_test(bool enabled)
	{
		impl->enable_stencil_test = enabled;
	}

	void DepthStencilStateDescription::set_stencil_compare_front(CompareFunction front, int front_ref, int front_mask)
	{
		impl->stencil_front_func = front;
		impl->stencil_front_ref = front_ref;
		impl->stencil_front_mask = front_mask;
	}

	void DepthStencilStateDescription::set_stencil_compare_back(CompareFunction back, int back_ref, int back_mask)
	{
		impl->stencil_back_func = back;
		impl->stencil_back_ref = back_ref;
		impl->stencil_back_mask = back_mask;
	}

	void DepthStencilStateDescription::set_stencil_write_mask(unsigned char front_facing_mask, unsigned char back_facing_mask)
	{
		impl->stencil_front_write_mask = front_facing_mask;
		impl->stencil_back_write_mask = back_facing_mask;
	}

	void DepthStencilStateDescription::set_stencil_op_front(StencilOp fail_front, StencilOp pass_depth_fail_front, StencilOp pass_depth_pass_front)
	{
		impl->fail_front = fail_front;
		impl->pass_depth_fail_front = pass_depth_fail_front;
		impl->pass_depth_pass_front = pass_depth_pass_front;
	}

	void DepthStencilStateDescription::set_stencil_op_back(StencilOp fail_back, StencilOp pass_depth_fail_back, StencilOp pass_depth_pass_back)
	{
		impl->fail_back = fail_back;
		impl->pass_depth_fail_back = pass_depth_fail_back;
		impl->pass_depth_pass_back = pass_depth_pass_back;
	}

	void DepthStencilStateDescription::enable_depth_test(bool enabled)
	{
		impl->enable_depth_test = enabled;
	}

	void DepthStencilStateDescription::enable_depth_write(bool enabled)
	{
		impl->enable_depth_write = enabled;
	}

	void DepthStencilStateDescription::set_depth_compare_function(CompareFunction func)
	{
		impl->depth_func = func;
	}

	bool DepthStencilStateDescription::operator==(const DepthStencilStateDescription &other) const
	{
		return impl == other.impl || *impl.get() == *other.impl.get();
	}

	bool DepthStencilStateDescription::operator<(const DepthStencilStateDescription &other) const
	{
		return *impl.get() < *other.impl.get();
	}
}
