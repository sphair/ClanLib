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

void TestApp::test_aes128()
{
	Console::write_line(" Header: aes128_encrypt.h and aes128_decrypt.h");
	Console::write_line("  Class: AES128_Encrypt and AES128_Decrypt");

	// Test data from http://csrc.nist.gov/publications/nistpubs/800-38a/sp800-38a.pdf
	// and http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/AES_CBC.pdf

	test_aes128_helper(
		"2b7e151628aed2a6abf7158809cf4f3c",	// KEY
		"000102030405060708090A0B0C0D0E0F",	// IV
		"6bc1bee22e409f96e93d7e117393172a"	// PLAINTEXT
		"ae2d8a571e03ac9c9eb76fac45af8e51"
		"30c81c46a35ce411e5fbc1191a0a52ef"
		"f69f2445df4f9b17ad2b417be66c3710",
		"7649abac8119b246cee98e9b12e9197d"  // CIPHERTEXT
		"5086cb9b507219ee95db113a917678b2"
		"73bed6b8e3c1743b7116e69e22229516"
		"3ff1caa1681fac09120eca307586e1a7"
		);
		
	const int test_data_length = 128;
	unsigned char test_data[test_data_length];
	std::vector<unsigned char> key;
	std::vector<unsigned char> iv;
	convert_ascii("2B7E151628AED2A6ABF7158809CF4F3C", key);
	convert_ascii("000102030405060708090A0B0C0D0E0F", iv);

	for (int cnt=0; cnt<test_data_length; cnt++)
	{
		test_data[cnt] = (unsigned char) cnt;

		AES128_Encrypt aes128_encrypt;
		aes128_encrypt.set_iv(&iv[0]);
		aes128_encrypt.set_key(&key[0]);
		aes128_encrypt.add(test_data, cnt+1);
		aes128_encrypt.calculate();

		AES128_Decrypt aes128_decrypt;
		aes128_decrypt.set_iv(&iv[0]);
		aes128_decrypt.set_key(&key[0]);
		DataBuffer buffer = aes128_encrypt.get_data();
		aes128_decrypt.add(buffer.get_data(), buffer.get_size());
		bool result = aes128_decrypt.calculate();
		if (!result)
			fail();
		DataBuffer buffer2 = aes128_decrypt.get_data();
		if (buffer2.get_size() != cnt+1)
			fail();
		unsigned char *data_ptr2 = (unsigned char *) buffer2.get_data();
		if (memcmp(data_ptr2, test_data, cnt+1))
			fail();
	}

}

void TestApp::convert_ascii(const char *src, std::vector<unsigned char> &dest)
{
	int len = strlen(src) / 2;
	dest.resize(len);

	for (int cnt=0; cnt < len; cnt++)
	{
		unsigned int let1 = toupper(*(src++));
		unsigned int let2 = toupper(*(src++));
		let1 >= 'A' ? let1 = let1 - 'A' + 10 : let1 = let1 - '0';
		let2 >= 'A' ? let2 = let2 - 'A' + 10 : let2 = let2 - '0';
		dest[cnt] = let1 << 4 | let2;
	}

}

void TestApp::test_aes128_helper(const char *key_ptr, const char *iv_ptr, const char *plaintext_ptr, const char *ciphertext_ptr)
{
	std::vector<unsigned char> key;
	std::vector<unsigned char> iv;
	std::vector<unsigned char> plaintext;
	std::vector<unsigned char> ciphertext;

	convert_ascii(key_ptr, key);
	convert_ascii(iv_ptr, iv);
	convert_ascii(plaintext_ptr, plaintext);
	convert_ascii(ciphertext_ptr, ciphertext);

	AES128_Encrypt aes128_encrypt;
	aes128_encrypt.set_padding(false);

	aes128_encrypt.set_iv(&iv[0]);
	aes128_encrypt.set_key(&key[0]);
	aes128_encrypt.add(&plaintext[0], plaintext.size());
	aes128_encrypt.calculate();
	DataBuffer buffer = aes128_encrypt.get_data();
	if (buffer.get_size() != ciphertext.size())
		fail();
	unsigned char *data_ptr = (unsigned char *) buffer.get_data();
	if (memcmp(data_ptr, &ciphertext[0], ciphertext.size()))
		fail();

	AES128_Decrypt aes128_decrypt;
	aes128_decrypt.set_padding(false);

	aes128_decrypt.set_iv(&iv[0]);
	aes128_decrypt.set_key(&key[0]);
	aes128_decrypt.add(data_ptr, buffer.get_size());
	bool result = aes128_decrypt.calculate();
	if (!result)
		fail();
	DataBuffer buffer2 = aes128_decrypt.get_data();
	if (buffer2.get_size() != plaintext.size())
		fail();
	unsigned char *data_ptr2 = (unsigned char *) buffer2.get_data();
	if (memcmp(data_ptr2, &plaintext[0], plaintext.size()))
		fail();

}

