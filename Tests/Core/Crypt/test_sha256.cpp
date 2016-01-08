/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

void TestApp::test_sha256()
{
	Console::write_line(" Header: sha256.h");
	Console::write_line("  Class: SHA256");

	// Using http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA256.pdf for test data

	SHA256 sha256;
	sha256.add("abc", 3);
	sha256.calculate();
	test_hash(sha256, "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD");

	sha256.add("a", 1);
	sha256.add("b", 1);
	sha256.add("c", 1);
	sha256.calculate();
	test_hash(sha256, "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD");

	const char *test_str1 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	sha256.add(test_str1, strlen(test_str1));
	sha256.calculate();
	test_hash(sha256, "248D6A61D20638B8E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1");


	// Using wikipedia http://en.wikipedia.org/wiki/Hmac test data
	char *test_str9a = "";
	char *test_str9b = "";
	sha256.set_hmac(test_str9a, strlen(test_str9a));
	sha256.add(test_str9b, strlen(test_str9b));
	sha256.calculate();
	test_hash(sha256, "B613679A0814D9EC772F95D778C35FC5FF1697C493715653C6C712144292C5AD");

	char *test_str10a = "key";
	char *test_str10b = "The quick brown fox jumps over the lazy dog";
	sha256.set_hmac(test_str10a, strlen(test_str10a));
	sha256.add(test_str10b, strlen(test_str10b));
	sha256.calculate();
	test_hash(sha256, "F7BC83F430538424B13298E6AA6FB143EF4D59A14946175997479DBC2D1A3CD8");
}


void TestApp::test_hash(const SHA256 &sha256, const char *hash_text)
{
	std::string hash = sha256.get_hash(true);
	if (hash != hash_text)
		fail();

	if (strlen(hash_text) != 64)
		fail();

	unsigned char out_hash[32];
	sha256.get_hash(out_hash);
	for (int cnt=0; cnt<32; cnt++)
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


