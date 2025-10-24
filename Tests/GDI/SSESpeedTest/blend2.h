
void blend2(Scanline *d)
{
	unsigned int *dest = d->dest;
	unsigned int *src = d->src;

	__m128i one, half, zero;
	one = _mm_set1_epi16(0x0100);
	half = _mm_set1_epi16(0x007f);
	zero = _mm_setzero_si128();

	for (int x = d->startx; x < d->endx; x+=4)
	{
		__m128i src0, dest0, srcwrk, destwrk, tmp;
		src0 = _mm_load_si128((__m128i*)(src+x));
		dest0 = _mm_load_si128((__m128i*)(dest+x));

		srcwrk = _mm_unpacklo_epi8(src0, zero);
		destwrk = _mm_unpacklo_epi8(dest0, zero);
		
		__m128i src_alpha, invsrc_alpha;
		src_alpha = srcwrk;
		src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
		src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);
		invsrc_alpha = _mm_sub_epi16(one, src_alpha);
		destwrk = _mm_mullo_epi16(destwrk, invsrc_alpha);
		destwrk = _mm_add_epi16(destwrk, half);
		destwrk = _mm_srli_epi16(destwrk, 8);
		destwrk = _mm_add_epi16(destwrk, srcwrk);
		srcwrk = _mm_unpacklo_epi8(src0, zero);
		destwrk = _mm_unpacklo_epi8(dest0, zero);
		tmp = _mm_unpackhi_epi8(dest0, zero);
		dest0 = _mm_packus_epi16(destwrk, tmp);

		srcwrk = _mm_unpackhi_epi8(src0, zero);
		destwrk = tmp;
		src_alpha = srcwrk;
		src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
		src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);
		invsrc_alpha = _mm_sub_epi16(one, src_alpha);
		destwrk = _mm_mullo_epi16(destwrk, invsrc_alpha);
		destwrk = _mm_add_epi16(destwrk, half);
		destwrk = _mm_srli_epi16(destwrk, 8);
		destwrk = _mm_add_epi16(destwrk, srcwrk);
		srcwrk = _mm_unpacklo_epi8(src0, zero);
		destwrk = _mm_unpacklo_epi8(dest0, zero);
		tmp = _mm_unpacklo_epi8(dest0, zero);
		dest0 = _mm_packus_epi16(tmp, destwrk);

		_mm_store_si128((__m128i*)(dest+x), dest0);
	}
}
