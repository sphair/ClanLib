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

void TestApp::test_md5()
{
	Console::write_line(" Header: md5.h");
	Console::write_line("  Class: MD5");

	// Using wikipedia http://en.wikipedia.org/wiki/Md5 test data

	MD5 md5;
	char *test_str1 = "The quick brown fox jumps over the lazy dog";
	md5.add(test_str1, strlen(test_str1));
	md5.calculate();
	test_hash(md5, "9E107D9D372BB6826BD81D3542A419D6");

	const char *test_str2 = "The quick brown fox jumps over the lazy dog.";
	md5.add(test_str2, strlen(test_str2));
	md5.calculate();
	test_hash(md5, "E4D909C290D0FB1CA068FFADDF22CBD0");

	md5.calculate();
	test_hash(md5, "D41D8CD98F00B204E9800998ECF8427E");

	// Using http://tools.ietf.org/html/rfc1321 test data
	const char *test_str3 = "a";
	md5.add(test_str3, strlen(test_str3));
	md5.calculate();
	test_hash(md5, "0CC175B9C0F1B6A831C399E269772661");

	const char *test_str4 = "abc";
	md5.add(test_str4, strlen(test_str4));
	md5.calculate();
	test_hash(md5, "900150983CD24FB0D6963F7D28E17F72");

	const char *test_str5 = "message digest";
	md5.add(test_str5, strlen(test_str5));
	md5.calculate();
	test_hash(md5, "F96B697D7CB7938D525A2F31AAF161D0");

	const char *test_str6 = "abcdefghijklmnopqrstuvwxyz";
	md5.add(test_str6, strlen(test_str6));
	md5.calculate();
	test_hash(md5, "C3FCD3D76192E4007DFB496CCA67E13B");

	const char *test_str7 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	md5.add(test_str7, strlen(test_str7));
	md5.calculate();
	test_hash(md5, "D174AB98D277D9F5A5611C2C9F419D9F");

	const char *test_str8 = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
	md5.add(test_str8, strlen(test_str8));
	md5.calculate();
	test_hash(md5, "57EDF4A22BE3C955AC49DA2E2107B67A");

	// Using wikipedia http://en.wikipedia.org/wiki/Hmac test data
	char *test_str9a = "";
	char *test_str9b = "";
	md5.set_hmac(test_str9a, strlen(test_str9a));
	md5.add(test_str9b, strlen(test_str9b));
	md5.calculate();
	test_hash(md5, "74E6F7298A9C2D168935F58C001BAD88");

	char *test_str10a = "key";
	char *test_str10b = "The quick brown fox jumps over the lazy dog";
	md5.set_hmac(test_str10a, strlen(test_str10a));
	md5.add(test_str10b, strlen(test_str10b));
	md5.calculate();
	test_hash(md5, "80070713463E7749B90C2DC24911E275");

}


void TestApp::test_hash(const MD5 &md5, const char *hash_text)
{
	std::string hash = md5.get_hash(true);
	if (hash != hash_text)
		fail();

	if (strlen(hash_text) != 32)
		fail();

	unsigned char out_hash[16];
	md5.get_hash(out_hash);
	for (int cnt=0; cnt<16; cnt++)
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


