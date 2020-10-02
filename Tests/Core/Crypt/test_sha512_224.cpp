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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

void TestApp::test_sha512_224()
{
	Console::write_line(" Header: sha512_224.h");
	Console::write_line("  Class: SHA512_224");

	// Using http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA512_224.pdf for test data

	SHA512_224 sha512_224;
	sha512_224.add("abc", 3);
	sha512_224.calculate();
	test_hash(sha512_224, "4634270F707B6A54DAAE7530460842E20E37ED265CEEE9A43E8924AA");

	sha512_224.add("a", 1);
	sha512_224.add("b", 1);
	sha512_224.add("c", 1);
	sha512_224.calculate();
	test_hash(sha512_224, "4634270F707B6A54DAAE7530460842E20E37ED265CEEE9A43E8924AA");

	const char *test_str1 = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
	sha512_224.add(test_str1, strlen(test_str1));
	sha512_224.calculate();
	test_hash(sha512_224, "23FEC5BB94D60B23308192640B0C453335D664734FE40E7268674AF9");
}

void TestApp::test_hash(const SHA512_224 &sha512_224, const char *hash_text)
{
	std::string hash = sha512_224.get_hash(true);
	if (hash != hash_text)
		fail();

	if (strlen(hash_text) != 56)
		fail();

	unsigned char out_hash[28];
	sha512_224.get_hash(out_hash);
	for (int cnt=0; cnt<28; cnt++)
	{
		unsigned int value = out_hash[cnt];
		char nibble_high = *(hash_text++);
		char nibble_low = *(hash_text++);

		nibble_high >= 'A' ? (nibble_high = nibble_high - 'A' + 10) : nibble_high = nibble_high - '0';
		nibble_low >= 'A' ? (nibble_low = nibble_low - 'A' + 10) : nibble_low = nibble_low - '0';

		if (value != ((nibble_high << 4) | (nibble_low) ))
			fail();
	}

}


