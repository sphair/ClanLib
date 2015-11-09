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
**    (if your name is missing here, please add it)
*/

#include <ClanLib/core.h>
using namespace clan;

#ifndef WIN32
#include <unistd.h>
#include <stdio.h>
#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#else
#include <direct.h>
#ifndef chdir
#define _chdir chdir
#endif
#ifndef MAX_PATH
#define _MAX_PATH MAX_PATH
#endif
#include <tchar.h>
#endif


#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include <cstring>

class TestApp
{
public:
	int main();
private:
	void test_aes128();
	void test_aes128_helper(const char *key_ptr, const char *iv_ptr, const char *plaintext_ptr, const char *ciphertext_ptr);
	void test_aes192();
	void test_aes192_helper(const char *key_ptr, const char *iv_ptr, const char *plaintext_ptr, const char *ciphertext_ptr);
	void test_aes256();
	void test_aes256_helper(const char *key_ptr, const char *iv_ptr, const char *plaintext_ptr, const char *ciphertext_ptr);
	void convert_ascii(const char *src, std::vector<unsigned char> &dest);

	void test_rsa();
	void test_md5();
	void test_hash(const MD5 &sha1, const char *hash_text);
	void test_sha1();
	void test_hash(const SHA1 &sha1, const char *hash_text);
	void test_sha224();
	void test_hash(const SHA224 &sha224, const char *hash_text);
	void test_sha256();
	void test_hash(const SHA256 &sha256, const char *hash_text);
	void test_sha384();
	void test_hash(const SHA384 &sha384, const char *hash_text);
	void test_sha512();
	void test_hash(const SHA512 &sha512, const char *hash_text);
	void test_sha512_224();
	void test_hash(const SHA512_224 &sha512_224, const char *hash_text);
	void test_sha512_256();
	void test_hash(const SHA512_256 &sha512_256, const char *hash_text);
public:
	void fail() const;

};

