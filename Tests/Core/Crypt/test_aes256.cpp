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

void TestApp::test_aes256()
{
	Console::write_line(" Header: aes256_encrypt.h and aes256_decrypt.h");
	Console::write_line("  Class: AES256_Encrypt and AES256_Decrypt");

	// Test data from http://csrc.nist.gov/publications/nistpubs/800-38a/sp800-38a.pdf
	// and http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/AES_CBC.pdf

	test_aes256_helper(
		"603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4",	// KEY
		"000102030405060708090A0B0C0D0E0F",	// IV
		"6bc1bee22e409f96e93d7e117393172a"	// PLAINTEXT
		"ae2d8a571e03ac9c9eb76fac45af8e51"
		"30c81c46a35ce411e5fbc1191a0a52ef"
		"f69f2445df4f9b17ad2b417be66c3710",
		"f58c4c04d6e5f1ba779eabfb5f7bfbd6"  // CIPHERTEXT
		"9cfc4e967edb808d679f777bc6702c7d"
		"39f23369a9d9bacfa530e26304231461"
		"b2eb05e2c39be9fcda6c19078c6a9d1b"
		);
		
	const int test_data_length = 256;
	unsigned char test_data[test_data_length];
	std::vector<unsigned char> key;
	std::vector<unsigned char> iv;
	convert_ascii("603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4", key);
	convert_ascii("000102030405060708090A0B0C0D0E0F", iv);

	for (int cnt=0; cnt<test_data_length; cnt++)
	{
		test_data[cnt] = (unsigned char) cnt;

		AES256_Encrypt aes256_encrypt;
		aes256_encrypt.set_iv(&iv[0]);
		aes256_encrypt.set_key(&key[0]);
		aes256_encrypt.add(test_data, cnt+1);
		aes256_encrypt.calculate();

		AES256_Decrypt aes256_decrypt;
		aes256_decrypt.set_iv(&iv[0]);
		aes256_decrypt.set_key(&key[0]);
		DataBuffer buffer = aes256_encrypt.get_data();
		aes256_decrypt.add(buffer.get_data(), buffer.get_size());
		bool result = aes256_decrypt.calculate();
		if (!result)
			fail();
		DataBuffer buffer2 = aes256_decrypt.get_data();
		if (buffer2.get_size() != cnt+1)
			fail();
		unsigned char *data_ptr2 = (unsigned char *) buffer2.get_data();
		if (memcmp(data_ptr2, test_data, cnt+1))
			fail();
	}

}

void TestApp::test_aes256_helper(const char *key_ptr, const char *iv_ptr, const char *plaintext_ptr, const char *ciphertext_ptr)
{
	std::vector<unsigned char> key;
	std::vector<unsigned char> iv;
	std::vector<unsigned char> plaintext;
	std::vector<unsigned char> ciphertext;

	convert_ascii(key_ptr, key);
	convert_ascii(iv_ptr, iv);
	convert_ascii(plaintext_ptr, plaintext);
	convert_ascii(ciphertext_ptr, ciphertext);

	AES256_Encrypt aes256_encrypt;
	aes256_encrypt.set_padding(false);

	aes256_encrypt.set_iv(&iv[0]);
	aes256_encrypt.set_key(&key[0]);
	aes256_encrypt.add(&plaintext[0], plaintext.size());
	aes256_encrypt.calculate();
	DataBuffer buffer = aes256_encrypt.get_data();
	if (buffer.get_size() != ciphertext.size())
		fail();
	unsigned char *data_ptr = (unsigned char *) buffer.get_data();
	if (memcmp(data_ptr, &ciphertext[0], ciphertext.size()))
		fail();

	AES256_Decrypt aes256_decrypt;
	aes256_decrypt.set_padding(false);

	aes256_decrypt.set_iv(&iv[0]);
	aes256_decrypt.set_key(&key[0]);
	aes256_decrypt.add(data_ptr, buffer.get_size());
	bool result = aes256_decrypt.calculate();
	if (!result)
		fail();
	DataBuffer buffer2 = aes256_decrypt.get_data();
	if (buffer2.get_size() != plaintext.size())
		fail();
	unsigned char *data_ptr2 = (unsigned char *) buffer2.get_data();
	if (memcmp(data_ptr2, &plaintext[0], plaintext.size()))
		fail();

}

