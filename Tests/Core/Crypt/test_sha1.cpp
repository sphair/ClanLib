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

void TestApp::test_sha1()
{
	Console::write_line(" Header: sha1.h");
	Console::write_line("  Class: SHA1");

	// Using http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA1.pdf for test data

	SHA1 sha1;
	sha1.add("abc", 3);
	sha1.calculate();
	test_hash(sha1, "A9993E364706816ABA3E25717850C26C9CD0D89D");

	sha1.add("a", 1);
	sha1.add("b", 1);
	sha1.add("c", 1);
	sha1.calculate();
	test_hash(sha1, "A9993E364706816ABA3E25717850C26C9CD0D89D");

	const char *test_str1 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	sha1.add(test_str1, strlen(test_str1));
	sha1.calculate();
	test_hash(sha1, "84983E441C3BD26EBAAE4AA1F95129E5E54670F1");

	// Using wikipedia http://en.wikipedia.org/wiki/Hmac test data
	char *test_str9a = "";
	char *test_str9b = "";
	sha1.set_hmac(test_str9a, strlen(test_str9a));
	sha1.add(test_str9b, strlen(test_str9b));
	sha1.calculate();
	test_hash(sha1, "FBDB1D1B18AA6C08324B7D64B71FB76370690E1D");

	char *test_str10a = "key";
	char *test_str10b = "The quick brown fox jumps over the lazy dog";
	sha1.set_hmac(test_str10a, strlen(test_str10a));
	sha1.add(test_str10b, strlen(test_str10b));
	sha1.calculate();
	test_hash(sha1, "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9");

}


void TestApp::test_hash(const SHA1 &sha1, const char *hash_text)
{
	std::string hash = sha1.get_hash(true);
	if (hash != hash_text)
		fail();

	if (strlen(hash_text) != 40)
		fail();

	unsigned char out_hash[20];
	sha1.get_hash(out_hash);
	for (int cnt=0; cnt<20; cnt++)
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


