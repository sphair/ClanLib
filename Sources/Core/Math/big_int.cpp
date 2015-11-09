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
**    Michael J. Fromberger
*/

// This class is based on the original MPI library (not NSS, because of license restrictions) with some modifications.
// Some ideas and algorithms are from NSS (Netscape Security Suite).  Where they have been used, the function contains a reference note
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

#include "Core/precomp.h"
#include "API/Core/Math/big_int.h"
#include "API/Core/Math/cl_math.h"
#include "big_int_impl.h"
#include <cstdlib>

namespace clan
{
	BigInt::BigInt() : impl(clan::make_unique<BigInt_Impl>(BigInt_Impl::default_allocated_precision))
	{
	}

	BigInt::BigInt(uint32_t value) : impl(clan::make_unique<BigInt_Impl>(BigInt_Impl::default_allocated_precision))
	{
		set(value);
	}

	BigInt::BigInt(int32_t value) : impl(clan::make_unique<BigInt_Impl>(BigInt_Impl::default_allocated_precision))
	{
		set(value);
	}

	BigInt::BigInt(uint64_t value) : impl(clan::make_unique<BigInt_Impl>(BigInt_Impl::default_allocated_precision))
	{
		set(value);
	}

	BigInt::BigInt(int64_t value) : impl(clan::make_unique<BigInt_Impl>(BigInt_Impl::default_allocated_precision))
	{
		set(value);
	}

	BigInt::BigInt(const BigInt &other)
	{
		if (&other != this)
		{
			impl = std::unique_ptr<BigInt_Impl>(new BigInt_Impl(*other.impl.get()));
		}
	}

	BigInt &BigInt::operator=(const BigInt& other)
	{
		if (&other != this)
		{
			other.impl->copy(this->impl.get());
		}
		return *this;
	}

	BigInt::~BigInt()
	{
	}

	void BigInt::zero()
	{
		impl->zero();
	}

	int BigInt::cmp_z() const
	{
		return impl->cmp_z();
	}

	void BigInt::read_unsigned_octets(const unsigned char *input_str, unsigned int input_length)
	{
		impl->read_unsigned_octets(input_str, input_length);
	}

	void BigInt::set_bit(unsigned int bit_number, unsigned int value)
	{
		impl->set_bit(bit_number, value);
	}

	int BigInt::significant_bits() const
	{
		return impl->significant_bits();
	}

	void BigInt::div_d(uint32_t d, BigInt *q, uint32_t *r) const
	{
		impl->div_d(d, q->impl.get(), r);
	}

	uint32_t BigInt::mod_d(uint32_t d) const
	{
		return impl->mod_d(d);
	}

	void BigInt::sieve(const uint32_t *primes, unsigned int num_primes, std::vector<unsigned char> &sieve)
	{
		impl->sieve(primes, num_primes, sieve);
	}

	void BigInt::set(uint32_t d)
	{
		impl->set(d);
	}

	void BigInt::set(int32_t d)
	{
		impl->set(d);
	}

	void BigInt::set(uint64_t d)
	{
		impl->set(d);
	}

	void BigInt::set(int64_t d)
	{
		impl->set(d);
	}

	void BigInt::get(uint32_t &d)
	{
		impl->get(d);
	}

	void BigInt::get(int32_t &d)
	{
		impl->get(d);
	}

	void BigInt::get(uint64_t &d)
	{
		impl->get(d);
	}

	void BigInt::get(int64_t &d)
	{
		impl->get(d);
	}

	void BigInt::div(uint32_t d, BigInt *q, BigInt *r) const
	{
		impl->div_2d(d, q->impl.get(), r->impl.get());
	}

	void BigInt::mod(const BigInt *m, BigInt *c) const
	{
		impl->mod(m->impl.get(), c->impl.get());
	}

	BigInt BigInt::operator + (const BigInt& b)
	{
		BigInt c;
		impl->add(b.impl.get(), c.impl.get());
		return c;
	}
	BigInt BigInt::operator + (uint32_t d)
	{
		BigInt c;
		impl->add_d(d, c.impl.get());
		return c;
	}

	BigInt BigInt::operator += (const BigInt& b)
	{
		BigInt c;
		impl->add(b.impl.get(), impl.get());
		return *this;
	}

	BigInt BigInt::operator += (uint32_t d)
	{
		BigInt c;
		impl->add_d(d, impl.get());
		return *this;
	}

	BigInt BigInt::operator - (const BigInt& b)
	{
		BigInt c;
		impl->sub(b.impl.get(), c.impl.get());
		return c;
	}

	BigInt BigInt::operator - (uint32_t d)
	{
		BigInt c;
		impl->sub_d(d, c.impl.get());
		return c;
	}

	BigInt BigInt::operator -= (const BigInt& b)
	{
		impl->sub(b.impl.get(), impl.get());
		return *this;
	}

	BigInt BigInt::operator -= (uint32_t d)
	{
		impl->sub_d(d, impl.get());
		return *this;
	}

	BigInt BigInt::operator * (const BigInt& b)
	{
		BigInt c;
		impl->mul(b.impl.get(), c.impl.get());
		return c;
	}

	BigInt BigInt::operator * (uint32_t d)
	{
		BigInt c;
		impl->mul_d(d, c.impl.get());
		return c;
	}

	BigInt BigInt::operator *= (const BigInt& b)
	{
		BigInt c;
		impl->mul(b.impl.get(), impl.get());
		return *this;
	}

	BigInt BigInt::operator *= (uint32_t d)
	{
		BigInt c;
		impl->mul_d(d, impl.get());
		return *this;
	}

	BigInt BigInt::operator / (const BigInt& b)
	{
		BigInt c;
		impl->div(b.impl.get(), c.impl.get(), nullptr);
		return c;
	}

	BigInt BigInt::operator / (uint32_t d)
	{
		BigInt c;
		impl->div_d(d, c.impl.get(), nullptr);
		return c;
	}

	BigInt BigInt::operator /= (const BigInt& b)
	{
		BigInt c;
		impl->div(b.impl.get(), impl.get(), nullptr);
		return *this;
	}

	BigInt BigInt::operator /= (uint32_t d)
	{
		BigInt c;
		impl->div_d(d, impl.get(), nullptr);
		return *this;
	}

	BigInt BigInt::operator % (const BigInt& b)
	{
		BigInt c;
		impl->div(b.impl.get(), nullptr, c.impl.get());
		return c;
	}

	BigInt BigInt::operator % (uint32_t d)
	{
		BigInt c;
		impl->div_2d(d, nullptr, c.impl.get());
		return c;
	}

	BigInt BigInt::operator %= (const BigInt& b)
	{
		BigInt c;
		impl->div(b.impl.get(), nullptr, impl.get());
		return *this;
	}

	BigInt BigInt::operator %= (uint32_t d)
	{
		BigInt c;
		impl->div_2d(d, nullptr, impl.get());
		return *this;
	}

	void BigInt::div(const BigInt &b, BigInt *q, BigInt *r) const
	{
		impl->div(b.impl.get(), q->impl.get(), r->impl.get());
	}


	int BigInt::cmp(const BigInt *b) const
	{
		return impl->cmp(b->impl.get());
	}

	void BigInt::exptmod(const BigInt *b, const BigInt *m, BigInt *c) const
	{
		impl->exptmod(b->impl.get(), m->impl.get(), c->impl.get());
	}

	bool BigInt::fermat(uint32_t w) const
	{
		return impl->fermat(w);
	}

	int BigInt::cmp_d(uint32_t d) const
	{
		return impl->cmp_d(d);
	}

	void BigInt::neg(BigInt *b) const
	{
		impl->neg(b->impl.get());
	}

	unsigned int BigInt::trailing_zeros() const
	{
		return impl->trailing_zeros();
	}

	void BigInt::sqr(BigInt *b) const
	{
		impl->sqr(b->impl.get());
	}

	void BigInt::sqrmod(const BigInt *m, BigInt *c) const
	{
		impl->sqrmod(m->impl.get(), c->impl.get());
	}

	void BigInt::random()
	{
		impl->random();
	}

	bool BigInt::pprime(int nt) const
	{
		return impl->pprime(nt);
	}

	bool BigInt::invmod(const BigInt *m, BigInt *c) const
	{
		return impl->invmod(m->impl.get(), c->impl.get());
	}

	void BigInt::abs(BigInt *b) const
	{
		impl->abs(b->impl.get());
	}

	bool BigInt::is_even() const
	{
		return impl->iseven() != 0;
	}

	bool BigInt::is_odd() const
	{
		return impl->isodd() != 0;
	}

	void BigInt::div_2(BigInt *c) const
	{
		impl->div_2(c->impl.get());
	}

	void BigInt::xgcd(const BigInt *b, BigInt *g, BigInt *x, BigInt *y) const
	{
		impl->xgcd(b->impl.get(), g->impl.get(), x->impl.get(), y->impl.get());
	}

	void BigInt::exch(BigInt *mp2)
	{
		impl->exch(mp2->impl.get());
	}

	bool BigInt::make_prime(unsigned int num_bits)
	{
		return impl->make_prime(num_bits);
	}

	int BigInt::unsigned_octet_size() const
	{
		return impl->unsigned_octet_size();
	}

	void BigInt::to_unsigned_octets(unsigned char *output_str, unsigned int output_length) const
	{
		impl->to_unsigned_octets(output_str, output_length);
	}
}
