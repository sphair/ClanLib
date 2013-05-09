/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Michael J. Fromberger
*/

// This class is based on the original MPI library (not NSS, because of license restrictions) with some modifications.
// Some ideas and algorithms are from NSS (Netscape Security Suite). Where they have been used, the function contains a reference note
//
// Note, since September 2011, I believe the MPI homepage is now: http://spinning-yarns.org/michael/mpi/
// Note, since 2013, the MPI page no longer exists, however the internet archive has the details here: http://web.archive.org/web/20100426001741/http://spinning-yarns.org/michael/mpi/README
// The license is as follows
// This software was written by Michael J. Fromberger,
//   http://www.dartmouth.edu/~sting/
//
// See the MPI home page at
//   http://www.dartmouth.edu/~sting/mpi/
//
// This software is in the public domain.  It is entirely free, and you
// may use it and/or redistribute it for whatever purpose you choose;
// however, as free software, it is provided without warranty of any
// kind, not even the implied warranty of merchantability or fitness for
// a particular purpose.

#pragma once

#include "API/Core/System/cl_platform.h"

namespace clan
{
class BigInt_Impl;

class BigInt_Impl
{

public:
	BigInt_Impl(unsigned int prec = default_allocated_precision);
	BigInt_Impl(const BigInt_Impl &other);
	~BigInt_Impl();

	void read_unsigned_octets( const unsigned char *input_str, unsigned int input_length);
	void zero();
	bool make_prime(unsigned int num_bits);
	int cmp_z() const;
	void set_bit(unsigned int bit_number, unsigned int value);
	int significant_bits() const;
	void sieve(const ubyte32 *primes, unsigned int num_primes, std::vector<unsigned char> &sieve);
	ubyte32 mod_d(ubyte32 d) const;
	void div_d(ubyte32 d, BigInt_Impl *q, ubyte32 *r) const;
	void copy(BigInt_Impl *to) const;
	void add_d(ubyte32 d, BigInt_Impl *out_b) const;
	bool fermat(ubyte32 w) const;
	bool pprime(int nt) const;
	void set(ubyte32 d);
	void exptmod(const BigInt_Impl *b, const BigInt_Impl *m, BigInt_Impl *c) const;
	void mod(const BigInt_Impl *m, BigInt_Impl *c) const;
	void div(const BigInt_Impl *b, BigInt_Impl *q, BigInt_Impl *r) const;
	void add(const BigInt_Impl *b, BigInt_Impl *c) const;
	void sub(const BigInt_Impl *b, BigInt_Impl *c) const;
	int cmp(const BigInt_Impl *b) const;
	int cmp_d(ubyte32 d) const;
	void sub_d(ubyte32 d, BigInt_Impl *b) const;
	void neg(BigInt_Impl *b) const;
	unsigned int trailing_zeros() const;
	void div_2d(ubyte32 d, BigInt_Impl *q, BigInt_Impl *r) const;
	void sqrmod(const BigInt_Impl *m, BigInt_Impl *c) const;
	void sqr(BigInt_Impl *b) const;
	void random();
	void exch(BigInt_Impl *mp2);
	void mul(const BigInt_Impl *b, BigInt_Impl *c) const;
	bool invmod(const BigInt_Impl *m, BigInt_Impl *c) const;
	void xgcd(const BigInt_Impl *b, BigInt_Impl *g, BigInt_Impl *x, BigInt_Impl *y) const;
	void abs(BigInt_Impl *b) const;
	int iseven() const;
	int isodd() const;
	void div_2(BigInt_Impl *c) const;
	void to_unsigned_octets( unsigned char *output_str, unsigned int output_length) const;
	int unsigned_octet_size() const;

	static const int default_allocated_precision = 64;

private:
	static const int num_bits_in_digit = (8*sizeof(ubyte32));
	static const int num_bits_in_word = (8*sizeof(ubyte64));
	static const ubyte64 digit_radix = 1ULL << (8*sizeof(ubyte32));
	static const ubyte32 digit_half_radix = 1U << (8*sizeof(ubyte32) - 1);
	static const ubyte64 word_maximim_value = ~0;

	static const int prime_tab_size = 6542;
	static std::vector<ubyte32> prime_tab;

	static void build_primes();

	static inline ubyte32 BigInt_Impl::internal_carryout(ubyte64 w)
	{
		return w >> num_bits_in_digit;
	}

	static inline ubyte32 BigInt_Impl::internal_accum(ubyte64 w)
	{
		return (ubyte32) w;
	}

	void internal_init_size(unsigned int prec);
	void internal_free();
	void internal_lshd(unsigned int p);
	void internal_pad(unsigned int min);
	void internal_grow(unsigned int min);
	void internal_clamp();
	int internal_ispow2d(ubyte32 d) const;
	void internal_div_2d(ubyte32 d);
	void internal_rshd(unsigned int p);
	int  internal_ispow2() const;
	void internal_mod_2d(ubyte32 d);
	void internal_mul_2d(ubyte32 d);

	// Exchange the data for a and b; (b, a) = (a, b)
	void internal_exch(BigInt_Impl *b);

	// Compare |a| <=> d, return 0 if equal, <0 if a<d, >0 if a>d
	int internal_cmp_d(ubyte32 d) const;

	void internal_div_d(ubyte32 d, ubyte32 *r);

	void internal_add_d(ubyte32 d);
	void internal_sub_d(ubyte32 d);
	ubyte32 internal_norm(BigInt_Impl *b);
	void internal_sub(const BigInt_Impl *b);

	int internal_cmp(const BigInt_Impl *b) const;
	void internal_div(BigInt_Impl *b);
	void internal_mul_d(ubyte32 d);
	void internal_add(const BigInt_Impl *b);
	void internal_mul(const BigInt_Impl *b);
	void internal_div_2();
	void internal_mul_2();

	void internal_reduce(const BigInt_Impl *m, BigInt_Impl *mu);
	void internal_sqr();

	bool digits_negative;	// True if the value is negative
	unsigned int digits_alloc;		// How many digits allocated
	unsigned int digits_used;		// How many digits used
	ubyte32 *digits;	// The digits themselves

	BigInt_Impl &operator=(const BigInt_Impl& other);	// Not defined

};

}

