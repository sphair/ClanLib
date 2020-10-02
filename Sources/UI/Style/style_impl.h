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

#pragma once

#include "API/UI/Style/style_property_parser.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace clan
{
	class Style;
	class ImageSource;
	class Colorf;

	class StyleString
	{
	public:
		StyleString()
		{
			_buffer[0] = 0;
			_ptr = _buffer;
		}

		StyleString(const char *str)
		{
			_ptr = str;
			_size = strlen(str);
		}

		StyleString(const std::string &str)
		{
			_buffer[0] = 0;
			_ptr = _buffer;
			append(str.c_str());
		}

		StyleString(const StyleString &that)
		{
			_buffer[0] = 0;
			_ptr = _buffer;
			append(that.c_str());
		}

		std::size_t size() const { return _size; }
		const char *data() const { return _ptr; }
		const char *c_str() const { return _ptr; }

		StyleString &append(const char *str)
		{
			std::size_t len = strlen(str);
			if (_size + len > max_size || _ptr != _buffer)
				throw Exception("Style property name too long!");

			strcpy(_buffer + _size, str);
			_size += len;

			return *this;
		}

		StyleString &append(const std::string &str)
		{
			return append(str.c_str());
		}

		operator const char *() const { return c_str(); }

		bool operator==(const StyleString &that) const { return _size == that._size && strcmp(_ptr, that._ptr) == 0; }
		bool operator!=(const StyleString &that) const { return _size != that._size || strcmp(_ptr, that._ptr) != 0; }
		bool operator<(const StyleString &that) const { return strcmp(_ptr, that._ptr) < 0; }
		bool operator<=(const StyleString &that) const { return strcmp(_ptr, that._ptr) <= 0; }
		bool operator>(const StyleString &that) const { return strcmp(_ptr, that._ptr) > 0; }
		bool operator>=(const StyleString &that) const { return strcmp(_ptr, that._ptr) >= 0; }

		StyleString &operator=(const StyleString &that)
		{
			if (this != &that)
			{
				_ptr = _buffer;
				_size = 0;
				append(that.c_str());
			}
			return *this;
		}

		class hash
		{
		public:
			typedef StyleString argument_type;
			typedef std::size_t result_type;

			std::size_t operator()(const StyleString &s) const
			{
				if (s._hash != 0)
					return s._hash;

				std::size_t hash = 2166136261U;
				for (std::size_t i = 0; i < s._size; i++)
				{
					hash = (hash * 16777619U) ^ static_cast<std::size_t>(s._ptr[i]);
				}
				s._hash = hash;
				return hash;
			}
		};

	private:
		enum { max_size = 63 };
		const char *_ptr = nullptr;
		char _buffer[max_size + 1];
		std::size_t _size = 0;
		mutable std::size_t _hash = 0;
	};

	class StyleImpl : public StylePropertySetter
	{
	public:
		void set_value(const std::string &name, const StyleSetValue &value) override;
		void set_value_array(const std::string &name, const std::vector<StyleSetValue> &value_array) override;

		std::unordered_map<StyleString, StyleValueType, StyleString::hash> prop_type;
		std::unordered_map<StyleString, std::string, StyleString::hash> prop_text;
		std::unordered_map<StyleString, float, StyleString::hash> prop_number;
		std::unordered_map<StyleString, StyleDimension, StyleString::hash> prop_dimension;
		std::unordered_map<StyleString, Colorf, StyleString::hash> prop_color;
	};
}
