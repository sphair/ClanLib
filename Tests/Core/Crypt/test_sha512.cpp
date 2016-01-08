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

void TestApp::test_sha512()
{
	Console::write_line(" Header: sha512.h");
	Console::write_line("  Class: SHA512");

	// Using http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA512.pdf for test data

	SHA512 sha512;
	sha512.add("abc", 3);
	sha512.calculate();
	test_hash(sha512, "DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A2192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F");

	sha512.add("a", 1);
	sha512.add("b", 1);
	sha512.add("c", 1);
	sha512.calculate();
	test_hash(sha512, "DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A2192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F");

	const char *test_str1 = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
	sha512.add(test_str1, strlen(test_str1));
	sha512.calculate();
	test_hash(sha512, "8E959B75DAE313DA8CF4F72814FC143F8F7779C6EB9F7FA17299AEADB6889018501D289E4900F7E4331B99DEC4B5433AC7D329EEB6DD26545E96E55B874BE909");


	// Using wikipedia http://en.wikipedia.org/wiki/Hmac test data
	char *test_str9a = "";
	char *test_str9b = "";
	sha512.set_hmac(test_str9a, strlen(test_str9a));
	sha512.add(test_str9b, strlen(test_str9b));
	sha512.calculate();
	test_hash(sha512, "B936CEE86C9F87AA5D3C6F2E84CB5A4239A5FE50480A6EC66B70AB5B1F4AC6730C6C515421B327EC1D69402E53DFB49AD7381EB067B338FD7B0CB22247225D47");

	char *test_str10a = "key";
	char *test_str10b = "The quick brown fox jumps over the lazy dog";
	sha512.set_hmac(test_str10a, strlen(test_str10a));
	sha512.add(test_str10b, strlen(test_str10b));
	sha512.calculate();
	test_hash(sha512, "B42AF09057BAC1E2D41708E48A902E09B5FF7F12AB428A4FE86653C73DD248FB82F948A549F7B791A5B41915EE4D1EC3935357E4E2317250D0372AFA2EBEEB3A");
}


void TestApp::test_hash(const SHA512 &sha512, const char *hash_text)
{
	std::string hash = sha512.get_hash(true);
	if (hash != hash_text)
		fail();

	if (strlen(hash_text) != 128)
		fail();

	unsigned char out_hash[64];
	sha512.get_hash(out_hash);
	for (int cnt=0; cnt<64; cnt++)
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


