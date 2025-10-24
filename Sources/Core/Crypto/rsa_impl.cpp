/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Magnus Norddahl
**    Mark Page
*/

#include "Core/precomp.h"
#include "rsa_impl.h"

#include "../../API/Core/Math/cl_math.h"
#include "../../API/Core/Crypto/secret.h"
#include "../../API/Core/Crypto/random.h"

#ifndef WIN32
#include <cstring>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_RSA_Impl Construction:

CL_RSA_Impl::CL_RSA_Impl()
{

}

/////////////////////////////////////////////////////////////////////////////
// CL_RSA_Impl Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_RSA_Impl Operations:

void CL_RSA_Impl::create(CL_Random &random, int key_size_in_bits, int public_exponent_value)
{
	// Algorithm from Mozilla NSS
	// Generate and return a new RSA public and private key.

	const int public_exponent_size = 4;
	unsigned char public_exponent[public_exponent_size];

	// Convert public_exponent_value to a valid CL_BigInt number
	int public_exponent_written = 0;
	for (int cnt=0; cnt < public_exponent_size; cnt++)
	{
		if (public_exponent_written || (public_exponent_value & 0xff000000 >> (cnt * 8)))	// Ignore top zeros
		{
			public_exponent[public_exponent_written] = (unsigned char) ((public_exponent_value >> (3-cnt) * 8));
			public_exponent_written++;
		}
	}

    unsigned int prime_len = key_size_in_bits / 16;

	// Require key size to be a multiple of 16 bits
	if ( (prime_len * 16) != key_size_in_bits )
		throw CL_Exception("Key size must be a multiple of 16 bits");

    CL_BigInt p, q, e, d;

	e.read_unsigned_octets(public_exponent, public_exponent_written);

    int kiter = 0;

	const int max_key_gen_attempts = 10;

	bool created_keypair = false;
	do
	{

		generate_prime(random, p, prime_len);
		generate_prime(random, q, prime_len);

		// Assure q < p
		if (p.cmp(&q) < 0)
			p.exch(&q);

		// Attempt to use these primes to generate a key
		if (build_from_primes(&p, &q, &e, &d, key_size_in_bits))
		{
			created_keypair = true;
			break;	// generated two good primes
		}

		kiter++;

	} while (kiter < max_key_gen_attempts);

	if (!created_keypair)
		throw CL_Exception("Cannot create the key pair after lots of attempts");

}

bool CL_RSA_Impl::build_from_primes(CL_BigInt *p, CL_BigInt *q, CL_BigInt *e, CL_BigInt *d, unsigned int key_size_in_bits)
{

	CL_BigInt n, phi;
	CL_BigInt psub1, qsub1, tmp;

	// 1.  Compute n = p*q
	p->mul(q, &n);

	// verify that the modulus has the desired number of bits
	if (n.significant_bits() != key_size_in_bits)
	{
		return false;
	}

	// 2.  Compute phi = (p-1)*(q-1)
	p->sub_d( 1, &psub1);
	q->sub_d( 1, &qsub1);

	psub1.mul(&qsub1, &phi);

	// 3.  Compute e = d**-1 mod(phi)
	bool inverse_found = e->invmod( &phi, d);

	// Verify that phi(n) and e have no common divisors
	if (!inverse_found)
		return false;

	// 4.  Compute exponent1 = d mod (p-1)
	d->mod(&psub1, &tmp);
	//tmp.to_unsigned_octets(rsa_private_key.exponent1);
	rsa_private_key.exponent1 = tmp;

	// 5.  Compute exponent2 = d mod (q-1)
	d->mod(&qsub1, &tmp);
	//tmp.to_unsigned_octets(rsa_private_key.exponent2);
	rsa_private_key.exponent2 = tmp;


	// 6.  Compute coefficient = q**-1 mod p
	q->invmod(p, &tmp);
	//tmp.to_unsigned_octets(rsa_private_key.coefficient);
	rsa_private_key.coefficient = tmp;

	// And copy the rest
	rsa_private_key.modulus = n;
	rsa_private_key.private_exponent = *d;
	rsa_private_key.public_exponent = *e;
	rsa_private_key.prime1 = *p;
	rsa_private_key.prime2 = *q;
	//n.to_unsigned_octets(rsa_private_key.modulus);
	//d->to_unsigned_octets(rsa_private_key.private_exponent);
	//e->to_unsigned_octets(rsa_private_key.public_exponent);
	//p->to_unsigned_octets(rsa_private_key.prime1);
	//q->to_unsigned_octets(rsa_private_key.prime2);

	return true;
}

void CL_RSA_Impl::generate_prime(CL_Random &random, CL_BigInt &prime, int prime_len)
{
	int piter;

	CL_Secret random_key(prime_len);
	unsigned char *random_bytes_ptr = random_key.get_data();	// Ready to write to

	const int max_prime_gen_attempts = 10;

	for (piter = 0; piter < max_prime_gen_attempts; piter++)
	{
		random.get_random_bytes(random_bytes_ptr, prime_len);

		random_bytes_ptr[0] |= 0xC0; // set two high-order bits
		random_bytes_ptr[prime_len-1] |= 0x01; // set low-order bit

		prime.read_unsigned_octets(random_bytes_ptr, prime_len);

		if (prime.make_prime(prime_len * 8))
			return;
	}
	throw CL_Exception("Cannot create a prime number");
}


void CL_RSA_Impl::rsaep(CL_BigInt *msg, const CL_BigInt *e, const CL_BigInt *modulus, CL_BigInt *cipher)
{
	// Insure that message representative is in range of modulus
	if((msg->cmp_z() < 0) || (msg->cmp(modulus) >= 0))
	{
		throw CL_Exception("Message is out of range of modulus");
	}

	msg->exptmod(e, modulus, cipher);

}

void CL_RSA_Impl::rsadp(CL_BigInt *cipher, const CL_BigInt *d, const CL_BigInt *modulus, CL_BigInt *msg)
{
	// Insure that ciphertext representative is in range of modulus
	if((cipher->cmp_z() < 0) || (cipher->cmp(modulus) >= 0))
	{
		throw CL_Exception("ciphertext is out of range of modulus");
	}

	cipher->exptmod(d, modulus, msg);
}

void CL_RSA_Impl::pkcs1v15_encode(int block_type, CL_Random &random, const char *msg, int mlen, char *emsg, int emlen)
{
	if(mlen > emlen - 11)
		throw CL_Exception("input length is out of range of output length");

	//  EB = 00 || BT || PS || 00 || D
	*(emsg++) = 0;
	*(emsg++) = block_type;

	int length = emlen - mlen - 3;
	switch (block_type)
	{
		case 1:
			memset(emsg, 0xff, length);
			break;
		case 2:
			random.get_random_bytes_nzero((unsigned char *) emsg, length);	// generate random pad
			break;
		default:
			// We do not support 0 because it is not used and it makes the decode more difficult to code
			//case 0:
			// memset(emsg, 0, length);
			// break;
			throw CL_Exception("PKCS1 v1.5 block type not supported");
	}
	emsg += length;
	*(emsg++) = 0;

	memcpy(emsg, msg, mlen);
}

CL_Secret CL_RSA_Impl::pkcs1v15_decode(const char *emsg, int emlen)
{
	int    ix, jx, outlen;

	// If message is less than minimum length, it's an error
	if(emlen < 11)
		throw CL_Exception("message is less than minimum length");

	if (emsg[0] != 0)
		throw CL_Exception("packet format error");

	int block_type = emsg[1];
	if ( ! ( (block_type == 1) || (block_type == 2) ) )
	{
		// We do not support block type of 0
		throw CL_Exception("packet format error");
	}

	// Look for zero separator
	for(ix = 2 + 8; ix < emlen; ix++)		// "As least 8 octets long PS"
		if(emsg[ix] == 0)
			break;

	if(ix == emlen)
		throw CL_Exception("zero sperator error");

	outlen = emlen - (ix + 1);
	CL_Secret buffer(outlen);

	if(outlen > 0)
		memmove(buffer.get_data(), emsg + (ix + 1), outlen);

	return buffer;
}

CL_DataBuffer CL_RSA_Impl::pkcs1v15_encrypt(int block_type, CL_Random &random, const char *msg, int mlen, const CL_BigInt *e, const CL_BigInt *modulus)
{
	int k = modulus->unsigned_octet_size();	// length of modulus, in bytes

	CL_Secret key(k);

	char *buf = (char *)key.get_data();

	// Encode according to PKCS #1 v1.5
	pkcs1v15_encode(block_type, random, msg, mlen, buf, k);

	// Convert encoded message to a big number for encryption
	CL_BigInt mrep;

	mrep.read_unsigned_octets(key.get_data(), key.get_size());

	// Now, encrypt...
	rsaep(&mrep, e, modulus, &mrep);

	// Unpack message representative...
	CL_DataBuffer buffer(mrep.unsigned_octet_size());
	mrep.to_unsigned_octets((unsigned char *) buffer.get_data(), buffer.get_size());
	return buffer;
}

CL_Secret CL_RSA_Impl::pkcs1v15_decrypt(const char *msg, int mlen, const CL_BigInt *d, const CL_BigInt *modulus)
{
	int     k;

	k = modulus->unsigned_octet_size();		// size of modulus, in bytes
	if(mlen != k)
		throw CL_Exception("Invalid message length");


	// Convert ciphertext to integer representative
	CL_BigInt  mrep;
	mrep.read_unsigned_octets((const unsigned char *) msg, mlen);

	// Decrypt ...
	rsadp(&mrep, d, modulus, &mrep);

	CL_Secret key_buffer(k);
	mrep.to_unsigned_octets(key_buffer.get_data(), k);
	return pkcs1v15_decode( (char *) key_buffer.get_data(), k);
}

CL_DataBuffer CL_RSA_Impl::encrypt(int block_type, CL_Random &random, const void *in_public_exponent, unsigned int in_public_exponent_size, const void *in_modulus, unsigned int in_modulus_size, const void *in_data, unsigned int in_data_size)
{
	CL_BigInt exponent;
	exponent.read_unsigned_octets((const unsigned char *) in_public_exponent, in_public_exponent_size);

	CL_BigInt modulus;
	modulus.read_unsigned_octets((const unsigned char *) in_modulus, in_modulus_size);

	return pkcs1v15_encrypt(block_type, random, (const char *) in_data, in_data_size, &exponent, &modulus);
}

CL_Secret CL_RSA_Impl::decrypt(const CL_Secret &in_private_exponent, const void *in_modulus, unsigned int in_modulus_size, const void *in_data, unsigned int in_data_size)
{
	CL_BigInt exponent;
	exponent.read_unsigned_octets((const unsigned char *) in_private_exponent.get_data(), in_private_exponent.get_size());

	CL_BigInt modulus;
	modulus.read_unsigned_octets((const unsigned char *) in_modulus, in_modulus_size);

	return pkcs1v15_decrypt((const char *) in_data, in_data_size, &exponent, &modulus);
}

void CL_RSA_Impl::create_keypair(CL_Random &random, CL_Secret &out_private_exponent, CL_DataBuffer &out_public_exponent, CL_DataBuffer &out_modulus, int key_size_in_bits, int public_exponent_value)
{
	create(random, key_size_in_bits, public_exponent_value);
	out_public_exponent = CL_DataBuffer(rsa_private_key.public_exponent.unsigned_octet_size());
	rsa_private_key.public_exponent.to_unsigned_octets((unsigned char *) out_public_exponent.get_data(), out_public_exponent.get_size());

	out_private_exponent = CL_Secret(rsa_private_key.private_exponent.unsigned_octet_size());
	rsa_private_key.private_exponent.to_unsigned_octets(out_private_exponent.get_data(), out_private_exponent.get_size());

	out_modulus = CL_DataBuffer(rsa_private_key.modulus.unsigned_octet_size());
	rsa_private_key.modulus.to_unsigned_octets((unsigned char *) out_modulus.get_data(), out_modulus.get_size());

}
