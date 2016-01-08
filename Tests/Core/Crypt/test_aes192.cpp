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

void TestApp::test_aes192()
{
	Console::write_line(" Header: aes192_encrypt.h and aes192_decrypt.h");
	Console::write_line("  Class: AES192_Encrypt and AES192_Decrypt");

	// Test data from http://csrc.nist.gov/publications/nistpubs/800-38a/sp800-38a.pdf
	// and http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/AES_CBC.pdf

	test_aes192_helper(
		"8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b",	// KEY
		"000102030405060708090A0B0C0D0E0F",	// IV
		"6bc1bee22e409f96e93d7e117393172a"	// PLAINTEXT
		"ae2d8a571e03ac9c9eb76fac45af8e51"
		"30c81c46a35ce411e5fbc1191a0a52ef"
		"f69f2445df4f9b17ad2b417be66c3710",
		"4f021db243bc633d7178183a9fa071e8"  // CIPHERTEXT
		"b4d9ada9ad7dedf4e5e738763f69145a"
		"571b242012fb7ae07fa9baac3df102e0"
		"08b0e27988598881d920a9e64f5615cd"
		);
		
	const int test_data_length = 192;
	unsigned char test_data[test_data_length];
	std::vector<unsigned char> key;
	std::vector<unsigned char> iv;
	convert_ascii("8E73B0F7DA0E6452C810F32B809079E562F8EAD2522C6B7B", key);
	convert_ascii("000102030405060708090A0B0C0D0E0F", iv);

	for (int cnt=0; cnt<test_data_length; cnt++)
	{
		test_data[cnt] = (unsigned char) cnt;

		AES192_Encrypt aes192_encrypt;
		aes192_encrypt.set_iv(&iv[0]);
		aes192_encrypt.set_key(&key[0]);
		aes192_encrypt.add(test_data, cnt+1);
		aes192_encrypt.calculate();

		AES192_Decrypt aes192_decrypt;
		aes192_decrypt.set_iv(&iv[0]);
		aes192_decrypt.set_key(&key[0]);
		DataBuffer buffer = aes192_encrypt.get_data();
		aes192_decrypt.add(buffer.get_data(), buffer.get_size());
		bool result = aes192_decrypt.calculate();
		if (!result)
			fail();
		DataBuffer buffer2 = aes192_decrypt.get_data();
		if (buffer2.get_size() != cnt+1)
			fail();
		unsigned char *data_ptr2 = (unsigned char *) buffer2.get_data();
		if (memcmp(data_ptr2, test_data, cnt+1))
			fail();
	}

}

void TestApp::test_aes192_helper(const char *key_ptr, const char *iv_ptr, const char *plaintext_ptr, const char *ciphertext_ptr)
{
	std::vector<unsigned char> key;
	std::vector<unsigned char> iv;
	std::vector<unsigned char> plaintext;
	std::vector<unsigned char> ciphertext;

	convert_ascii(key_ptr, key);
	convert_ascii(iv_ptr, iv);
	convert_ascii(plaintext_ptr, plaintext);
	convert_ascii(ciphertext_ptr, ciphertext);

	AES192_Encrypt aes192_encrypt;
	aes192_encrypt.set_padding(false);

	aes192_encrypt.set_iv(&iv[0]);
	aes192_encrypt.set_key(&key[0]);
	aes192_encrypt.add(&plaintext[0], plaintext.size());
	aes192_encrypt.calculate();
	DataBuffer buffer = aes192_encrypt.get_data();
	if (buffer.get_size() != ciphertext.size())
		fail();
	unsigned char *data_ptr = (unsigned char *) buffer.get_data();
	if (memcmp(data_ptr, &ciphertext[0], ciphertext.size()))
		fail();

	AES192_Decrypt aes192_decrypt;
	aes192_decrypt.set_padding(false);

	aes192_decrypt.set_iv(&iv[0]);
	aes192_decrypt.set_key(&key[0]);
	aes192_decrypt.add(data_ptr, buffer.get_size());
	bool result = aes192_decrypt.calculate();
	if (!result)
		fail();
	DataBuffer buffer2 = aes192_decrypt.get_data();
	if (buffer2.get_size() != plaintext.size())
		fail();
	unsigned char *data_ptr2 = (unsigned char *) buffer2.get_data();
	if (memcmp(data_ptr2, &plaintext[0], plaintext.size()))
		fail();

}

