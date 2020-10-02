/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "API/Display/Render/blend_state_description.h"

namespace clan
{
	class BlendStateDescription_Impl
	{
	public:
		BlendStateDescription_Impl()
			: enable_blending(true), equation_color(), equation_alpha(), func_src(), func_dest(), func_src_alpha(), func_dest_alpha(), write_red(true), write_green(true), write_blue(true), write_alpha(true),
			logic_op_enabled(false), logic_op(LogicOp::copy)
		{
			func_src = BlendFunc::src_alpha;
			func_dest = BlendFunc::one_minus_src_alpha;
			func_src_alpha = BlendFunc::one;
			func_dest_alpha = BlendFunc::one_minus_src_alpha;
			equation_color = BlendEquation::add;
			equation_alpha = BlendEquation::add;
		}

		bool operator==(const BlendStateDescription_Impl &other) const
		{
			return
				enable_blending == other.enable_blending &&
				equation_color == other.equation_color &&
				equation_alpha == other.equation_alpha &&
				func_src == other.func_src &&
				func_dest == other.func_dest &&
				func_src_alpha == other.func_src_alpha &&
				func_dest_alpha == other.func_dest_alpha &&
				write_red == other.write_red &&
				write_green == other.write_green &&
				write_blue == other.write_blue &&
				write_alpha == other.write_alpha &&
				logic_op_enabled == other.logic_op_enabled &&
				logic_op == other.logic_op;
		}

		bool operator<(const BlendStateDescription_Impl &other) const
		{
			if (enable_blending != other.enable_blending)
				return enable_blending < other.enable_blending;
			else if (equation_color != other.equation_color)
				return equation_color < other.equation_color;
			else if (equation_alpha != other.equation_alpha)
				return equation_alpha < other.equation_alpha;
			else if (func_src != other.func_src)
				return func_src < other.func_src;
			else if (func_dest != other.func_dest)
				return func_dest < other.func_dest;
			else if (func_src_alpha != other.func_src_alpha)
				return func_src_alpha < other.func_src_alpha;
			else if (func_dest_alpha != other.func_dest_alpha)
				return func_dest_alpha < other.func_dest_alpha;
			else if (write_red != other.write_red)
				return write_red < other.write_red;
			else if (write_green != other.write_green)
				return write_green < other.write_green;
			else if (write_blue != other.write_blue)
				return write_blue < other.write_blue;
			else if (write_alpha != other.write_alpha)
				return write_alpha < other.write_alpha;
			else if (logic_op_enabled != other.logic_op_enabled)
				return logic_op_enabled < other.logic_op_enabled;
			else
				return logic_op < other.logic_op;
		}

		bool enable_blending;
		BlendEquation equation_color;
		BlendEquation equation_alpha;
		BlendFunc func_src;
		BlendFunc func_dest;
		BlendFunc func_src_alpha;
		BlendFunc func_dest_alpha;
		bool write_red, write_green, write_blue, write_alpha;
		bool logic_op_enabled;
		LogicOp logic_op;

	};

	BlendStateDescription::BlendStateDescription()
		: impl(std::make_shared<BlendStateDescription_Impl>())
	{
	}

	BlendStateDescription BlendStateDescription::clone() const
	{
		BlendStateDescription copy;
		if (impl)
			copy.impl = std::shared_ptr<BlendStateDescription_Impl>(new BlendStateDescription_Impl(*impl.get()));
		return copy;
	}

	BlendStateDescription BlendStateDescription::blend(bool src_premultiplied, bool dest_premultiplied)
	{
		BlendStateDescription desc;
		if (src_premultiplied)
		{
			desc.impl->func_src = BlendFunc::one;
		}
		else
		{
			desc.impl->func_src = BlendFunc::src_alpha;
		}

		desc.impl->func_dest = BlendFunc::one_minus_src_alpha;
		if (dest_premultiplied)
		{
			desc.impl->func_src_alpha = BlendFunc::one;
		}
		else
		{
			desc.impl->func_src_alpha = BlendFunc::src_alpha;
		}
		desc.impl->func_dest_alpha = BlendFunc::one_minus_src_alpha;

		return desc;

	}

	BlendStateDescription BlendStateDescription::opaque()
	{
		BlendStateDescription desc;
		desc.enable_blending(false);
		return desc;
	}


	bool BlendStateDescription::is_blending_enabled() const
	{
		return impl->enable_blending;
	}

	bool BlendStateDescription::is_logic_op_enabled() const
	{
		return impl->logic_op_enabled;
	}

	LogicOp BlendStateDescription::get_logic_op() const
	{
		return impl->logic_op;
	}

	void BlendStateDescription::get_blend_equation(BlendEquation &out_color, BlendEquation &out_alpha) const
	{
		out_color = impl->equation_color;
		out_alpha = impl->equation_alpha;
	}

	void BlendStateDescription::get_blend_function(BlendFunc &out_src, BlendFunc &out_dest, BlendFunc &out_src_alpha, BlendFunc &out_dest_alpha) const
	{
		out_src = impl->func_src;
		out_dest = impl->func_dest;
		out_src_alpha = impl->func_src_alpha;
		out_dest_alpha = impl->func_dest_alpha;
	}

	void BlendStateDescription::get_color_write(bool &out_red, bool &out_green, bool &out_blue, bool &out_alpha) const
	{
		out_red = impl->write_red;
		out_green = impl->write_green;
		out_blue = impl->write_blue;
		out_alpha = impl->write_alpha;
	}

	void BlendStateDescription::enable_blending(bool value)
	{
		impl->enable_blending = value;
	}

	void BlendStateDescription::set_blend_equation(BlendEquation color, BlendEquation alpha)
	{
		impl->equation_color = color;
		impl->equation_alpha = alpha;
	}

	void BlendStateDescription::set_blend_function(BlendFunc src, BlendFunc dest, BlendFunc src_alpha, BlendFunc dest_alpha)
	{
		impl->func_src = src;
		impl->func_dest = dest;
		impl->func_src_alpha = src_alpha;
		impl->func_dest_alpha = dest_alpha;
	}

	void BlendStateDescription::enable_color_write(bool red, bool green, bool blue, bool alpha)
	{
		impl->write_red = red;
		impl->write_green = green;
		impl->write_blue = blue;
		impl->write_alpha = alpha;
	}

	void BlendStateDescription::enable_logic_op(bool enabled)
	{
		impl->logic_op_enabled = enabled;
	}

	void BlendStateDescription::set_logic_op(LogicOp op)
	{
		impl->logic_op = op;
	}

	bool BlendStateDescription::operator==(const BlendStateDescription &other) const
	{
		return impl == other.impl || *impl.get() == *other.impl.get();
	}

	bool BlendStateDescription::operator<(const BlendStateDescription &other) const
	{
		return *impl.get() < *other.impl.get();
	}
}
