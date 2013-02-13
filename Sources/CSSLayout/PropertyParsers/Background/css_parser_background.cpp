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

#include "CSSLayout/precomp.h"
#include "css_parser_background.h"
#include "API/CSSLayout/PropertyValues/css_value_background_color.h"
#include "API/CSSLayout/PropertyValues/css_value_background_image.h"
#include "API/CSSLayout/PropertyValues/css_value_background_repeat.h"
#include "API/CSSLayout/PropertyValues/css_value_background_attachment.h"
#include "API/CSSLayout/PropertyValues/css_value_background_position.h"
#include "API/CSSLayout/PropertyValues/css_value_background_origin.h"
#include "API/CSSLayout/PropertyValues/css_value_background_clip.h"
#include "API/CSSLayout/PropertyValues/css_value_background_size.h"

namespace clan
{

std::vector<std::string> CSSParserBackground::get_names()
{
	std::vector<std::string> names;
	names.push_back("background");
	return names;
}

void CSSParserBackground::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueBackgroundColor> bgcolor(new CSSValueBackgroundColor());
	std::unique_ptr<CSSValueBackgroundImage> bgimage(new CSSValueBackgroundImage());
	std::unique_ptr<CSSValueBackgroundRepeat> bgrepeat(new CSSValueBackgroundRepeat());
	std::unique_ptr<CSSValueBackgroundAttachment> bgattachment(new CSSValueBackgroundAttachment());
	std::unique_ptr<CSSValueBackgroundPosition> bgposition(new CSSValueBackgroundPosition());
	std::unique_ptr<CSSValueBackgroundOrigin> bgorigin(new CSSValueBackgroundOrigin());
	std::unique_ptr<CSSValueBackgroundClip> bgclip(new CSSValueBackgroundClip());
	std::unique_ptr<CSSValueBackgroundSize> bgsize(new CSSValueBackgroundSize());

	if (tokens.size() == 1 && tokens[0].type == CSSToken::type_ident && equals(tokens[0].value, "inherit"))
	{
		bgcolor->type = CSSValueBackgroundColor::type_inherit;
		bgimage->type = CSSValueBackgroundImage::type_inherit;
		bgrepeat->type = CSSValueBackgroundRepeat::type_inherit;
		bgattachment->type = CSSValueBackgroundAttachment::type_inherit;
		bgposition->type = CSSValueBackgroundPosition::type_inherit;
		bgorigin->type = CSSValueBackgroundOrigin::type_inherit;
		bgclip->type = CSSValueBackgroundClip::type_inherit;
		bgsize->type = CSSValueBackgroundSize::type_inherit;

		inout_values.push_back(std::move(bgcolor));
		inout_values.push_back(std::move(bgimage));
		inout_values.push_back(std::move(bgrepeat));
		inout_values.push_back(std::move(bgattachment));
		inout_values.push_back(std::move(bgposition));
		inout_values.push_back(std::move(bgorigin));
		inout_values.push_back(std::move(bgclip));
		inout_values.push_back(std::move(bgsize));
		return;
	}

	bgimage->images.clear();
	bgrepeat->repeat_x.clear();
	bgrepeat->repeat_y.clear();
	bgattachment->attachments.clear();
	bgposition->positions.clear();
	bgorigin->values.clear();
	bgclip->values.clear();
	bgsize->values.clear();

	size_t pos = 0;
	while (true) // for each layer
	{
		bool color_specified = false;
		bool image_specified = false;
		bool repeat_specified = false;
		bool attachment_specified = false;
		bool position_specified = false;
		bool boxes_specified = false;

		CSSValueBackgroundImage::Image layer_image(CSSValueBackgroundImage::image_type_none);
		CSSValueBackgroundRepeat::RepeatStyle layer_repeat_x = CSSValueBackgroundRepeat::style_repeat;
		CSSValueBackgroundRepeat::RepeatStyle layer_repeat_y = CSSValueBackgroundRepeat::style_repeat;
		CSSValueBackgroundAttachment::Attachment layer_attachment = CSSValueBackgroundAttachment::attachment_scroll;
		CSSValueBackgroundPosition::Position layer_position;
		CSSValueBackgroundOrigin::OriginType layer_origin = CSSValueBackgroundOrigin::origin_padding_box;
		CSSValueBackgroundClip::ClipType layer_clip = CSSValueBackgroundClip::clip_border_box;
		CSSValueBackgroundSize::Size layer_size;

		while (true) // for each declaration in layer
		{
			if (!color_specified && parse_bgcolor(*bgcolor.get(), pos, tokens))
			{
				color_specified = true;
			}
			else if (!image_specified && parse_image(layer_image, pos, tokens))
			{
				image_specified = true;
			}
			else if (!repeat_specified && parse_repeat(layer_repeat_x, layer_repeat_y, pos, tokens))
			{
				repeat_specified = true;
			}
			else if (!attachment_specified && parse_attachment(layer_attachment, pos, tokens))
			{
				attachment_specified = true;
			}
			else if (!position_specified && parse_position(layer_position, pos, tokens))
			{
				parse_size(layer_size, pos, tokens);
				position_specified = true;
			}
			else if (!boxes_specified && parse_boxes(layer_origin, layer_clip, pos, tokens))
			{
				boxes_specified = true;
			}
			else
			{
				return;
			}

			if (pos == tokens.size())
				break;

			size_t peek_pos = pos;
			CSSToken token = next_token(peek_pos, tokens);
			if (token.type == CSSToken::type_delim && token.value == ",")
				break;
		}

		bgimage->images.push_back(layer_image);
		bgrepeat->repeat_x.push_back(layer_repeat_x);
		bgrepeat->repeat_y.push_back(layer_repeat_y);
		bgattachment->attachments.push_back(layer_attachment);
		bgposition->positions.push_back(layer_position);
		bgorigin->values.push_back(layer_origin);
		bgclip->values.push_back(layer_clip);
		bgsize->values.push_back(layer_size);

		if (pos == tokens.size())
			break;

		CSSToken token = next_token(pos, tokens);
		if (token.type != CSSToken::type_delim || token.value != "," || color_specified)
			return;
	}

	inout_values.push_back(std::move(bgcolor));
	inout_values.push_back(std::move(bgimage));
	inout_values.push_back(std::move(bgrepeat));
	inout_values.push_back(std::move(bgattachment));
	inout_values.push_back(std::move(bgposition));
	inout_values.push_back(std::move(bgorigin));
	inout_values.push_back(std::move(bgclip));
	inout_values.push_back(std::move(bgsize));
}

bool CSSParserBackground::parse_bgcolor(CSSValueBackgroundColor &bgcolor, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	Colorf color;
	if (parse_color(tokens, pos, color))
	{
		bgcolor.type = CSSValueBackgroundColor::type_color;
		bgcolor.color = color;
		parse_pos = pos;
		return true;
	}
	return false;
}

bool CSSParserBackground::parse_image(CSSValueBackgroundImage::Image &layer_image, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && equals(token.value, "none"))
	{
		layer_image = CSSValueBackgroundImage::Image(CSSValueBackgroundImage::image_type_none);
		parse_pos = pos;
		return true;
	}
	else if (token.type == CSSToken::type_uri)
	{
		layer_image.type = CSSValueBackgroundImage::image_type_uri;
		layer_image.url = token.value;
		parse_pos = pos;
		return true;
	}
	return false;
}

bool CSSParserBackground::parse_repeat(CSSValueBackgroundRepeat::RepeatStyle &layer_repeat_x, CSSValueBackgroundRepeat::RepeatStyle &layer_repeat_y, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);

	if (token.type != CSSToken::type_ident)
		return false;

	CSSValueBackgroundRepeat::RepeatStyle repeat_x, repeat_y;
	bool single_style = false;

	if (equals(token.value, "repeat"))
	{
		repeat_x = CSSValueBackgroundRepeat::style_repeat;
	}
	else if (equals(token.value, "repeat-x"))
	{
		repeat_x = CSSValueBackgroundRepeat::style_repeat;
		repeat_y = CSSValueBackgroundRepeat::style_no_repeat;
		single_style = true;
	}
	else if (equals(token.value, "repeat-y"))
	{
		repeat_x = CSSValueBackgroundRepeat::style_no_repeat;
		repeat_y = CSSValueBackgroundRepeat::style_repeat;
		single_style = true;
	}
	else if (equals(token.value, "no-repeat"))
	{
		repeat_x = CSSValueBackgroundRepeat::style_no_repeat;
	}
	else if (equals(token.value, "space"))
	{
		repeat_x = CSSValueBackgroundRepeat::style_space;
	}
	else if (equals(token.value, "round"))
	{
		repeat_x = CSSValueBackgroundRepeat::style_round;
	}
	else
	{
		return false;
	}

	if (pos == tokens.size() || single_style)
	{
		if (!single_style)
			repeat_y = repeat_x;

		layer_repeat_x = repeat_x;
		layer_repeat_y = repeat_y;
		parse_pos = pos;
		return true;
	}

	parse_pos = pos;
	repeat_y = repeat_x;

	token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident)
	{
		if (equals(token.value, "repeat"))
		{
			repeat_y = CSSValueBackgroundRepeat::style_repeat;
			parse_pos = pos;
		}
		else if (equals(token.value, "no-repeat"))
		{
			repeat_y = CSSValueBackgroundRepeat::style_no_repeat;
			parse_pos = pos;
		}
		else if (equals(token.value, "space"))
		{
			repeat_y = CSSValueBackgroundRepeat::style_space;
			parse_pos = pos;
		}
		else if (equals(token.value, "round"))
		{
			repeat_y = CSSValueBackgroundRepeat::style_round;
			parse_pos = pos;
		}
	}

	layer_repeat_x = repeat_x;
	layer_repeat_y = repeat_y;
	return true;
}

bool CSSParserBackground::parse_attachment(CSSValueBackgroundAttachment::Attachment &layer_attachment, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);
	if (token.type != CSSToken::type_ident)
		return false;

	if (equals(token.value, "scroll"))
		layer_attachment = CSSValueBackgroundAttachment::attachment_scroll;
	else if (equals(token.value, "fixed"))
		layer_attachment = CSSValueBackgroundAttachment::attachment_fixed;
	else
		return false;

	parse_pos = pos;
	return true;
}

bool CSSParserBackground::parse_position(CSSValueBackgroundPosition::Position &layer_position, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t last_pos = parse_pos;
	size_t pos = last_pos;
	CSSToken token = next_token(pos, tokens);

	CSSValueBackgroundPosition::Position bg_pos;
	bool x_specified = false;
	bool y_specified = false;
	bool center_specified = false;
	while (true)
	{
		if (token.type == CSSToken::type_ident)
		{
			if (!y_specified && equals(token.value, "top"))
			{
				bg_pos.type_y = CSSValueBackgroundPosition::type2_top;
				y_specified = true;

				if (center_specified)
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
					x_specified = true;
					center_specified = false;
				}
			}
			else if (!y_specified && equals(token.value, "bottom"))
			{
				bg_pos.type_y = CSSValueBackgroundPosition::type2_bottom;
				y_specified = true;

				if (center_specified)
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
					x_specified = true;
					center_specified = false;
				}
			}
			else if (!x_specified && equals(token.value, "left"))
			{
				bg_pos.type_x = CSSValueBackgroundPosition::type1_left;
				x_specified = true;

				if (center_specified)
				{
					bg_pos.type_y = CSSValueBackgroundPosition::type2_center;
					y_specified = true;
					center_specified = false;
				}
			}
			else if (!x_specified && equals(token.value, "right"))
			{
				bg_pos.type_x = CSSValueBackgroundPosition::type1_right;
				x_specified = true;

				if (center_specified)
				{
					bg_pos.type_y = CSSValueBackgroundPosition::type2_center;
					y_specified = true;
					center_specified = false;
				}
			}
			else if (equals(token.value, "center"))
			{
				if (center_specified)
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
					x_specified = true;
					center_specified = false;
				}

				if (x_specified && !y_specified)
				{
					bg_pos.type_y = CSSValueBackgroundPosition::type2_center;
					y_specified = true;
				}
				else if (y_specified && !x_specified)
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
					x_specified = true;
				}
				else if (!x_specified && !y_specified)
				{
					center_specified = true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				break;
			}
		}
		else if (is_length(token))
		{
			CSSLength length;
			if (parse_length(token, length))
			{
				if (center_specified)
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
					x_specified = true;
					center_specified = false;
				}

				if (!x_specified && !y_specified)
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_length;
					bg_pos.length_x = length;
					x_specified = true;
				}
				else if (x_specified && !y_specified)
				{
					bg_pos.type_y = CSSValueBackgroundPosition::type2_length;
					bg_pos.length_y = length;
					y_specified = true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (token.type == CSSToken::type_percentage)
		{
			if (center_specified)
			{
				bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
				x_specified = true;
				center_specified = false;
			}

			if (!x_specified && !y_specified)
			{
				bg_pos.type_x = CSSValueBackgroundPosition::type1_percentage;
				bg_pos.percentage_x = StringHelp::text_to_float(token.value);
				x_specified = true;
			}
			else if (x_specified && !y_specified)
			{
				bg_pos.type_y = CSSValueBackgroundPosition::type2_percentage;
				bg_pos.percentage_y = StringHelp::text_to_float(token.value);
				y_specified = true;
			}
			else
			{
				return false;
			}
		}
		else if (token.type == CSSToken::type_delim && token.value == "-")
		{
			token = next_token(pos, tokens);
			if (is_length(token))
			{
				CSSLength length;
				if (parse_length(token, length))
				{
					length.value = -length.value;
					if (center_specified)
					{
						bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
						x_specified = true;
						center_specified = false;
					}

					if (!x_specified && !y_specified)
					{
						bg_pos.type_x = CSSValueBackgroundPosition::type1_length;
						bg_pos.length_x = length;
						x_specified = true;
					}
					else if (x_specified && !y_specified)
					{
						bg_pos.type_y = CSSValueBackgroundPosition::type2_length;
						bg_pos.length_y = length;
						y_specified = true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else if (token.type == CSSToken::type_percentage)
			{
				if (center_specified)
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
					x_specified = true;
					center_specified = false;
				}

				if (!x_specified && !y_specified)
				{
					bg_pos.type_x = CSSValueBackgroundPosition::type1_percentage;
					bg_pos.percentage_x = -StringHelp::text_to_float(token.value);
					x_specified = true;
					parse_pos = pos;
				}
				else if (x_specified && !y_specified)
				{
					bg_pos.type_y = CSSValueBackgroundPosition::type2_percentage;
					bg_pos.percentage_y = -StringHelp::text_to_float(token.value);
					y_specified = true;
					parse_pos = pos;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			break;
		}

		last_pos = pos;
		token = next_token(pos, tokens);
	}

	if (!x_specified)
		bg_pos.type_x = CSSValueBackgroundPosition::type1_center;
	else if (!y_specified)
		bg_pos.type_y = CSSValueBackgroundPosition::type2_center;

	parse_pos = last_pos;
	layer_position = bg_pos;
	return true;
}

bool CSSParserBackground::parse_size(CSSValueBackgroundSize::Size &layer_size, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);
	if (token.type != CSSToken::type_delim || token.value != "/")
		return false;

	token = next_token(pos, tokens);

	CSSValueBackgroundSize::Size size;
	bool single_value = false;
	if (token.type == CSSToken::type_ident)
	{
		if (equals(token.value, "contain"))
		{
			size.type = CSSValueBackgroundSize::size_contain;
			single_value = true;
		}
		else if (equals(token.value, "cover"))
		{
			size.type = CSSValueBackgroundSize::size_cover;
			single_value = true;
		}
		else if (equals(token.value, "auto"))
		{
			size.type = CSSValueBackgroundSize::size_values;
			size.value_x = CSSValueBackgroundSize::value_type_auto;
		}
		else
		{
			return false;
		}
	}
	else if (is_length(token))
	{
		CSSLength length;
		if (parse_length(token, length))
		{
			size.value_x = CSSValueBackgroundSize::value_type_length;
			size.length_x = length;
		}
		else
		{
			return false;
		}
	}
	else if (token.type == CSSToken::type_percentage)
	{
		size.value_x = CSSValueBackgroundSize::value_type_percentage;
		size.percentage_x = StringHelp::text_to_float(token.value);
	}
	else
	{
		return false;
	}

	if (pos == tokens.size() || single_value)
	{
		layer_size = size;
		parse_pos = pos;
		return true;
	}

	token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && equals(token.value, "auto"))
	{
		size.value_y = CSSValueBackgroundSize::value_type_auto;
		parse_pos = pos;
	}
	else if (is_length(token))
	{
		CSSLength length;
		if (parse_length(token, length))
		{
			size.value_y = CSSValueBackgroundSize::value_type_length;
			size.length_y = length;
			parse_pos = pos;
		}
	}
	else if (token.type == CSSToken::type_percentage)
	{
		size.value_y = CSSValueBackgroundSize::value_type_percentage;
		size.percentage_y = StringHelp::text_to_float(token.value);
		parse_pos = pos;
	}

	layer_size = size;
	return true;
}

bool CSSParserBackground::parse_boxes(CSSValueBackgroundOrigin::OriginType &layer_origin, CSSValueBackgroundClip::ClipType &layer_clip, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);
	if (token.type != CSSToken::type_ident)
		return false;

	if (equals(token.value, "border-box"))
	{
		layer_origin = CSSValueBackgroundOrigin::origin_border_box;
		layer_clip = CSSValueBackgroundClip::clip_border_box;
	}
	else if (equals(token.value, "padding-box"))
	{
		layer_origin = CSSValueBackgroundOrigin::origin_padding_box;
		layer_clip = CSSValueBackgroundClip::clip_padding_box;
	}
	else if (equals(token.value, "content-box"))
	{
		layer_origin = CSSValueBackgroundOrigin::origin_content_box;
		layer_clip = CSSValueBackgroundClip::clip_content_box;
	}
	else
	{
		return false;
	}

	parse_pos = pos;

	token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident)
	{
		if (equals(token.value, "border-box"))
		{
			layer_clip = CSSValueBackgroundClip::clip_border_box;
			parse_pos = pos;
		}
		else if (equals(token.value, "padding-box"))
		{
			layer_clip = CSSValueBackgroundClip::clip_padding_box;
			parse_pos = pos;
		}
		else if (equals(token.value, "content-box"))
		{
			layer_clip = CSSValueBackgroundClip::clip_content_box;
			parse_pos = pos;
		}
	}

	return true;
}

}
