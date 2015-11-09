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
**    Mark Page
*/

#pragma once

#include <memory>

namespace clan
{
	/// \addtogroup clanCore_Crypto clanCore Crypto
	/// \{

	class DataBuffer;
	class SHA256_Impl;

	/// \brief SHA-224 hash function class.
	class SHA224
	{
	public:
		/// \brief Constructs a SHA-224 hash generator.
		SHA224();

		static const int hash_size = 28;

		/// \brief Returns the calculated hash.
		std::string get_hash(bool uppercase = false) const;

		/// \brief Get hash
		///
		/// \param out_hash = where to write to
		void get_hash(unsigned char out_hash[hash_size]) const;

		/// \brief Resets the hash generator.
		void reset();

		/// \brief Enable a HMAC based calculation
		///
		/// Call this function before the initial add(). It is reset by reset()
		///
		/// \param key_data = The HMAC key
		/// \param key_size = The size of the key_data
		void set_hmac(const void *key_data, int key_size);

		/// \brief Adds data to be hashed.
		void add(const void *data, int size);

		/// \brief Add
		///
		/// \param data = Data Buffer
		void add(const DataBuffer &data);

		/// \brief Finalize hash calculation.
		void calculate();

	private:
		std::shared_ptr<SHA256_Impl> impl;	// Uses SHA256 implementation
	};

	/// \}
}
