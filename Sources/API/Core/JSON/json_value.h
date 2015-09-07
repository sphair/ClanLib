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

#pragma once

#include <map>
#include <vector>

namespace clan
{
	/// \addtogroup clanCore_JSON clanCore JSON
	/// \{

	/// \brief Exception class thrown for JSON exceptions.
	class JsonException : public Exception
	{
	public:
		JsonException(const std::string &message) : Exception(message) { }
	};

	/// \brief Class representing a JSON value
	class JsonValue
	{
	public:
		/// \brief value type
		enum class Type
		{
			undefined,
			null,
			object,
			array,
			string,
			number,
			boolean
		};

		/// \brief Create a object value
		static JsonValue object() { return JsonValue(Type::object); }

		/// \brief Create a array value
		static JsonValue array() { return JsonValue(Type::array); }

		/// \brief Create a null value
		static JsonValue null() { return JsonValue(Type::null); }

		/// \brief Create a string value
		static JsonValue string(const std::string &value) { return JsonValue(value); }

		/// \brief Create a boolean value
		static JsonValue boolean(bool value) { return JsonValue(value); }

		/// \brief Create a number value
		static JsonValue number(int value) { return JsonValue(value); }
		static JsonValue number(double value) { return JsonValue(value); }

		/// \brief Create a value from UTF-8 JSON string
		static JsonValue from_json(const std::string &json);

		/// \brief Constructs a value
		JsonValue() : type(Type::undefined), value_number(), value_boolean() { }
		JsonValue(Type type) : type(type), value_number(), value_boolean() { }
		JsonValue(bool value) : type(Type::boolean), value_number(), value_boolean(value) { }
		JsonValue(int value) : type(Type::number), value_number((double)value), value_boolean() { }
		JsonValue(double value) : type(Type::number), value_number(value), value_boolean() { }
		JsonValue(const char *value) : type(Type::string), value_string(value), value_number(), value_boolean() { }
		JsonValue(const std::string &value) : type(Type::string), value_string(value), value_number(), value_boolean() { }

		/// \brief Convert value to a different type
		explicit operator bool() const { return to_boolean(); }
		operator std::string() const { return to_string(); }
		operator float() const { return to_float(); }
		operator double() const { return to_double(); }
		operator int() const { return to_int(); }

		/// \brief Indexers for object members or array items
		JsonValue &operator[](const char *key) { return members[key]; }
		JsonValue &operator[](const std::string &key) { return members[key]; }
		const JsonValue &operator[](const char *key) const
		{
			static JsonValue undefined;
			auto it = members.find(key);
			if (it == members.end()) return undefined;
			else return it->second;
		}
		const JsonValue &operator[](const std::string &key) const
		{
			static JsonValue undefined;
			auto it = members.find(key);
			if (it == members.end()) return undefined;
			else return it->second;
		}

		const JsonValue &operator[](int index) const { return items[index]; }
		JsonValue &operator[](int index) { return items[index]; }

		/// \brief Get value type
		Type get_type() const { return type; }

		/// \brief Get size of value
		size_t get_size() const
		{
			switch (type)
			{
			case Type::object: return members.size();
			case Type::array: return items.size();
			case Type::string: return value_string.size();
			default: return 0;
			}
		}

		/// \brief Get object members
		std::map<std::string, JsonValue> &get_members() { return members; }
		const std::map<std::string, JsonValue> &get_members() const { return members; }

		/// \brief Get array items
		std::vector<JsonValue> &get_items() { return items; }
		const std::vector<JsonValue> &get_items() const { return items; }

		/// \brief Return true if value is undefined
		bool is_undefined() const { return type == Type::undefined; }

		/// \brief Return true if value is null
		bool is_null() const { return type == Type::null; }

		/// \brief Return true if value is an object
		bool is_object() const { return type == Type::object; }

		/// \brief Return true if value is an array
		bool is_array() const { return type == Type::array; }

		/// \brief Return true if value is a string
		bool is_string() const { return type == Type::string; }

		/// \brief Return true if value is a number
		bool is_number() const { return type == Type::number; }

		/// \brief Return true if value is a boolean
		bool is_boolean() const { return type == Type::boolean; }

		/// \brief Convert value object to a string
		std::string to_string() const { if (type != Type::string) throw JsonException("JSON Value is not a string"); return value_string; }

		/// \brief Convert value object to an int
		int to_int() const { if (type != Type::number) throw JsonException("JSON Value is not a number"); return (int)value_number; }

		/// \brief Convert value object to a float
		float to_float() const { if (type != Type::number) throw JsonException("JSON Value is not a number"); return (float)value_number; }

		/// \brief Convert value object to a double
		double to_double() const { if (type != Type::number) throw JsonException("JSON Value is not a number"); return value_number; }

		/// \brief Convert value object to a boolean
		bool to_boolean() const { if (type != Type::boolean) throw JsonException("JSON Value is not a boolean"); return value_boolean; }

		/// \brief Assign a new value
		template<typename T>
		JsonValue &operator =(const T &value) { *this = JsonValue(value); return *this; }

		JsonValue &operator =(const JsonValue &value)
		{
			type = value.type;
			members = value.members;
			items = value.items;
			value_string = value.value_string;
			value_number = value.value_number;
			value_boolean = value.value_boolean;
			return *this;
		}

		/// \brief Convert value object to a std::map with the template specified value type
		template<typename Type>
		std::map<std::string, Type> to_map() const
		{
			if (type != Type::object)
				throw JsonException("JSON Value is not an object");

			std::map<std::string, Type> object;
			std::map<std::string, JsonValue>::const_iterator it;
			for (it = members.begin(); it != members.end(); ++it)
				object[it->first] = it->second;
			return object;
		}

		/// \brief Convert value array to a std::vector with the template specified value type
		template<typename Type>
		std::vector<Type> to_vector() const
		{
			if (type != Type::array)
				throw JsonException("JSON Value is not an array");

			std::vector<Type> list;
			list.reserve(items.size());
			for (auto & elem : items)
				list.push_back(elem);
			return list;
		}

		/// \brief Create an UTF-8 JSON string for the value
		std::string to_json() const;
		void to_json(std::string &result) const;

	private:
		void write(std::string &json) const;
		void write_array(std::string &json) const;
		void write_object(std::string &json) const;
		static void write_string(const std::string &str, std::string &json);
		void write_number(std::string &json) const;

		static JsonValue read(const std::string &json, size_t &pos);
		static JsonValue read_object(const std::string &json, size_t &pos);
		static JsonValue read_array(const std::string &json, size_t &pos);
		static std::string read_string(const std::string &json, size_t &pos);
		static JsonValue read_number(const std::string &json, size_t &pos);
		static JsonValue read_boolean(const std::string &json, size_t &pos);
		static void read_whitespace(const std::string &json, size_t &pos);

		Type type;
		std::map<std::string, JsonValue> members;
		std::vector<JsonValue> items;
		std::string value_string;
		double value_number;
		bool value_boolean;
	};

	/// \}
}
