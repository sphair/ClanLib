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

void TestApp::test_sha384()
{
	Console::write_line(" Header: sha384.h");
	Console::write_line("  Class: SHA384");

	// Using http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA384.pdf for test data

	SHA384 sha384;
	sha384.add("abc", 3);
	sha384.calculate();
	test_hash(sha384, "CB00753F45A35E8BB5A03D699AC65007272C32AB0EDED1631A8B605A43FF5BED8086072BA1E7CC2358BAECA134C825A7");

	sha384.add("a", 1);
	sha384.add("b", 1);
	sha384.add("c", 1);
	sha384.calculate();
	test_hash(sha384, "CB00753F45A35E8BB5A03D699AC65007272C32AB0EDED1631A8B605A43FF5BED8086072BA1E7CC2358BAECA134C825A7");

	const char *test_str1 = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
	sha384.add(test_str1, strlen(test_str1));
	sha384.calculate();
	test_hash(sha384, "09330C33F71147E83D192FC782CD1B4753111B173B3B05D22FA08086E3B0F712FCC7C71A557E2DB966C3E9FA91746039");
}


void TestApp::test_hash(const SHA384 &sha384, const char *hash_text)
{
	std::string hash = sha384.get_hash(true);
	if (hash != hash_text)
		fail();

	if (strlen(hash_text) != 96)
		fail();

	unsigned char out_hash[48];
	sha384.get_hash(out_hash);
	for (int cnt=0; cnt<48; cnt++)
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


