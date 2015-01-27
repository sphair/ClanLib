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

#include "UI/precomp.h"
#include "API/UI/Style/style.h"
#include "API/Display/Font/font.h"
#include "style_impl.h"

namespace clan
{
	Style::Style() : impl(new StyleImpl())
	{
	}

	Style::~Style()
	{
	}

	const std::shared_ptr<Style> &Style::get_base()
	{
		return impl->base;
	}

	void Style::set_base(const std::shared_ptr<Style> &new_base)
	{
		impl->base = new_base;
	}

	void Style::set(const std::string &property_name, const std::string &property_value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		StyleProperty::parse(impl.get(), property_name, property_value, args);
	}

	bool Style::has(const std::string &property_name) const
	{
		return false;
	}

	void Style::remove(const std::string &property_name)
	{
	}

	bool Style::is(const std::string &property_name, StyleValueType value_type) const
	{
		return false;
	}

	bool Style::is(const std::string &property_name, const std::string &keyword) const
	{
		return false;
	}

	StyleValueType Style::value_type(const std::string &property_name) const
	{
		return StyleValueType::undefined;
	}

	const std::string &Style::keyword(const std::string &property_name) const
	{

		static std::string not_found;
		return not_found;
	}

	float Style::length(const std::string &property_name) const
	{

		return 0.0f;
	}

	float Style::percentage(const std::string &property_name) const
	{

		return 0.0f;
	}

	const std::string &Style::string(const std::string &property_name) const
	{

		static std::string not_found;
		return not_found;
	}

	const std::string &Style::url(const std::string &property_name) const
	{

		static std::string not_found;
		return not_found;
	}

	const Colorf &Style::color(const std::string &property_name) const
	{

		static Colorf not_found;
		return not_found;
	}

	const std::shared_ptr<ImageSource> &Style::image(const std::string &property_name) const
	{

		static std::shared_ptr<ImageSource> not_found;
		return not_found;
	}

	void Style::render_background(Canvas &canvas, const BoxGeometry &geometry) const
	{
	}

	void Style::render_border(Canvas &canvas, const BoxGeometry &geometry) const
	{
	}

	Font Style::get_font(Canvas &canvas)
	{
		return Font();
	}

	/////////////////////////////////////////////////////////////////////////

	void StyleImpl::set_keyword(const std::string &name, const std::string &keyword)
	{
	}

	void StyleImpl::set_length(const std::string &name, float length)
	{
	}

	void StyleImpl::set_percentage(const std::string &name, float length)
	{
	}

	void StyleImpl::set_string(const std::string &name, const std::string &str)
	{
	}

	void StyleImpl::set_url(const std::string &name, const std::string &url)
	{
	}

	void StyleImpl::set_color(const std::string &name, const Colorf &color)
	{
	}

	void StyleImpl::set_image(const std::string &name, const std::shared_ptr<ImageSource> &image)
	{
	}
}
