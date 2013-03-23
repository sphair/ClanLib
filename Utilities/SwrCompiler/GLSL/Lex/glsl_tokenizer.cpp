
#include "precomp.h"
#include "glsl_tokenizer.h"

using namespace clan;

GlslTokenizer::GlslTokenizer(std::string data)
: data(data), pos(0), saved_pos(0)
{
	keywords["attribute"] = GlslToken::keyword_attribute;
	keywords["const"] = GlslToken::keyword_const;
	keywords["uniform"] = GlslToken::keyword_uniform;
	keywords["varying"] = GlslToken::keyword_varying;
	keywords["coherent"] = GlslToken::keyword_coherent;
	keywords["volatile"] = GlslToken::keyword_volatile;
	keywords["restrict"] = GlslToken::keyword_restrict;
	keywords["readonly"] = GlslToken::keyword_readonly;
	keywords["writeonly"] = GlslToken::keyword_writeonly;
	keywords["atomic_uint"] = GlslToken::keyword_atomic_uint;
	keywords["layout"] = GlslToken::keyword_layout;
	keywords["centroid"] = GlslToken::keyword_centroid;
	keywords["flat"] = GlslToken::keyword_flat;
	keywords["smooth"] = GlslToken::keyword_smooth;
	keywords["noperspective"] = GlslToken::keyword_noperspective;
	keywords["patch"] = GlslToken::keyword_patch;
	keywords["sample"] = GlslToken::keyword_sample;
	keywords["break"] = GlslToken::keyword_break;
	keywords["continue"] = GlslToken::keyword_continue;
	keywords["do"] = GlslToken::keyword_do;
	keywords["for"] = GlslToken::keyword_for;
	keywords["while"] = GlslToken::keyword_while;
	keywords["switch"] = GlslToken::keyword_switch;
	keywords["case"] = GlslToken::keyword_case;
	keywords["default"] = GlslToken::keyword_default;
	keywords["if"] = GlslToken::keyword_if;
	keywords["else"] = GlslToken::keyword_else;
	keywords["subroutine"] = GlslToken::keyword_subroutine;
	keywords["in"] = GlslToken::keyword_in;
	keywords["out"] = GlslToken::keyword_out;
	keywords["inout"] = GlslToken::keyword_inout;
	keywords["float"] = GlslToken::keyword_float;
	keywords["double"] = GlslToken::keyword_double;
	keywords["int"] = GlslToken::keyword_int;
	keywords["void"] = GlslToken::keyword_void;
	keywords["bool"] = GlslToken::keyword_bool;
	keywords["true"] = GlslToken::keyword_true;
	keywords["false"] = GlslToken::keyword_false;
	keywords["invariant"] = GlslToken::keyword_invariant;
	keywords["discard"] = GlslToken::keyword_discard;
	keywords["return"] = GlslToken::keyword_return;
	keywords["mat2"] = GlslToken::keyword_mat2;
	keywords["mat3"] = GlslToken::keyword_mat3;
	keywords["mat4"] = GlslToken::keyword_mat4;
	keywords["dmat2"] = GlslToken::keyword_dmat2;
	keywords["dmat3"] = GlslToken::keyword_dmat3;
	keywords["dmat4"] = GlslToken::keyword_dmat4;
	keywords["mat2x2"] = GlslToken::keyword_mat2x2;
	keywords["mat2x3"] = GlslToken::keyword_mat2x3;
	keywords["mat2x4"] = GlslToken::keyword_mat2x4;
	keywords["dmat2x2"] = GlslToken::keyword_dmat2x2;
	keywords["dmat2x3"] = GlslToken::keyword_dmat2x3;
	keywords["dmat2x4"] = GlslToken::keyword_dmat2x4;
	keywords["mat3x2"] = GlslToken::keyword_mat3x2;
	keywords["mat3x3"] = GlslToken::keyword_mat3x3;
	keywords["mat3x4"] = GlslToken::keyword_mat3x4;
	keywords["dmat3x2"] = GlslToken::keyword_dmat3x2;
	keywords["dmat3x3"] = GlslToken::keyword_dmat3x3;
	keywords["dmat3x4"] = GlslToken::keyword_dmat3x4;
	keywords["mat4x2"] = GlslToken::keyword_mat4x2;
	keywords["mat4x3"] = GlslToken::keyword_mat4x3;
	keywords["mat4x4"] = GlslToken::keyword_mat4x4;
	keywords["dmat4x2"] = GlslToken::keyword_dmat4x2;
	keywords["dmat4x3"] = GlslToken::keyword_dmat4x3;
	keywords["dmat4x4"] = GlslToken::keyword_dmat4x4;
	keywords["vec2"] = GlslToken::keyword_vec2;
	keywords["vec3"] = GlslToken::keyword_vec3;
	keywords["vec4"] = GlslToken::keyword_vec4;
	keywords["ivec2"] = GlslToken::keyword_ivec2;
	keywords["ivec3"] = GlslToken::keyword_ivec3;
	keywords["ivec4"] = GlslToken::keyword_ivec4;
	keywords["bvec2"] = GlslToken::keyword_bvec2;
	keywords["bvec3"] = GlslToken::keyword_bvec3;
	keywords["bvec4"] = GlslToken::keyword_bvec4;
	keywords["dvec2"] = GlslToken::keyword_dvec2;
	keywords["dvec3"] = GlslToken::keyword_dvec3;
	keywords["dvec4"] = GlslToken::keyword_dvec4;
	keywords["uint"] = GlslToken::keyword_uint;
	keywords["uvec2"] = GlslToken::keyword_uvec2;
	keywords["uvec3"] = GlslToken::keyword_uvec3;
	keywords["uvec4"] = GlslToken::keyword_uvec4;
	keywords["lowp"] = GlslToken::keyword_lowp;
	keywords["mediump"] = GlslToken::keyword_mediump;
	keywords["highp"] = GlslToken::keyword_highp;
	keywords["precision"] = GlslToken::keyword_precision;
	keywords["sampler1D"] = GlslToken::keyword_sampler1D;
	keywords["sampler2D"] = GlslToken::keyword_sampler2D;
	keywords["sampler3D"] = GlslToken::keyword_sampler3D;
	keywords["samplerCube"] = GlslToken::keyword_samplerCube;
	keywords["sampler1DShadow"] = GlslToken::keyword_sampler1DShadow;
	keywords["sampler2DShadow"] = GlslToken::keyword_sampler2DShadow;
	keywords["samplerCubeShadow"] = GlslToken::keyword_samplerCubeShadow;
	keywords["sampler1DArray"] = GlslToken::keyword_sampler1DArray;
	keywords["sampler2DArray"] = GlslToken::keyword_sampler2DArray;
	keywords["sampler1DArrayShadow"] = GlslToken::keyword_sampler1DArrayShadow;
	keywords["sampler2DArrayShadow"] = GlslToken::keyword_sampler2DArrayShadow;
	keywords["isampler1D"] = GlslToken::keyword_isampler1D;
	keywords["isampler2D"] = GlslToken::keyword_isampler2D;
	keywords["isampler3D"] = GlslToken::keyword_isampler3D;
	keywords["isamplerCube"] = GlslToken::keyword_isamplerCube;
	keywords["isampler1DArray"] = GlslToken::keyword_isampler1DArray;
	keywords["isampler2DArray"] = GlslToken::keyword_isampler2DArray;
	keywords["usampler1D"] = GlslToken::keyword_usampler1D;
	keywords["usampler2D"] = GlslToken::keyword_usampler2D;
	keywords["usampler3D"] = GlslToken::keyword_usampler3D;
	keywords["usamplerCube"] = GlslToken::keyword_usamplerCube;
	keywords["usampler1DArray"] = GlslToken::keyword_usampler1DArray;
	keywords["usampler2DArray"] = GlslToken::keyword_usampler2DArray;
	keywords["sampler2DRect"] = GlslToken::keyword_sampler2DRect;
	keywords["sampler2DRectShadow"] = GlslToken::keyword_sampler2DRectShadow;
	keywords["isampler2DRect"] = GlslToken::keyword_isampler2DRect;
	keywords["usampler2DRect"] = GlslToken::keyword_usampler2DRect;
	keywords["samplerBuffer"] = GlslToken::keyword_samplerBuffer;
	keywords["isamplerBuffer"] = GlslToken::keyword_isamplerBuffer;
	keywords["usamplerBuffer"] = GlslToken::keyword_usamplerBuffer;
	keywords["sampler2DMS"] = GlslToken::keyword_sampler2DMS;
	keywords["isampler2DMS"] = GlslToken::keyword_isampler2DMS;
	keywords["usampler2DMS"] = GlslToken::keyword_usampler2DMS;
	keywords["sampler2DMSArray"] = GlslToken::keyword_sampler2DMSArray;
	keywords["isampler2DMSArray"] = GlslToken::keyword_isampler2DMSArray;
	keywords["usampler2DMSArray"] = GlslToken::keyword_usampler2DMSArray;
	keywords["samplerCubeArray"] = GlslToken::keyword_samplerCubeArray;
	keywords["samplerCubeArrayShadow"] = GlslToken::keyword_samplerCubeArrayShadow;
	keywords["isamplerCubeArray"] = GlslToken::keyword_isamplerCubeArray;
	keywords["usamplerCubeArray"] = GlslToken::keyword_usamplerCubeArray;
	keywords["image1D"] = GlslToken::keyword_image1D;
	keywords["iimage1D"] = GlslToken::keyword_iimage1D;
	keywords["uimage1D"] = GlslToken::keyword_uimage1D;
	keywords["image2D"] = GlslToken::keyword_image2D;
	keywords["iimage2D"] = GlslToken::keyword_iimage2D;
	keywords["uimage2D"] = GlslToken::keyword_uimage2D;
	keywords["image3D"] = GlslToken::keyword_image3D;
	keywords["iimage3D"] = GlslToken::keyword_iimage3D;
	keywords["uimage3D"] = GlslToken::keyword_uimage3D;
	keywords["image2DRect"] = GlslToken::keyword_image2DRect;
	keywords["iimage2DRect"] = GlslToken::keyword_iimage2DRect;
	keywords["uimage2DRect"] = GlslToken::keyword_uimage2DRect;
	keywords["imageCube"] = GlslToken::keyword_imageCube;
	keywords["iimageCube"] = GlslToken::keyword_iimageCube;
	keywords["uimageCube"] = GlslToken::keyword_uimageCube;
	keywords["imageBuffer"] = GlslToken::keyword_imageBuffer;
	keywords["iimageBuffer"] = GlslToken::keyword_iimageBuffer;
	keywords["uimageBuffer"] = GlslToken::keyword_uimageBuffer;
	keywords["image1DArray"] = GlslToken::keyword_image1DArray;
	keywords["iimage1DArray"] = GlslToken::keyword_iimage1DArray;
	keywords["uimage1DArray"] = GlslToken::keyword_uimage1DArray;
	keywords["image2DArray"] = GlslToken::keyword_image2DArray;
	keywords["iimage2DArray"] = GlslToken::keyword_iimage2DArray;
	keywords["uimage2DArray"] = GlslToken::keyword_uimage2DArray;
	keywords["imageCubeArray"] = GlslToken::keyword_imageCubeArray;
	keywords["iimageCubeArray"] = GlslToken::keyword_iimageCubeArray;
	keywords["uimageCubeArray"] = GlslToken::keyword_uimageCubeArray;
	keywords["image2DMS"] = GlslToken::keyword_image2DMS;
	keywords["iimage2DMS"] = GlslToken::keyword_iimage2DMS;
	keywords["uimage2DMS"] = GlslToken::keyword_uimage2DMS;
	keywords["image2DMSArray"] = GlslToken::keyword_image2DMSArray;
	keywords["iimage2DMSArray"] = GlslToken::keyword_iimage2DMSArray;
	keywords["uimage2DMSArray"] = GlslToken::keyword_uimage2DMSArray;
	keywords["struct"] = GlslToken::keyword_struct;
	keywords["common"] = GlslToken::keyword_common;
	keywords["partition"] = GlslToken::keyword_partition;
	keywords["active"] = GlslToken::keyword_active;
	keywords["asm"] = GlslToken::keyword_asm;
	keywords["class"] = GlslToken::keyword_class;
	keywords["union"] = GlslToken::keyword_union;
	keywords["enum"] = GlslToken::keyword_enum;
	keywords["typedef"] = GlslToken::keyword_typedef;
	keywords["template"] = GlslToken::keyword_template;
	keywords["this"] = GlslToken::keyword_this;
	keywords["packed"] = GlslToken::keyword_packed;
	keywords["resource"] = GlslToken::keyword_resource;
	keywords["goto"] = GlslToken::keyword_goto;
	keywords["inline"] = GlslToken::keyword_inline;
	keywords["noinline"] = GlslToken::keyword_noinline;
	keywords["public"] = GlslToken::keyword_public;
	keywords["static"] = GlslToken::keyword_static;
	keywords["extern"] = GlslToken::keyword_extern;
	keywords["external"] = GlslToken::keyword_external;
	keywords["interface"] = GlslToken::keyword_interface;
	keywords["long"] = GlslToken::keyword_long;
	keywords["short"] = GlslToken::keyword_short;
	keywords["half"] = GlslToken::keyword_half;
	keywords["fixed"] = GlslToken::keyword_fixed;
	keywords["unsigned"] = GlslToken::keyword_unsigned;
	keywords["superp"] = GlslToken::keyword_superp;
	keywords["input"] = GlslToken::keyword_input;
	keywords["output"] = GlslToken::keyword_output;
	keywords["hvec2"] = GlslToken::keyword_hvec2;
	keywords["hvec3"] = GlslToken::keyword_hvec3;
	keywords["hvec4"] = GlslToken::keyword_hvec4;
	keywords["fvec2"] = GlslToken::keyword_fvec2;
	keywords["fvec3"] = GlslToken::keyword_fvec3;
	keywords["fvec4"] = GlslToken::keyword_fvec4;
	keywords["sampler3DRect"] = GlslToken::keyword_sampler3DRect;
	keywords["filter"] = GlslToken::keyword_filter;
	keywords["sizeof"] = GlslToken::keyword_sizeof;
	keywords["cast"] = GlslToken::keyword_cast;
	keywords["namespace"] = GlslToken::keyword_namespace;
	keywords["using"] = GlslToken::keyword_using;
	keywords["row_major"] = GlslToken::keyword_row_major;
}

GlslToken GlslTokenizer::read_token()
{
	GlslToken token;
	read_token(token);
	return token;
}

void GlslTokenizer::read_token(GlslToken &out_token, bool skip_whitespace_and_comments)
{
	do
	{
		read_token(out_token);
	} while (out_token.type == GlslToken::type_whitespace || out_token.type == GlslToken::type_comment);
}

GlslToken GlslTokenizer::peek_token(bool skip_whitespace_and_comments)
{
	GlslToken token;
	save_position();
	read_token(token, skip_whitespace_and_comments);
	restore_position();
	return token;
}

void GlslTokenizer::read_token(GlslToken &out_token)
{
	// To do: add backslask (\) line-continuation when used as last character of line

	std::string::char_type next_char = 0;
	std::string::char_type third_char = 0;
	if (pos + 1 < data.length())
		next_char = data[pos + 1];
	if (pos + 2 < data.length())
		third_char = data[pos + 2];

	if (pos == data.length())
	{
		out_token.type = GlslToken::type_eof;
	}
	else if ((data[pos] >= '0' && data[pos] <= '9') || (data[pos] == '.' && next_char >= '0' && next_char < '9'))
	{
		bool is_float = false;
		bool is_double = false;
		bool is_unsigned = false;
		bool is_octal = false;
		bool is_hex = false;

		if (data[pos] == '0')
		{
			if (pos + 1 < data.length() && (data[pos + 1] == 'x' || data[pos + 1] == 'X'))
			{
				is_hex = true;
				pos += 2;
			}
			// To do: parse octals.. (while still correctly parsing 0.5f)
			/*else
			{
				is_octal = true;
				pos++;
			}*/
		}

		std::string number;
		number += read_digit_sequence(is_octal, is_hex);
		if (pos != data.length() && data[pos] == '.')
		{
			is_float = true;
			number.push_back('.');
			pos++;
			number += read_digit_sequence(is_octal, is_hex);
		}

		if (pos != data.length() && (data[pos] == 'e' && data[pos] == 'E'))
		{
			next_char = 0;
			third_char = 0;
			if (pos + 1 < data.length())
				next_char = data[pos + 1];
			if (pos + 2 < data.length())
				third_char = data[pos + 2];
			if ((next_char == '+' || next_char == '-') && (third_char >= '0' && third_char <= '9'))
			{
				number.push_back(data[pos]);
				number.push_back(data[pos + 1]);
				pos += 2;
				number += read_digit_sequence(is_octal, is_hex);
				is_float = true;
			}
			else
			{
				throw Exception("Invalid number encountered");
			}
		}

		if (pos != data.length() && ((data[pos] >= 'a' && data[pos] <= 'z') || (data[pos] >= 'A' && data[pos] <= 'Z')))
		{
			next_char = 0;
			if (pos + 1 < data.length())
				next_char = data[pos + 1];
			if (data[pos] == 'f' || data[pos] == 'F')
			{
				is_float = true;
			}
			else if ((data[pos] == 'l' && next_char == 'f') || (data[pos] == 'L' && next_char == 'F'))
			{
				is_float = false;
				is_double = true;
			}
			else if (data[pos] == 'u' && data[pos] == 'U')
			{
				if (is_float)
					throw Exception("Invalid number encountered");
				is_unsigned = true;
			}
			else
			{
				throw Exception("Invalid number encountered");
			}
			pos++;
		}

		if (is_hex && (is_float || is_double))
			throw Exception("Invalid number encountered");

		int base = 10;
		if (is_hex)
			base = 16;
		else if (is_octal)
			base = 8;

		if (is_float)
		{
			out_token.type = GlslToken::type_float_constant;
			out_token.float_constant = StringHelp::text_to_float(number);
		}
		else if (is_double)
		{
			out_token.type = GlslToken::type_double_constant;
			out_token.double_constant = StringHelp::text_to_double(number);
		}
		else if (is_unsigned)
		{
			out_token.type = GlslToken::type_uint_constant;
			out_token.int_constant = StringHelp::text_to_uint(number, base);
		}
		else
		{
			out_token.type = GlslToken::type_int_constant;
			out_token.int_constant = StringHelp::text_to_int(number, base);
		}
	}
	else if ((data[pos] >= 'a' && data[pos] <= 'z') || (data[pos] >= 'A' && data[pos] <= 'Z') || data[pos] == '_')
	{
		out_token.identifier.clear();
		while (pos != data.length() && ((data[pos] >= 'a' && data[pos] <= 'z') || (data[pos] >= 'A' && data[pos] <= 'Z') || (data[pos] >= '0' && data[pos] <= '9') || data[pos] == '_'))
		{
			out_token.identifier.push_back(data[pos]);
			pos++;
		}

		std::map<std::string, GlslToken::Keyword>::iterator it = keywords.find(out_token.identifier);
		if (it == keywords.end())
		{
			out_token.type = GlslToken::type_identifier;
		}
		else
		{
			out_token.type = GlslToken::type_keyword;
			out_token.keyword = it->second;
		}
	}
	else
	{
		switch (data[pos])
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
		case '\f':
		case '\v':
			while (pos != data.length() && (data[pos] == ' ' || data[pos] == '\t' || data[pos] == '\r' || data[pos] == '\n' || data[pos] == '\f' || data[pos] == '\v'))
				pos++;
			out_token.type = GlslToken::type_whitespace;
			break;

		case '.':
			out_token.type = GlslToken::type_operator;
			out_token.oper = GlslToken::operator_dot;
			pos++;
			break;

		case '+':
			out_token.type = GlslToken::type_operator;
			if (next_char == '+')
			{
				out_token.oper = GlslToken::operator_increment;
				pos += 2;
			}
			else if (next_char == '=')
			{
				out_token.oper = GlslToken::operator_add_assign;
				pos += 2;
			}
			else
			{
				out_token.oper = GlslToken::operator_plus;
				pos++;
			}
			break;

		case '-':
			out_token.type = GlslToken::type_operator;
			if (next_char == '-')
			{
				out_token.oper = GlslToken::operator_decrement;
				pos += 2;
			}
			else if (next_char == '=')
			{
				out_token.oper = GlslToken::operator_sub_assign;
				pos += 2;
			}
			else
			{
				out_token.oper = GlslToken::operator_minus;
				pos++;
			}
			break;

		case '/':
			if (next_char == '/')
			{
				while (pos != data.length())
				{
					if (data[pos] == '\n')
					{
						pos++;
						break;
					}
					pos++;
				}
				out_token.type = GlslToken::type_comment;
			}
			else if (next_char == '*')
			{
				while (pos != data.length())
				{
					if (data[pos] == '*' && pos + 1 < data.length() && data[pos + 1] == '/')
					{
						pos += 2;
						break;
					}
					pos++;
				}
				out_token.type = GlslToken::type_comment;
			}
			else if (next_char == '=')
			{
				out_token.type = GlslToken::type_operator;
				out_token.oper = GlslToken::operator_divide_assign;
				pos += 2;
			}
			else
			{
				out_token.type = GlslToken::type_operator;
				out_token.oper = GlslToken::operator_divide;
				pos++;
			}
			break;

		case '*':
			out_token.type = GlslToken::type_operator;
			if (next_char == '=')
			{
				out_token.oper = GlslToken::operator_multiply_assign;
				pos += 2;
			}
			else
			{
				out_token.oper = GlslToken::operator_multiply;
				pos++;
			}
			break;

		case '%':
			out_token.type = GlslToken::type_operator;
			if (next_char == '=')
			{
				out_token.oper = GlslToken::operator_modulus_assign;
				pos += 2;
			}
			else
			{
				out_token.oper = GlslToken::operator_modulus;
				pos++;
			}
			break;

		case '<':
			out_token.type = GlslToken::type_operator;
			if (next_char == '<' && third_char == '=')
			{
				out_token.oper = GlslToken::operator_shift_left_assign;
				pos += 3;
			}
			else if (next_char == '<')
			{
				out_token.oper = GlslToken::operator_shift_left;
				pos += 2;
			}
			else if (next_char == '=')
			{
				out_token.oper = GlslToken::operator_less_equal;
				pos += 2;
			}
			else
			{
				out_token.oper = GlslToken::operator_less;
				pos++;
			}
			break;

		case '>':
			out_token.type = GlslToken::type_operator;
			if (next_char == '>' && third_char == '=')
			{
				out_token.oper = GlslToken::operator_shift_right_assign;
				pos += 3;
			}
			else if (next_char == '>')
			{
				out_token.oper = GlslToken::operator_shift_right;
				pos += 2;
			}
			else if (next_char == '=')
			{
				out_token.oper = GlslToken::operator_greater_equal;
				pos += 2;
			}
			else
			{
				out_token.oper = GlslToken::operator_greater;
				pos++;
			}
			break;

		case '[':
			out_token.type = GlslToken::type_operator;
			out_token.oper = GlslToken::operator_bracket_begin;
			pos++;
			break;

		case ']':
			out_token.type = GlslToken::type_operator;
			out_token.oper = GlslToken::operator_bracket_end;
			pos++;
			break;

		case '(':
			out_token.type = GlslToken::type_operator;
			out_token.oper = GlslToken::operator_paranthesis_begin;
			pos++;
			break;

		case ')':
			out_token.type = GlslToken::type_operator;
			out_token.oper = GlslToken::operator_paranthesis_end;
			pos++;
			break;

		case '{':
			out_token.type = GlslToken::type_scope_begin;
			pos++;
			break;

		case '}':
			out_token.type = GlslToken::type_scope_end;
			pos++;
			break;

		case '^':
			out_token.type = GlslToken::type_operator;
			if (next_char == '^')
			{
				out_token.oper = GlslToken::operator_logical_xor;
				pos += 2;
			}
			else if (next_char == '=')
			{
				out_token.oper = GlslToken::operator_bit_xor_assign;
				pos += 2;
			}
			else
			{
				out_token.oper = GlslToken::operator_bit_xor;
				pos++;
			}
			break;

		case '|':
			out_token.type = GlslToken::type_operator;
			if (next_char == '|')
			{
				out_token.oper = GlslToken::operator_logical_or;
				pos += 2;
			}
			else if (next_char == '=')
			{
				out_token.oper = GlslToken::operator_bit_or_assign;
				pos += 2;
			}
			else
			{
				out_token.oper = GlslToken::operator_bit_or;
				pos++;
			}
			break;

		case '&':
			out_token.type = GlslToken::type_operator;
			if (next_char == '&')
			{
				out_token.oper = GlslToken::operator_logical_and;
				pos += 2;
			}
			else if (next_char == '=')
			{
				out_token.oper = GlslToken::operator_bit_and_assign;
				pos += 2;
			}
			else
			{
				out_token.oper = GlslToken::operator_bit_and;
				pos++;
			}
			break;

		case '~':
			out_token.type = GlslToken::type_operator;
			out_token.oper = GlslToken::operator_bit_not;
			pos++;
			break;

		case '=':
			if (next_char == '=')
			{
				out_token.type = GlslToken::type_operator;
				out_token.oper = GlslToken::operator_equal;
				pos += 2;
			}
			else
			{
				out_token.type = GlslToken::type_operator;
				out_token.oper = GlslToken::operator_assign;
				pos++;
			}
			break;

		case '!':
			out_token.type = GlslToken::type_operator;
			if (next_char == '=')
			{
				out_token.oper = GlslToken::operator_not_equal;
				pos += 2;
			}
			else
			{
				out_token.oper = GlslToken::operator_logical_not;
				pos++;
			}
			break;

		case ':':
			out_token.type = GlslToken::type_operator;
			out_token.oper = GlslToken::operator_colon;
			pos++;
			break;

		case ';':
			out_token.type = GlslToken::type_semicolon;
			pos++;
			break;

		case ',':
			out_token.type = GlslToken::type_operator;
			out_token.oper = GlslToken::operator_comma;
			pos++;
			break;

		case '?':
			out_token.type = GlslToken::type_operator;
			out_token.oper = GlslToken::operator_questionmark;
			pos++;
			break;

		default:
			throw Exception("Invalid character encountered");
			break;
		}
	}
}

std::string GlslTokenizer::read_digit_sequence(bool is_octal, bool is_hex)
{
	if (is_octal)
	{
		std::string digits;
		while (pos != data.length() && data[pos] >= '0' && data[pos] <= '7')
		{
			digits.push_back(data[pos]);
			pos++;
		}
		return digits;
	}
	else if (is_hex)
	{
		std::string digits;
		while (pos != data.length() && ((data[pos] >= '0' && data[pos] <= '9') || (data[pos] >= 'a' && data[pos] <= 'f') || (data[pos] >= 'A' && data[pos] <= 'F')))
		{
			digits.push_back(data[pos]);
			pos++;
		}
		return digits;
	}
	else
	{
		std::string digits;
		while (pos != data.length() && data[pos] >= '0' && data[pos] <= '9')
		{
			digits.push_back(data[pos]);
			pos++;
		}
		return digits;
	}
}
