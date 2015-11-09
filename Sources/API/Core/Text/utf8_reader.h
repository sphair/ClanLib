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
**   Mark Page
*/

#pragma once

#include <memory>

namespace clan
{
	/// \addtogroup clanCore_Text clanCore Text
	/// \{

	class UTF8_Reader_Impl;

	/// \brief UTF8 reader helper functions.
	class UTF8_Reader
	{
	public:
		/// Important: text is not copied by this class and must remain valid during its usage.
		UTF8_Reader(const std::string::value_type *text, std::string::size_type length);

		/// \brief Returns true if the current position is at the end of the string
		bool is_end();

		/// \brief Get the character at the current position
		unsigned int get_char();

		/// \brief Returns the length of the current character
		std::string::size_type get_char_length();

		/// \brief Moves position to the previous character
		void prev();

		/// \brief Moves position to the next character
		void next();

		/// \brief Moves position to the lead byte of the character
		void move_to_leadbyte();

		/// \brief Get the current position of the reader
		std::string::size_type get_position();

		/// \brief Set the current position of the reader
		void set_position(std::string::size_type position);

	private:
		std::string::size_type current_position = 0;
		std::string::size_type length = 0;
		const unsigned char *data = nullptr;
	};

	/// \}
}
