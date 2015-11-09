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

#include <memory>

namespace clan
{
	/// \addtogroup clanCore_Math clanCore Math
	/// \{

	class DataBuffer;
	class Base64Decoder_Impl;

	/// \brief Base64 decoder class.
	class Base64Decoder
	{
	public:
		/// \brief Constructs a base64 decoder.
		Base64Decoder();

		/// \brief Returns the decoded data.
		DataBuffer &get_result();

		/// \brief Resets the decoder.
		void reset();

		/// \brief Feeds the decoder with base64 encoded data.
		void feed(const void *data, int size, bool append_result = true);

		/// \brief Decode base64 data and return it in a buffer.
		static DataBuffer decode(const void *data, int size);

		/// \brief Decode
		///
		/// \param data = String Ref8
		///
		/// \return Data Buffer
		static DataBuffer decode(const std::string &data);

		/// \brief Decode
		///
		/// \param data = Data Buffer
		///
		/// \return Data Buffer
		static DataBuffer decode(const DataBuffer &data);

	private:
		std::shared_ptr<Base64Decoder_Impl> impl;
	};

	/// \}
}
