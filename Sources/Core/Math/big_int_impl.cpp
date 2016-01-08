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
#include "big_int_impl.h"
#include "API/Core/Math/big_int.h"
#include <cstdlib>

namespace clan
{
	const int BigInt_Impl::default_allocated_precision = 64;

	std::vector<uint32_t> BigInt_Impl::prime_tab;

	BigInt_Impl::BigInt_Impl(unsigned int prec) : digits_negative(false), digits_alloc(0), digits_used(0), digits(nullptr)
	{
		if (prec)
			internal_init_size(prec);
	}

	BigInt_Impl::BigInt_Impl(const BigInt_Impl &other) : digits_negative(false), digits_alloc(0), digits_used(0), digits(nullptr)
	{
		other.copy(this);
	}

	BigInt_Impl::~BigInt_Impl()
	{
		internal_free();
	}

	void BigInt_Impl::internal_init_size(unsigned int prec)
	{
		internal_free();
		digits = new uint32_t[prec];
		if (prec)
			memset(digits, 0, prec * sizeof(uint32_t));
		digits_alloc = prec;
		digits_used = 1;
		digits_negative = false;
	}

	void BigInt_Impl::internal_free()
	{
		// Remember to clear memory when deleting
		if (digits)
		{
			// memset(digits, 0, digits_alloc * sizeof(uint32_t)); <-- ClanLib does not pretend to be secure
			delete[] digits;
			digits = nullptr;

		}
		digits_negative = false;
		digits_alloc = 0;
		digits_used = 0;
	}

	void BigInt_Impl::zero()
	{
		memset(digits, 0, digits_alloc * sizeof(uint32_t));		// <-- Do not try to optimise this, it's possible code assumes unused digits are zero (unless you are willing to check the entire code)
		digits_used = 1;
		digits_negative = false;
	}

	int BigInt_Impl::cmp_z() const
	{
		if(digits_negative)
			return -1;

		if(digits_used == 1 && digits[0] == 0)
			return 0;

		return 1;
	}



	void BigInt_Impl::internal_lshd(unsigned int p)
	{
		// Shift leftward by p digits, growing if needed, and zero-filling
		// the in-shifted digits at the right end.  This is a convenient
		// alternative to multiplication by powers of the radix

		unsigned int  pos;
		uint32_t *dp;
		int ix;

		if(p == 0)
			return;

		internal_pad(digits_used + p);

		pos = digits_used - 1;
		dp = digits;

		// Shift all the significant figures over as needed
		for(ix = pos - p; ix >= 0; ix--)
			dp[ix + p] = dp[ix];

		// Fill the bottom digits with zeroes
		for(ix = 0; ix < p; ix++)
			dp[ix] = 0;
	}

	void BigInt_Impl::internal_pad(unsigned int min)
	{
		// Make sure the used size is at least 'min', growing if needed
		if (min > digits_used)
		{
			// Make sure there is room to increase precision
			if(min > digits_alloc)
			{
				internal_grow(min);
			}
			// Increase precision; should already be 0-filled
			digits_used = min;
	   }
	}

	void BigInt_Impl::internal_grow(unsigned int min)
	{
		// Make sure there are at least 'min' digits allocated
		if (min > digits_alloc)
		{

			// Set min to next nearest default precision block size
			min = ((min + (default_allocated_precision - 1)) / default_allocated_precision) * default_allocated_precision;

			auto tmp = new uint32_t[min];

			memcpy(tmp, digits, sizeof(uint32_t) * digits_used);
			memset(tmp + digits_used, 0, (min - digits_used) * sizeof(uint32_t));
			memset(digits, 0, digits_alloc * sizeof(uint32_t));
			delete[] digits;
			digits = tmp;
			digits_alloc = min;
		}
	}

	void BigInt_Impl::read_unsigned_octets( const unsigned char *input_str, unsigned int input_length)
	{
		zero();

		uint32_t d;
		unsigned int len = input_length;
		const unsigned char *str = input_str;

		int count = len % sizeof(uint32_t);
		if (count)
		{
			for (d = 0; count-- > 0; --len)
			{
				d = (d << 8) | *str++;
			}

			digits[0] = d;
		}

		// Read the rest of the digits
		for(; len > 0; len -= sizeof(uint32_t))
		{
			for (d = 0, count = sizeof(uint32_t); count > 0; --count)
			{
				 d = (d << 8) | *str++;
			}
			if (cmp_z() == 0)	// Ignore leading zeros
			{
				if (!d)
					continue;
			}
			else
			{
				internal_lshd(1);
			}
			digits[0] = d;
		}
	}

	void BigInt_Impl::internal_clamp()
	{
		unsigned int used = digits_used;
		while (used > 1 && digits[used - 1] == 0)
			--used;
		digits_used = used;
	}

	void BigInt_Impl::set_bit(unsigned int bit_number, unsigned int value)
	{
		unsigned int ix = bit_number / num_bits_in_digit;
		if (ix + 1 > digits_used)
		{
			internal_pad(ix + 1);
		}

		bit_number = bit_number % num_bits_in_digit;
		uint32_t mask = 1 << bit_number;

		if (value)
		{
			digits[ix] |= mask;
		}
		else
		{
			digits[ix] &= ~mask;
		}
		internal_clamp();
	}


	int BigInt_Impl::significant_bits() const
	{
		// Returns number of significnant bits in abs(a).
		// Returns 1 if value is zero.

		int bits = 0;
		int ix;

		for (ix = digits_used; ix > 0; )
		{
			uint32_t d;
			d = digits[--ix];
			if (d)
			{
				while (d)
				{
					++bits;
					d >>= 1;
				}
				break;
			}
		}
		bits += ix * num_bits_in_digit;
		if (!bits)
			bits = 1;
		return bits;
	}

	int BigInt_Impl::internal_cmp_d(uint32_t d) const
	{
		unsigned int  ua = digits_used;
		uint32_t *ap = digits;

		if(ua > 1)
			return 1;

		if(*ap < d)
			return -1;

		if(*ap > d)
			return 1;

		return 0;
	}

	int BigInt_Impl::internal_ispow2d(uint32_t d) const
	{
		int   pow = 0;

		while((d & 1) == 0)
		{
			++pow;
			d >>= 1;
		}

		if(d == 1)
			return pow;

		return -1;

	}


	void BigInt_Impl::copy(BigInt_Impl *to) const
	{
		if (to != this)
		{
			to->internal_free();
			to->digits_negative = digits_negative;
			to->digits_alloc = digits_alloc;
			to->digits_used = digits_used;
			to->digits = nullptr;
			if (digits)
			{
				to->digits = new uint32_t[digits_alloc];
				memcpy(to->digits, digits, digits_alloc * sizeof(uint32_t));
			}
		}
	}

	void BigInt_Impl::internal_rshd(unsigned int p)
	{

		// Shift rightward by p digits.  Maintains the invariant that
		// digits above the precision are all zero.  Digits shifted off the
		// end are lost.  Cannot fail.

		unsigned int  ix;
		uint32_t *dp;

		if(p == 0)
			return;

		// Shortcut when all digits are to be shifted off
		if(p >= digits_used)
		{
			memset(digits, 0, digits_alloc * sizeof(uint32_t));
			digits_used = 1;
			digits_negative = false;
			return;
		}

		// Shift all the significant figures over as needed
		dp = digits;
		for(ix = p; ix < digits_used; ix++)
			dp[ix - p] = dp[ix];

		// Fill the top digits with zeroes
		ix -= p;
		while(ix < digits_used)
			dp[ix++] = 0;

		// Strip off any leading zeroes
		internal_clamp();

	}

	void BigInt_Impl::internal_div_2d(uint32_t d)
	{
		// Divide the integer by 2^d, where d is a number of bits.  This
		// amounts to a bitwise shift of the value, and does not require the
		// full division code (used in Barrett reduction, see below)

		uint32_t save, next, mask, *dp = digits;

		internal_rshd(d / num_bits_in_digit);
		d %= num_bits_in_digit;

		mask = (1 << d) - 1;

		save = 0;
		for(int ix = digits_used - 1; ix >= 0; ix--)
		{
			next = dp[ix] & mask;
			dp[ix] = (dp[ix] >> d) | (save << (num_bits_in_digit - d));
			save = next;
		}

		internal_clamp();
	}



	void BigInt_Impl::internal_div_d(uint32_t d, uint32_t *r)
	{
		// Compute the quotient mp = mp / d and remainder r = mp mod d, for a
		// single digit d.  If r is null, the remainder will be discarded.

		uint64_t w = 0, t;
		BigInt_Impl quot_impl(digits_used);

		uint32_t *dp = digits, *qp;

		int ix;

		if(d == 0)
			throw Exception("Divide by zero");

		quot_impl.digits_used = digits_used; // so clamping will work below
		qp = quot_impl.digits;

		// Divide without subtraction
		for(ix = digits_used - 1; ix >= 0; ix--)
		{
			w = (w << num_bits_in_digit) | dp[ix];

			if(w >= d)
			{
				t = w / d;
				w = w % d;
			}
			else
			{
				t = 0;
			}
			qp[ix] = t;
		}

		// Deliver the remainder, if desired
		if(r)
			*r = w;

		quot_impl.internal_clamp();
		quot_impl.internal_exch(this);

	}

	void BigInt_Impl::internal_exch(BigInt_Impl *b)
	{
		bool b_digits_negative = b->digits_negative;
		unsigned int b_digits_alloc = b->digits_alloc;
		unsigned int b_digits_used = b->digits_used;
		uint32_t *b_digits = b->digits;

		b->digits_negative = digits_negative;
		b->digits_alloc = digits_alloc;
		b->digits_used = digits_used;
		b->digits = digits;

		digits_negative = b_digits_negative;
		digits_alloc = b_digits_alloc;
		digits_used = b_digits_used;
		digits = b_digits;
	}

	void BigInt_Impl::div_d(uint32_t d, BigInt_Impl *q, uint32_t *r) const
	{
		uint32_t rem;
		int      pow;

		if(d == 0)
			throw Exception("Divide by zero error");

		// Shortcut for powers of two ...
		if ((pow = internal_ispow2d(d)) >= 0)
		{
			uint32_t  mask;

			mask = (1 << pow) - 1;
			rem = digits[0] & mask;

			if(q)
			{
				copy(q);
				q->internal_div_2d(pow);
			}

			if(r)
				*r = rem;

			return;
		}


		//If the quotient is actually going to be returned, we'll try to
		//avoid hitting the memory allocator by copying the dividend into it
		//and doing the division there.  This can't be any _worse_ than
		//always copying, and will sometimes be better (since it won't make
		//another copy)

		//If it's not going to be returned, we need to allocate a temporary
		//to hold the quotient, which will just be discarded.

		if(q)
		{
			copy(q);
			q->internal_div_d(d, &rem);

			if(q->internal_cmp_d(0) == 0)
				q->digits_negative = false;

		}
		else
		{
			BigInt_Impl  qp(*this);
			qp.internal_div_d(d, &rem);
			if(qp.internal_cmp_d(0) == 0)
				qp.digits_negative = false;

		}

		if(r)
			*r = rem;

	}

	uint32_t BigInt_Impl::mod_d(uint32_t d) const
	{
		uint32_t rem;

		if(internal_cmp_d(d) > 0)
		{
			div_d(d, nullptr, &rem);
		}
		else
		{
			if (digits_negative)
			{
				rem = d - digits[0];
			}
			else
			{
				rem = digits[0];
			}
		}
		return rem;

	}

	void BigInt_Impl::sieve(const uint32_t *primes, unsigned int num_primes, std::vector<unsigned char> &sieve)
	{
		// Algorithm from Mozilla NSS

		// Produce table of composites from list of primes and trial value.
		// trial must be odd. List of primes must not include 2.
		// sieve should have dimension >= MAXPRIME/2, where MAXPRIME is largest
		// prime in list of primes.  After this function is finished,
		// if sieve[i] is non-zero, then (trial + 2*i) is composite.
		// Each prime used in the sieve costs one division of trial, and eliminates
		// one or more values from the search space. (3 eliminates 1/3 of the values
		// alone!)  Each value left in the search space costs 1 or more modular
		// exponentations.  So, these divisions are a bargain!

		//mp_err       res;
		//mp_digit     rem;
		//mp_size      ix;
		unsigned long offset;

		unsigned int sieve_size = sieve.size();

		unsigned char *sieve_ptr = &sieve[0];
		memset(sieve_ptr, 0, sieve_size);

		for(unsigned int ix = 0; ix < num_primes; ix++)
		{
			uint32_t prime = primes[ix];
			uint32_t rem = mod_d(prime);

			if (rem == 0)
			{
				offset = 0;
			}
			else
			{
				offset = prime - (rem / 2);
			}
			for (unsigned int i = offset; i < sieve_size ; i += prime)
			{
				sieve_ptr[i] = 1;
			}
		}
	}

	void BigInt_Impl::internal_add_d(uint32_t d)
	{
		// Add d to |mp| in place
		// unsigned digit addition

		uint64_t w;
		uint32_t k;
		unsigned int ix = 1, used = digits_used;
		uint32_t *dp = digits;

		w = dp[0] + d;
		dp[0] = internal_accum(w);
		k = internal_carryout(w);

		while(ix < used && k)
		{
			w = (uint64_t) dp[ix] + k;
			dp[ix] = internal_accum(w);
			k = internal_carryout(w);
			++ix;
		}

		if(k != 0)
		{
			internal_pad(digits_used + 1);
			digits[ix] = k;
		}

	}

	void BigInt_Impl::internal_sub_d(uint32_t d)
	{
		// Subtract d from |mp| in place, assumes |mp| > d
		// unsigned digit subtract

		uint64_t   w, b = 0;
		unsigned int   ix = 1, used = digits_used;
		uint32_t *dp = digits;

		// Compute initial subtraction
		w = (digit_radix + dp[0]) - d;
		b = internal_carryout(w) ? 0 : 1;
		dp[0] = internal_accum(w);

		// Propagate borrows leftward
		while(b && ix < used)
		{
			w = (digit_radix + dp[ix]) - b;
			b = internal_carryout(w) ? 0 : 1;
			dp[ix] = internal_accum(w);
			++ix;
		}

		// Remove leading zeroes
		internal_clamp();

		// If we have a borrow out, it's a violation of the input invariant
		if(b)
			throw Exception("Borrow out detected, internal error");

	}

	void BigInt_Impl::add_d(uint32_t d, BigInt_Impl *out_b) const
	{
		copy(out_b);

		if (!digits_negative)
		{
			out_b->internal_add_d(d);
		}
		else if (out_b->internal_cmp_d(d) >= 0)
		{
			out_b->internal_sub_d(d);
		}
		else
		{
			out_b->digits_negative = false;
			out_b->digits[0] = d - out_b->digits[0];
		}
	}

	void BigInt_Impl::set(uint32_t d)
	{
		zero();
		digits[0] = d;
	}

	void BigInt_Impl::set(uint64_t d)
	{
		zero();
		uint32_t d_low = d;
		uint32_t d_high = d>>32;

		if (d_high)
		{
			internal_pad(2);
			digits[1] = d_high;
		}

		digits[0] = d_low;


	}

	void BigInt_Impl::set(int32_t d)
	{
		zero();
		if (d <0)
		{
			d = -d;
			digits_negative = true;
		}
		digits[0] = d;
	}

	void BigInt_Impl::set(int64_t d)
	{
		zero();

		if (d <0)
		{
			d = -d;
			digits_negative = true;
		}

		uint32_t d_low = d;
		uint32_t d_high = d>>32;

		if (d_high)
		{
			internal_pad(2);
			digits[1] = d_high;
		}

		digits[0] = d_low;

	}

	int BigInt_Impl::internal_cmp(const BigInt_Impl *b) const
	{
		// Compare |a| <=> |b|, return 0 if equal, <0 if a<b, >0 if a>b
		unsigned int   ua = digits_used, ub = b->digits_used;

		if(ua > ub)
			return 1;
		else if(ua < ub)
			return -1;
		else
		{
			int ix = ua - 1;
			const uint32_t *ap = digits + ix, *bp = b->digits + ix;

			while(ix >= 0)
			{
				if(*ap > *bp)
					return 1;
				else if(*ap < *bp)
				return -1;

				--ap; --bp; --ix;
			}

			return 0;
		}
	}

	int  BigInt_Impl::internal_ispow2() const
	{
		// Returns -1 if the value is not a power of two; otherwise, it returns k such that v = 2^k, i.e. lg(v).

		uint32_t d;
		const uint32_t *dp;
		unsigned int  uv = digits_used;
		int      extra = 0, ix;

		d = digits[uv - 1]; // most significant digit of v

		while(d && ((d & 1) == 0))
		{
			d >>= 1;
			++extra;
		}

		if(d == 1)
		{
			ix = uv - 2;
			dp = digits + ix;

			while(ix >= 0)
			{
				if(*dp)
					return -1; // not a power of two

				--dp; --ix;
			}

			return ((uv - 1) * num_bits_in_digit) + extra;
		}

		return -1;
	}

	void BigInt_Impl::internal_mod_2d(uint32_t d)
	{
		// Remainder the integer by 2^d, where d is a number of bits.  This
		// amounts to a bitwise AND of the value, and does not require the full division code

		unsigned int  ndig = (d / num_bits_in_digit), nbit = (d % num_bits_in_digit);
		unsigned int  ix;
		uint32_t dmask, *dp = digits;

		if(ndig >= digits_used)
			return;

		// Flush all the bits above 2^d in its digit
		dmask = (1 << nbit) - 1;
		dp[ndig] &= dmask;

		// Flush all digits above the one with 2^d in it
		for(ix = ndig + 1; ix < digits_used; ix++)
			dp[ix] = 0;

		internal_clamp();

	}

	void BigInt_Impl::internal_mul_2d(uint32_t d)
	{
		// Multiply by the integer 2^d, where d is a number of bits.  This
		// amounts to a bitwise shift of the value, and does not require the
		// full multiplication code.

		uint32_t save, next, mask, *dp;
		unsigned int  used;
		unsigned int ix;

		internal_lshd(d / num_bits_in_digit);

		dp = digits; used = digits_used;
		d %= num_bits_in_digit;

		mask = (1 << d) - 1;

		// If the shift requires another digit, make sure we've got one to work with
		if((dp[used - 1] >> (num_bits_in_digit - d)) & mask)
		{
			internal_grow(used + 1);
			dp = digits;
		}

		// Do the shifting...
		save = 0;
		for(ix = 0; ix < used; ix++)
		{
			next = (dp[ix] >> (num_bits_in_digit - d)) & mask;
			dp[ix] = (dp[ix] << d) | save;
			save = next;
		}

		// If, at this point, we have a nonzero carryout into the next
		// digit, we'll increase the size by one digit, and store it...

		if(save)
		{
			dp[used] = save;
			digits_used += 1;
		}

		internal_clamp();
	}

	uint32_t BigInt_Impl::internal_norm(BigInt_Impl *b)
	{
		// Normalize a and b for division, where b is the divisor.  In order
		// that we might make good guesses for quotient digits, we want the
		// leading digit of b to be at least half the radix, which we
		// accomplish by multiplying a and b by a constant.  This constant is
		// returned (so that it can be divided back out of the remainder at the
		// end of the division process).

		// We multiply by the smallest power of 2 that gives us a leading digit
		// at least half the radix.  By choosing a power of 2, we simplify the
		// multiplication and division steps to simple shifts.


		uint32_t  t, d = 0;

		t = b->digits[b->digits_used - 1];
		while(t < digit_half_radix)
		{
			t <<= 1;
			++d;
		}

		if(d != 0)
		{
			internal_mul_2d(d);
			b->internal_mul_2d(d);
		}

		return d;

	}

	void BigInt_Impl::internal_mul_d(uint32_t d)
	{
		// Compute a = a * d, single digit multiplication
		uint64_t w, k = 0;
		unsigned int ix, max;
		uint32_t *dp = digits;


		// Single-digit multiplication will increase the precision of the
		// output by at most one digit.  However, we can detect when this
		// will happen -- if the high-order digit of a, times d, gives a
		// two-digit result, then the precision of the result will increase;
		// otherwise it won't.  We use this fact to avoid calling s_mp_pad()
		// unless absolutely necessary.

		max = digits_used;
		w = (uint64_t) dp[max - 1] * d;
		if(internal_carryout(w) != 0)
		{
			internal_pad(max + 1);
			dp = digits;
		}

		for(ix = 0; ix < max; ix++)
		{
			w = ((uint64_t) dp[ix] * (uint64_t) d) + k;
			dp[ix] = internal_accum(w);
			k = internal_carryout(w);
		}

		// If there is a precision increase, take care of it here; the above
		// test guarantees we have enough storage to do this safely.

		if(k)
		{
			dp[max] = k;
			digits_used = max + 1;
		}

		internal_clamp();
	}

	void BigInt_Impl::internal_sub(const BigInt_Impl *b)
	{
		// Compute a = |a| - |b|, assumes |a| >= |b|
		uint64_t   w = 0;
		uint32_t *pa;
		const uint32_t *pb;
		unsigned int   ix, used = b->digits_used;


		// Subtract and propagate borrow.  Up to the precision of b, this
		// accounts for the digits of b; after that, we just make sure the
		// carries get to the right place.  This saves having to pad b out to
		// the precision of a just to make the loops work right...

		pa = digits;
		pb = b->digits;

		for(ix = 0; ix < used; ++ix)
		{
			w = (digit_radix + *pa) - w - *pb++;
			*pa++ = internal_accum(w);
			w = internal_carryout(w) ? 0 : 1;
		}

		used = digits_used;
		while(ix < used)
		{
			w = digit_radix + *pa - w;
			*pa++ = internal_accum(w);
			w = internal_carryout(w) ? 0 : 1;
			++ix;
		}

		// Clobber any leading zeroes we created
		internal_clamp();

		// If there was a borrow out, then |b| > |a| in violation
		// of our input invariant.  We've already done the work,
		// but we'll at least complain about it...
		if(w)
			throw Exception("Internal violation error");

	}

	void BigInt_Impl::internal_div(BigInt_Impl *b)
	{
		// Compute a = a / b and b = a mod b.  Assumes b > a.

		uint64_t  q;
		uint32_t d;
		int      ix;

		if (b->cmp_z() == 0)
			throw Exception("Divide by zero");

		// Shortcut if b is power of two
		if((ix = b->internal_ispow2()) >= 0)
		{
			copy(b);	// need this for remainder
			internal_div_2d(ix);
			b->internal_mod_2d(ix);
			return;
		}

		BigInt_Impl quot_impl(digits_used);
		BigInt_Impl rem_impl(digits_used);
		BigInt_Impl t_impl(digits_used);

		// Normalize to optimize guessing
		d = internal_norm(b);

		// Perform the division itself...woo!
		ix = digits_used - 1;

		while(ix >= 0)
		{
			// Find a partial substring of a which is at least b
			while(rem_impl.internal_cmp(b) < 0 && ix >= 0)
			{
				rem_impl.internal_lshd(1);
				quot_impl.internal_lshd(1);
				rem_impl.digits[0] = digits[ix];
				rem_impl.internal_clamp();
				--ix;
			}

			// If we didn't find one, we're finished dividing
			if(rem_impl.internal_cmp(b) < 0)
				break;

			// Compute a guess for the next quotient digit
			q = rem_impl.digits[rem_impl.digits_used - 1];
			if(q <= b->digits[b->digits_used - 1] && rem_impl.digits_used > 1)
				q = (q << num_bits_in_digit) | rem_impl.digits[rem_impl.digits_used - 2];

			q /= (uint64_t) b->digits[b->digits_used - 1];

			// The guess can be as much as RADIX + 1
			if(q >= digit_radix)
				q = digit_radix - 1;

			// See what that multiplies out to
			b->copy(&t_impl);
			t_impl.internal_mul_d(q);

			// If it's too big, back it off.  We should not have to do this
			// more than once, or, in rare cases, twice.  Knuth describes a
			// method by which this could be reduced to a maximum of once, but
			// I didn't implement that here.
			while(t_impl.internal_cmp(&rem_impl) > 0)
			{
				--q;
				t_impl.internal_sub(b);
			}

			// At this point, q should be the right next digit
			rem_impl.internal_sub(&t_impl);

			// Include the digit in the quotient.  We allocated enough memory
			// for any quotient we could ever possibly get, so we should not
			// have to check for failures here
			quot_impl.digits[0] = q;
		}

		// Denormalize remainder
		if(d != 0)
			rem_impl.internal_div_2d(d);

		quot_impl.internal_clamp();
		rem_impl.internal_clamp();

		// Copy quotient back to output
		quot_impl.internal_exch(this);

		// Copy remainder back to output
		rem_impl.internal_exch(b);
	}

	void BigInt_Impl::div_2d(uint32_t d, BigInt_Impl *q, BigInt_Impl *r) const
	{
		if(q)
		{
			copy(q);
			q->internal_div_2d(d);
		}

		if(r)
		{
			copy(r);
			r->internal_mod_2d(d);
		}

	}

	void BigInt_Impl::div(const BigInt_Impl *b, BigInt_Impl *q, BigInt_Impl *r) const
	{
		int      cmp;

		if(b->cmp_z() == 0)
			throw Exception("Divide by zero");

		// If a <= b, we can compute the solution without division, and	avoid any memory allocation
		if((cmp = internal_cmp(b)) < 0)
		{
			if(r)
			{
				copy(r);
			}

			if(q)
				q->zero();

			return;

		} else if(cmp == 0)
		{

			// Set quotient to 1, with appropriate sign
			if(q)
			{
				int qneg = (digits_negative != b->digits_negative);

				q->set(1);
				if(qneg)
					q->digits_negative = true;
			}

			if(r)
				r->zero();

			return;
		}

		// If we get here, it means we actually have to do some division

		// Set up some temporaries...
		BigInt_Impl qtmp(*this);
		BigInt_Impl	rtmp(*b);

		qtmp.internal_div(&rtmp);

		// Compute the signs for the output
		rtmp.digits_negative = digits_negative; // Sr = Sa
		if (digits_negative == b->digits_negative)
			qtmp.digits_negative = false;		// Sq = MP_ZPOS if Sa = Sb
		else
			qtmp.digits_negative = true;		// Sq = MP_NEG if Sa != Sb

		if(qtmp.internal_cmp_d(0) == 0)
			qtmp.digits_negative = false;
		if(rtmp.internal_cmp_d(0) == 0)
			rtmp.digits_negative = false;

		// Copy output, if it is needed
		if(q)
			qtmp.internal_exch(q);

		if(r)
			rtmp.internal_exch(r);

	}

	void BigInt_Impl::add(const BigInt_Impl *b, BigInt_Impl *c) const
	{
		int cmp;

		if(digits_negative == b->digits_negative)	// same sign:  add values, keep sign
		{
			// Commutativity of addition lets us do this in either order,
			// so we avoid having to use a temporary even if the result
			// is supposed to replace the output

			if(c == b)
			{
				c->internal_add(this);
			}
			else
			{
				if(c != this)
					copy(c);

				c->internal_add(b);
			}

		} else if((cmp = internal_cmp(b)) > 0) // different sign: a > b
		{
			// If the output is going to be clobbered, we will use a temporary
			// variable; otherwise, we'll do it without touching the memory
			// allocator at all, if possible

			if(c == b)
			{
				BigInt_Impl tmp(*this);

				tmp.internal_sub(b);

				tmp.internal_exch(c);
			}
			else
			{
				if(c != this)
					copy(c);
				c->internal_sub(b);
			}

		}
		else if(cmp == 0)	// different sign, a == b
		{
			c->zero();
			return;
		}
		else		// different sign: a < b
		{
			// See above...
			if(c == this)
			{
				BigInt_Impl tmp(*b);
				tmp.internal_sub(this);

				tmp.internal_exch(c);
			}
			else
			{
				if(c != b)
					b->copy(c);

				c->internal_sub(this);
			}
		}

		if(c->digits_used == 1 && c->digits[0] == 0)
			c->digits_negative = false;
	}


	void BigInt_Impl::internal_add(const BigInt_Impl *b)
	{
		// Compute a = |a| + |b|
		uint64_t   w = 0;
		uint32_t *pa;
		const uint32_t *pb;
		unsigned int   ix, used = b->digits_used;

		// Make sure a has enough precision for the output value
		if(used > digits_used)
			internal_pad(used);

		// Add up all digits up to the precision of b.  If b had initially
		// the same precision as a, or greater, we took care of it by the
		// padding step above, so there is no problem.  If b had initially
		// less precision, we'll have to make sure the carry out is duly
		// propagated upward among the higher-order digits of the sum.

		pa = digits;
		pb = b->digits;
		for(ix = 0; ix < used; ++ix)
		{
			w += (uint64_t) *pa + (uint64_t) *pb++;
			*pa++ = internal_accum(w);
			w = internal_carryout(w);
		}

		// If we run out of 'b' digits before we're actually done, make	sure the carries get propagated upward...
		used = digits_used;
		while(w && ix < used)
		{
			w += (uint64_t) *pa;
			*pa++ = internal_accum(w);
			w = internal_carryout(w);
			++ix;
		}

		// If there's an overall carry out, increase precision and include
		// it.  We could have done this initially, but why touch the memory
		// allocator unless we're sure we have to?

		if(w)
		{
			internal_pad(used+1);
			digits[ix] = w;		// pa may not be valid after pad() call
		}
	}

	void BigInt_Impl::mod(const BigInt_Impl *m, BigInt_Impl *c) const
	{
		int  mag;

		if (m->digits_negative)
			throw Exception("Cannot mod() using negative values");

		// If |a| > m, we need to divide to get the remainder and take the absolute value.
		// If |a| < m, we don't need to do any division, just copy and adjust the sign (if a is negative).
		// If |a| == m, we can simply set the result to zero.

		//This order is intended to minimize the average path length of the
		//comparison chain on common workloads -- the most frequent cases are
		//that |a| != m, so we do those first.

		if((mag = internal_cmp(m)) > 0)
		{
			div(m, nullptr, c);

			if (c->digits_negative)
			{
				c->add(m, c);
			}
		}
		else if(mag < 0)
		{
			copy(c);

			if(cmp_z() < 0)
			{
				c->add(m, c);
			}
  		}
		else
		{
			c->zero();
		}
	}

	void BigInt_Impl::internal_mul(const BigInt_Impl *b)
	{
		// Compute a = |a| * |b|
		uint64_t   w, k = 0;
		unsigned int   ix, jx, ua = digits_used, ub = b->digits_used;
		uint32_t *pa;
		const uint32_t *pb;
		uint32_t *pt, *pbt;

		BigInt_Impl tmp_impl(ua + ub);

		// This has the effect of left-padding with zeroes...
		tmp_impl.digits_used = ua + ub;

		// We're going to need the base value each iteration
		pbt = tmp_impl.digits;

		// Outer loop:  Digits of b

		pb = b->digits;
		for(ix = 0; ix < ub; ++ix, ++pb)
		{
			if(*pb == 0)
			continue;

			// Inner product:  Digits of a
			pa = digits;
			for(jx = 0; jx < ua; ++jx, ++pa)
			{
				pt = pbt + ix + jx;
				w = (uint64_t) *pb * (uint64_t) *pa + k + (uint64_t) *pt;
				*pt = internal_accum(w);
				k = internal_carryout(w);
			}

			pbt[ix + jx] = k;
			k = 0;
		}

		tmp_impl.internal_clamp();
		tmp_impl.internal_exch(this);

	}

	/*
	  mp_sub(a, b, c)

	  Compute c = a - b.  All parameters may be identical.
	 */

	void BigInt_Impl::sub(const BigInt_Impl *b, BigInt_Impl *c) const
	{
		int     cmp;

		if(digits_negative != b->digits_negative)
		{
			if(c == this)
			{
				c->internal_add(b);
			}
			else
			{
				if (c != b)
					b->copy(c);
				c->internal_add(this);
				c->digits_negative = digits_negative;
			}

		}
		else if((cmp = internal_cmp(b)) > 0) // Same sign, a > b
		{
			if(c == b)
			{
				BigInt_Impl tmp(*this);
				tmp.internal_sub(b);
				tmp.internal_exch(c);
			}
			else
			{
				if(c != this)
					copy(c);

				c->internal_sub(b);
			}

		}
		else if(cmp == 0) // Same sign, equal magnitude
		{
			c->zero();
			return;

		}
		else	// Same sign, b > a
		{
			if(c == this)
			{
				BigInt_Impl tmp(*b);
				tmp.internal_sub(this);
				tmp.internal_exch(c);
			}
			else
			{
				if(c != b)
					b->copy(c);

				c->internal_sub(this);
			}
			c->digits_negative = !b->digits_negative;
		}

		if(c->digits_used == 1 && c->digits[0] == 0)
			c->digits_negative = false;
	}

	int BigInt_Impl::cmp(const BigInt_Impl *b) const
	{

		if(digits_negative == b->digits_negative)
		{
			int  mag;

			if((mag = internal_cmp(b)) == 0)
			return 0;

			if(digits_negative == false)
				return mag;
			else
				return -mag;

		}
		else if(digits_negative == false)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	void BigInt_Impl::internal_reduce(const BigInt_Impl *m, BigInt_Impl *mu)
	{
		// Compute Barrett reduction, x (mod m), given a precomputed value for
		// mu = b^2k / m, where b = RADIX and k = #digits(m).  This should be
		// faster than straight division, when many reductions by the same
		// value of m are required (such as in modular exponentiation).  This
		// can nearly halve the time required to do modular exponentiation,
		// as compared to using the full integer divide to reduce.

		// This algorithm was derived from the _Handbook of Applied
		// Cryptography_ by Menezes, Oorschot and VanStone, Ch. 14,
		// pp. 603-604.

		unsigned int  um = m->digits_used;

		BigInt_Impl   q(*this);

		q.internal_rshd(um - 1);       // q1 = x / b^(k-1)
		q.internal_mul(mu);            // q2 = q1 * mu
		q.internal_rshd(um + 1);       // q3 = q2 / b^(k+1)

		// x = x mod b^(k+1), quick (no division)
		internal_mod_2d(num_bits_in_digit * (um + 1));

		// q = q * m mod b^(k+1), quick (no division)
		q.internal_mul(m);

		q.internal_mod_2d(num_bits_in_digit * (um + 1));

		// x = x - q
		sub(&q, this);

		// If x < 0, add b^(k+1) to it
		if(cmp_z() < 0)
		{
			q.set(1);
			q.internal_lshd(um + 1);
			add(&q, this);
		}

		// Back off if it's too big
		while(cmp(m) >= 0)
		{
			internal_sub(m);
		}
	}

	void BigInt_Impl::internal_sqr()
	{
		// Computes the square of a, in place.  This can be done more
		// efficiently than a general multiplication, because many of the
		// computation steps are redundant when squaring.  The inner product
		// step is a bit more complicated, but we save a fair number of
		// iterations of the multiplication loop.

		uint64_t  w, k = 0;
		unsigned int  ix, jx, kx, used = digits_used;
		uint32_t *pa1, *pa2, *pt, *pbt;

		BigInt_Impl tmp_impl( 2 * used);

		// Left-pad with zeroes
		tmp_impl.digits_used = 2 * used;

		// We need the base value each time through the loop
		pbt = tmp_impl.digits;

		pa1 = digits;
		for(ix = 0; ix < used; ++ix, ++pa1)
		{
			if(*pa1 == 0)
				continue;

			w = (uint64_t) tmp_impl.digits[ix + ix] + ( (uint64_t) *pa1 * (uint64_t) *pa1);

			pbt[ix + ix] = internal_accum(w);
			k = internal_carryout(w);


			// The inner product is computed as:
			// (C, S) = t[i,j] + 2 a[i] a[j] + C
			// This can overflow what can be represented in an mp_word, and
			// since C arithmetic does not provide any way to check for
			// overflow, we have to check explicitly for overflow conditions
			// before they happen.

			for(jx = ix + 1, pa2 = digits + jx; jx < used; ++jx, ++pa2)
			{
				uint64_t  u = 0, v;

				// Store this in a temporary to avoid indirections later
				pt = pbt + ix + jx;

				// Compute the multiplicative step
				w = (uint64_t) *pa1 * (uint64_t) *pa2;

				// If w is more than half MP_WORD_MAX, the doubling will
				// overflow, and we need to record a carry out into the next
				// word
				u = (w >> (num_bits_in_word - 1)) & 1;

				// Double what we've got, overflow will be ignored as defined
				// for C arithmetic (we've already noted if it is to occur)
				w *= 2;

				// Compute the additive step
				v = (uint64_t) *pt + k;

				// If we do not already have an overflow carry, check to see
				// if the addition will cause one, and set the carry out if so
				u |= ((word_maximim_value - v) < w);

				// Add in the rest, again ignoring overflow
				w += v;

				// Set the i,j digit of the output
				*pt = internal_accum(w);

				// Save carry information for the next iteration of the loop.
				// This is why k must be an mp_word, instead of an mp_digit
				k = internal_carryout(w) | ( (uint64_t) u << num_bits_in_digit);

			}

			// Set the last digit in the cycle and reset the carry
			k = (uint64_t) tmp_impl.digits[ix + jx] + k;
			pbt[ix + jx] = internal_accum(k);
			k = internal_carryout(k);

			// If we are carrying out, propagate the carry to the next digit
			// in the output.  This may cascade, so we have to be somewhat
			// circumspect -- but we will have enough precision in the output
			// that we won't overflow

			kx = 1;
			while(k)
			{
				k = (uint64_t) pbt[ix + jx + kx] + 1;
				pbt[ix + jx + kx] = internal_accum(k);
				k = internal_carryout(k);
				++kx;
			}
		}

		tmp_impl.internal_clamp();
		tmp_impl.internal_exch(this);
	}

	void BigInt_Impl::exptmod(const BigInt_Impl *b, const BigInt_Impl *m, BigInt_Impl *c) const
	{
		BigInt_Impl s, mu;
		uint32_t d;
		const uint32_t *db = b->digits;

		unsigned int  ub = b->digits_used;
		unsigned int dig, bit;

		if (b->cmp_z() < 0 || m->cmp_z() <= 0)
			throw Exception("Divide by zero");

		BigInt_Impl x(*this);

		x.mod(m, &x);

		s.set(1);

		// mu = b^2k / m
		mu.internal_add_d(1);
		mu.internal_lshd(2 * m->digits_used);
		mu.div(m, &mu, nullptr);

		// Loop over digits of b in ascending order, except highest order
		for(dig = 0; dig < (ub - 1); dig++)
		{
			d = *db++;

			// Loop over the bits of the lower-order digits
			for(bit = 0; bit < num_bits_in_digit; bit++)
			{
				if(d & 1)
				{
					s.internal_mul(&x);
					s.internal_reduce(m, &mu);
				}

				d >>= 1;
				x.internal_sqr();
				x.internal_reduce(m, &mu);
			}
		}

		// Now do the last digit...
		d = *db;

		while(d)
		{
			if(d & 1)
			{
				s.internal_mul(&x);
				s.internal_reduce(m, &mu);
			}

			d >>= 1;

			x.internal_sqr();
			x.internal_reduce(m, &mu);
		}

		s.internal_exch(c);
	}

	bool BigInt_Impl::fermat(uint32_t w) const
	{
		BigInt_Impl  base, test;

		base.set(w);

		// Compute test = base^a (mod a)
		base.exptmod(this, this, &test);

		if(base.cmp(&test) == 0)
			return true;
		return false;
	}


	int BigInt_Impl::cmp_d(uint32_t d) const
	{
		if(digits_negative == true)
			return -1;

		return internal_cmp_d(d);
	}

	void BigInt_Impl::neg(BigInt_Impl *b) const
	{
		copy(b);

		if(b->internal_cmp_d(0) == 0)
			b->digits_negative = false;
		else
			b->digits_negative = (b->digits_negative == true) ? false : true;
	}


	void BigInt_Impl::sub_d(uint32_t d, BigInt_Impl *b) const
	{
		copy(b);

		if(b->digits_negative == true)
		{
			b->internal_add_d(d);
		}
		else if(b->internal_cmp_d(d) >= 0)
		{
			b->internal_sub_d(d);
		}
		else
		{
			b->neg(b);
			b->digits[0] = d - b->digits[0];
			b->digits_negative = true;
		}

		if(b->internal_cmp_d(0) == 0)
			b->digits_negative = false;
	}

	unsigned int BigInt_Impl::trailing_zeros() const
	{
		uint32_t d = 0;
		unsigned int  n = 0;
		unsigned int ix;

		if (!cmp_z())
			return 0;

		for (ix = 0; ix < digits_used; ++ix)
		{
			d = digits[ix];
			if (d)
				break;

			n += num_bits_in_digit;
		}

		if (!d)
		return 0;	// shouldn't happen, but ...

		if (sizeof(uint32_t) > 4)
		{
			if (!(d & 0xffffffffU))
			{
				d >>= 32;
				n  += 32;
			}
		}

		if (sizeof(uint32_t) > 2)
		{
			if (!(d & 0xffffU))
			{
				d >>= 16;
				n  += 16;
			}
		}

		if (!(d & 0xffU))
		{
			d >>= 8;
			n  += 8;
		}
		if (!(d & 0xfU))
		{
			d >>= 4;
			n  += 4;
		}
		if (!(d & 0x3U))
		{
			d >>= 2;
			n  += 2;
		}
		if (!(d & 0x1U))
		{
			d >>= 1;
			n  += 1;
		}

		return n;
	}

	void BigInt_Impl::sqr(BigInt_Impl *b) const
	{
		copy(b);
		b->internal_sqr();
		b->digits_negative = false;
	}

	void BigInt_Impl::sqrmod(const BigInt_Impl *m, BigInt_Impl *c) const
	{
		sqr(c);
		c->mod(m, c);
	}

	void BigInt_Impl::random()
	{
		uint32_t  next = 0;
		unsigned int       ix, jx;

		for(ix = 0; ix < digits_used; ix++)
		{
			for(jx = 0; jx < sizeof(uint32_t); jx++)
			{
				next = (next << 8) | (rand() & 0xff);
			}
			digits[ix] = next;
		}
	}

	bool BigInt_Impl::pprime(int nt) const
	{
		// Using NSS algorithm, MPI original version is a bit confusing. I an see why it was rewritten in NSS
		BigInt_Impl m, z;
		int iter;
		unsigned int jx;
		unsigned int b;

		BigInt_Impl amo(*this);	// "amo" = "a minus one"
		BigInt_Impl x(*this);

		// Compute amo = a - 1 for what follows...
		sub_d(1, &amo);

		b = amo.trailing_zeros();

		if (!b)		// a was even ?
		{
			return false;
		}

		amo.div_2d(b, &m, nullptr);

		bool result = true;

		// Do the test nt times...
		for(iter = 0; iter < nt; iter++)
		{
			// Choose a random value for 1 < x < a
			x.internal_pad(digits_used);
			x.random();
			x.mod(this, &x);
			if(x.cmp_d(1) <= 0)
			{
				iter--;    // don't count this iteration
				continue;  // choose a new x
			}

			// Compute z = (x ** m) mod a
			x.exptmod(&m, this, &z);

			if(z.cmp_d(1) == 0 || z.cmp(&amo) == 0)
			{
				result = true;
				continue;
			}

			result = false;  // just in case the following for loop never executes.
			for (jx = 1; jx < b; jx++)
			{
				// z = z^2 (mod a)
				z.sqrmod(this, &z);

				if(z.cmp_d(1) == 0)
				{
					break;
				}
				if(z.cmp(&amo) == 0)
				{
					result = true;
					break;
				}
			}

			// If the test passes, we will continue iterating, but a failed
			// test means the candidate is definitely NOT prime, so we will
			// immediately break out of this loop
			if (result == false)
				break;
		}
		return result;
	}

	void BigInt_Impl::mul(const BigInt_Impl *b, BigInt_Impl *c) const
	{
		bool  is_negative;

		is_negative = (digits_negative == b->digits_negative) ? false : true;

		if(c == b)
		{
			c->internal_mul(this);
		}
		else
		{
			copy(c);
			c->internal_mul(b);
		}

		if(is_negative == false || c->internal_cmp_d(0) == 0)
			c->digits_negative = false;
		else
			c->digits_negative = is_negative;

	}

	void BigInt_Impl::mul_d(uint32_t d, BigInt_Impl *c) const
	{
		copy(c);

		bool  is_negative = digits_negative;

		c->internal_mul_d(d);

		if(is_negative == false || c->internal_cmp_d(0) == 0)
			c->digits_negative = false;
		else
			c->digits_negative = is_negative;
	}

	bool BigInt_Impl::invmod(const BigInt_Impl *m, BigInt_Impl *c) const
	{
		bool  is_negative;

		if (cmp_z() == 0 || m->cmp_z() == 0)
			throw Exception("Divide by zero");

		is_negative = digits_negative;

		BigInt_Impl  g, x;

		xgcd( m, &g, &x, nullptr);

		if(g.cmp_d(1) != 0)
		{
			return false;	// Undefined
		}

		x.mod(m, c);
		c->digits_negative = is_negative;
		return true;

	}

	void BigInt_Impl::abs(BigInt_Impl *b) const
	{
		copy(b);
		b->digits_negative = false;
	}

	int BigInt_Impl::iseven() const
	{
		return !isodd();
	}

	int BigInt_Impl::isodd() const
	{
	  return (digits[0] & 1);
	}

	void BigInt_Impl::div_2(BigInt_Impl *c) const
	{
		copy(c);
		c->internal_div_2();
	}

	void BigInt_Impl::internal_div_2()
	{
		// Divide by two -- take advantage of radix properties to do it fast
		internal_div_2d(1);
	}

	void BigInt_Impl::internal_mul_2()
	{
		unsigned int      ix;
		uint32_t kin = 0, kout, *dp = digits;

		// Shift digits leftward by 1 bit
		for(ix = 0; ix < digits_used; ix++)
		{
			kout = (dp[ix] >> (num_bits_in_digit - 1)) & 1;
			dp[ix] = (dp[ix] << 1) | kin;

			kin = kout;
		}

		// Deal with rollover from last digit
		if(kin)
		{
			if(ix >= digits_alloc)
			{
				internal_grow(digits_alloc + 1);
				dp = digits;
			}

			dp[ix] = kin;
			digits_used += 1;
		}

	}

	void BigInt_Impl::xgcd(const BigInt_Impl *b, BigInt_Impl *g, BigInt_Impl *x, BigInt_Impl *y) const
	{
		BigInt_Impl   gx, A, B, C, D;

		if(b->cmp_z() == 0)
			throw Exception("Divide by zero");

		BigInt_Impl xc(*this);
		xc.abs(&xc);

		BigInt_Impl yc(*b);
		yc.abs(&yc);

		gx.set(1);

		// Divide by two until at least one of them is even
		while(xc.iseven() && yc.iseven())
		{
			xc.internal_div_2();
			yc.internal_div_2();
			gx.internal_mul_2();
		}
		BigInt_Impl u(xc);
		BigInt_Impl v(yc);

		A.set(1);
		D.set(1);

		// Loop through binary GCD algorithm
		for(;;)
		{
			while(u.iseven())
			{
				u.internal_div_2();

				if(A.iseven() && B.iseven())
				{
					A.internal_div_2();
					B.internal_div_2();
				}
				else
				{
					A.add(&yc, &A);
					A.internal_div_2();
					B.sub(&xc, &B);
					B.internal_div_2();
				}
			}

			while(v.iseven())
			{
				v.internal_div_2();

				if(C.iseven() && D.iseven())
				{
					C.internal_div_2();
					D.internal_div_2();
				}
				else
				{
					C.add(&yc, &C);
					C.internal_div_2();
					D.sub(&xc, &D);
					D.internal_div_2();
				}
			}

			if(u.cmp(&v) >= 0)
			{
				u.sub(&v, &u);
				A.sub(&C, &A);
				B.sub(&D, &B);
			}
			else
			{
				v.sub(&u, &v);
				C.sub(&A, &C);
				D.sub(&B, &D);
			}

			// If we're done, copy results to output
			if(u.cmp_z() == 0)
			{
				if(x)
					C.copy(x);

				if(y)
					D.copy(y);

				if(g)
					gx.mul(&v, g);

				break;
			}
		}

	}


	void BigInt_Impl::exch(BigInt_Impl *mp2)
	{
		internal_exch(mp2);
	}

	bool BigInt_Impl::make_prime(unsigned int num_bits)
	{
		// Algorithm from Mozilla NSS

		const int sieve_size = 32*1024;
		std::vector<unsigned char> work_sieve;
		work_sieve.resize(sieve_size);

		BigInt_Impl trial_impl(default_allocated_precision);

		unsigned int num_tests;

		// Values taken from table 4.4, HandBook of Applied Cryptography
		if (num_bits >= 1300) {
			num_tests = 2;
		} else if (num_bits >= 850) {
			num_tests = 3;
		} else if (num_bits >= 650) {
			num_tests = 4;
		} else if (num_bits >= 550) {
			num_tests = 5;
		} else if (num_bits >= 450) {
			num_tests = 6;
		} else if (num_bits >= 400) {
			num_tests = 7;
		} else if (num_bits >= 350) {
			num_tests = 8;
		} else if (num_bits >= 300) {
			num_tests = 9;
		} else if (num_bits >= 250) {
			num_tests = 12;
		} else if (num_bits >= 200) {
			num_tests = 15;
		} else if (num_bits >= 150) {
			num_tests = 18;
		} else if (num_bits >= 100) {
			num_tests = 27;
		} else
			num_tests = 50;

		set_bit(num_bits - 1, 1);
		set_bit(0, 1);

		for (unsigned int i = significant_bits() - 1; i >= num_bits; --i)
		{
			set_bit(i, 0);
		}

		// Start sieveing with prime value of 3.
		BigInt_Impl::build_primes();		// Build prime numbers if they do not already exist
		sieve(&BigInt_Impl::prime_tab[1], BigInt_Impl::prime_tab_size - 1, work_sieve);

		unsigned char *sieve_ptr = &work_sieve[0];

		bool found_prime = false;
		unsigned int i;
		// Loop through sieved values
		for(i = 0; i < sieve_size; ++i)
		{
			if (sieve_ptr[i])	// this number is composite
				continue;

			add_d(2 * i, &trial_impl);

			// Run a Fermat test
			if (!trial_impl.fermat(2))
				continue;	// was composite

			// If that passed, run some Miller-Rabin tests
			if (!trial_impl.pprime(num_tests))
				continue;	// was composite

			// Note, we can test q = 2p + 1. For an unknown reason, by default, NSS does not do this. This should be investigated
			found_prime = true;
			break;
		}
		if (!found_prime)
			return false;

		internal_exch(&trial_impl);

		memset(&work_sieve[0], 0, work_sieve.size());

		return true;
	}

	int BigInt_Impl::unsigned_octet_size() const
	{
		int  bytes;
		int  ix;
		uint32_t  d = 0;

		if (digits_negative)
			throw Exception("Digits negative");

		bytes = (digits_used * sizeof(uint32_t));

		// subtract leading zeros.
		// Iterate over each digit...
		for(ix = digits_used - 1; ix >= 0; ix--)
		{
			d = digits[ix];
			if (d)
				break;
			bytes -= sizeof(d);
		}
		if (!bytes)
			return 1;

		// Have MSD, check digit bytes, high order first
		for(ix = sizeof(uint32_t) - 1; ix >= 0; ix--)
		{
			unsigned char x = (unsigned char)(d >> (ix * 8));
			if (x)
				break;
			--bytes;
		}
		return bytes;
	}

	void BigInt_Impl::to_unsigned_octets(unsigned char *output_str, unsigned int output_length) const
	{
		int  ix, pos = 0;
		int  bytes;

		bytes = unsigned_octet_size();
		if (bytes > output_length)
			throw Exception("output size mismatch");

		if (bytes < output_length)
		{
			// We insert leading zeros here
			unsigned int padding = output_length - bytes;
			memset(output_str, 0, padding);
			output_str += padding;
		}


		// Iterate over each digit...
		for(ix = digits_used - 1; ix >= 0; ix--)
		{
			uint32_t  d = digits[ix];
			int       jx;

			// Unpack digit bytes, high order first
			for(jx = sizeof(uint32_t) - 1; jx >= 0; jx--)
			{
				unsigned char x = (unsigned char)(d >> (jx * 8));
				if (!pos && !x)	// suppress leading zeros
					continue;
				*(output_str++) = x;
				pos++;
			}
		}
		if (!pos)
			*output_str = 0;
	}

	void BigInt_Impl::build_primes()
	{
		if (!prime_tab.empty())
			return;

		prime_tab.resize(prime_tab_size);

		// Perform the Sieve of Eratosthenes on the first 65536 integers
		const int sieve_max = 65536;
		const int sieve_size = sieve_max / 2;	// Exclude even numbers
		const int half_sieve_max = sieve_max / 2;

		std::vector<unsigned char> sieve;
		sieve.resize(sieve_size);
		unsigned char *sieve_ptr = &sieve[0];
		memset(sieve_ptr, 0, sieve_size * sizeof(unsigned char));

		for (int prime_number = 3; prime_number < half_sieve_max; prime_number += 2)
		{
			if (sieve_ptr[prime_number / 2])
				continue;

			for (int inner_count = prime_number * 3; inner_count < sieve_max; inner_count+= (prime_number*2))
			{
				sieve_ptr[inner_count / 2] = 1;
			}
		}

		// Store the sieve
		int prime_offset = 1;
		prime_tab[0] = 2;	// First prime is 2

		for (int cnt  = 1; cnt < sieve_size; cnt++)
		{
			if (!sieve_ptr[cnt])
			{
				if (prime_offset >= prime_tab_size)
					throw Exception("Build primes unexpected result");

				BigInt_Impl::prime_tab[prime_offset] = (cnt * 2) + 1;
				prime_offset++;
			}
		}
		if (prime_offset != prime_tab_size)
				throw Exception("Build primes unexpected result");
	}

	void BigInt_Impl::get(uint32_t &d)
	{
		if (digits_used == 0)
		{
			d = 0;
			return;
		}
		if (digits_used > 1)
				throw Exception("Number greater than 32bit");

		if (digits_negative)
				throw Exception("Number is negative");

		d = digits[0];
	}

	void BigInt_Impl::get(int32_t &d)
	{
		if (digits_used == 0)
		{
			d = 0;
			return;
		}
		if (digits_used > 1)
				throw Exception("Number greater than 32bit");

		d = digits[0];
		if (d & (1<<(num_bits_in_digit-1)))
			throw Exception("Number is too large");

		if (digits_negative)
			d = -d;

	}

	void BigInt_Impl::get(uint64_t &d)
	{
		if (digits_used == 0)
		{
			d = 0;
			return;
		}
		if (digits_used > 2)
				throw Exception("Number greater than 64bit");

		if (digits_negative)
				throw Exception("Number is negative");

		d = digits[0];
		if (digits_used == 2)
		{
			uint64_t d2 = digits[1];
			d |= d2 << 32;
		}
	}

	void BigInt_Impl::get(int64_t &d)
	{
		if (digits_used == 0)
		{
			d = 0;
			return;
		}
		if (digits_used > 2)
				throw Exception("Number greater than 64bit");

		d = digits[0];
		if (digits_used == 2)
		{
			int64_t d2= digits[1];
			if (d2 & (1<<(num_bits_in_digit-1)))
				throw Exception("Number is too large");
			d |= d2 << 32LL;
		}
		if (digits_negative)
			d = -d;
	}
}
