
void linear5(Scanline *d)
{
	unsigned int *dest = d->dest;
	unsigned int *texdata = d->src;
	unsigned int texwidth = d->src_width;
	unsigned int texheight = d->src_height;

	float startoffset = d->startx+0.5f-d->x1;
	float deltax = d->x2-d->x1;
	float deltatx = d->tx2-d->tx1;
	float deltaty = d->ty2-d->ty1;

	int dxx = (int)(deltatx/deltax*32768);
	int dyy = (int)(deltaty/deltax*32768);
	int xx = (int)(d->tx1*32768+startoffset*dxx);
	int yy = (int)(d->ty1*32768+startoffset*dyy);

	xx *= texwidth;
	dxx *= texwidth;
	yy *= texheight;
	dyy *= texheight;

	__m128i zero = _mm_setzero_si128();
	__m128i half = _mm_set1_epi16(0x0040);
	__m128i texwidthmax = _mm_set1_epi32(texwidth-1);
	__m128i texheightmax = _mm_set1_epi32(texheight-1);

	__m128i one = _mm_set1_epi16(0x0100);
	__m128i half7f = _mm_set1_epi16(0x007f);

	for (int x = d->startx; x < d->endx; x+=4)
	{
		__declspec(align(16)) int xx_input[4];
		__declspec(align(16)) int yy_input[4];
		for (int i = 0; i < 4; i++)
		{
			xx_input[i] = xx;
			yy_input[i] = yy;
			xx += dxx;
			yy += dyy;
		}

		__m128i tx = _mm_load_si128((__m128i*)xx_input);
		__m128i ty = _mm_load_si128((__m128i*)yy_input);

		__m128i a = _mm_and_si128(tx, _mm_set1_epi32(0x7fff));
		__m128i b = _mm_and_si128(ty, _mm_set1_epi32(0x7fff));
		__m128i inv_a = _mm_sub_epi32(_mm_set1_epi32(0x8000), a);
		__m128i inv_b = _mm_sub_epi32(_mm_set1_epi32(0x8000), b);
		__m128i c1 = _mm_srli_epi32(_mm_mulhi_epi16(a, inv_b), 7);
		__m128i c2 = _mm_srli_epi32(_mm_mulhi_epi16(inv_a, b), 7);
		__m128i c3 = _mm_srli_epi32(_mm_mulhi_epi16(a, b), 7);
		__m128i c0 = _mm_sub_epi32(_mm_sub_epi32(_mm_sub_epi32(_mm_set1_epi32(128), c1), c2), c3);

		__declspec(align(16)) int ic0[4], ic1[4], ic2[4], ic3[4];
		_mm_store_si128((__m128i*)ic0, c0);
		_mm_store_si128((__m128i*)ic1, c1);
		_mm_store_si128((__m128i*)ic2, c2);
		_mm_store_si128((__m128i*)ic3, c3);

		__m128i tx0 = _mm_srli_epi32(tx, 15);
		__m128i ty0 = _mm_srli_epi32(ty, 15);
		__m128i tx1 = _mm_add_epi32(tx0, _mm_set1_epi32(1));
		__m128i ty1 = _mm_add_epi32(ty0, _mm_set1_epi32(1));

		// wrapClampToEdge:
		tx0 = _mm_max_epi16(tx0, zero);
		ty0 = _mm_max_epi16(ty0, zero);
		tx1 = _mm_max_epi16(tx1, zero);
		ty1 = _mm_max_epi16(ty1, zero);
		tx0 = _mm_min_epi16(tx0, texwidthmax);
		ty0 = _mm_min_epi16(ty0, texheightmax);
		tx1 = _mm_min_epi16(tx1, texwidthmax);
		ty1 = _mm_min_epi16(ty1, texheightmax);

		__declspec(align(16)) int itx0[4], ity0[4], itx1[4], ity1[4];
		_mm_store_si128((__m128i*)itx0, tx0);
		_mm_store_si128((__m128i*)ity0, ty0);
		_mm_store_si128((__m128i*)itx1, tx1);
		_mm_store_si128((__m128i*)ity1, ty1);

		for (int i = 0; i < 4; i++)
		{
			__m128i p0, p1, p2, p3, tmp0, tmp1, result;
			p0 = _mm_cvtsi32_si128(texdata[itx0[i]+ity0[i]*texwidth]);
			tmp0 = _mm_unpacklo_epi8(p0, zero);
			tmp1 = _mm_set1_epi16(ic0[i]);
			result = _mm_mullo_epi16(tmp0, tmp1);

			p1 = _mm_cvtsi32_si128(texdata[itx1[i]+ity0[i]*texwidth]);
			tmp0 = _mm_unpacklo_epi8(p1, zero);
			tmp1 = _mm_set1_epi16(ic1[i]);
			result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

			p2 = _mm_cvtsi32_si128(texdata[itx0[i]+ity1[i]*texwidth]);
			tmp0 = _mm_unpacklo_epi8(p2, zero);
			tmp1 = _mm_set1_epi16(ic2[i]);
			result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

			p3 = _mm_cvtsi32_si128(texdata[itx1[i]+ity1[i]*texwidth]);
			tmp0 = _mm_unpacklo_epi8(p3, zero);
			tmp1 = _mm_set1_epi16(ic3[i]);
			result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

			result = _mm_add_epi16(result, half);
			result = _mm_srli_epi16(result, 7);
			result = _mm_packus_epi16(result, zero);

			__m128i src0, dest0;
			__m128i src_alpha, invsrc_alpha;
			src0 = result;
			dest0 = _mm_cvtsi32_si128(dest[x+i]);

			src0 = _mm_unpacklo_epi8(src0, zero);
			dest0 = _mm_unpacklo_epi8(dest0, zero);
			src_alpha = _mm_shufflelo_epi16(src0, _MM_SHUFFLE(3,3,3,3));
			src_alpha = _mm_shufflehi_epi16(src_alpha, _MM_SHUFFLE(3,3,3,3));
			invsrc_alpha = _mm_sub_epi16(one, src_alpha);
			dest0 = _mm_mullo_epi16(dest0, invsrc_alpha);
			dest0 = _mm_add_epi16(dest0, half7f);
			dest0 = _mm_srli_epi16(dest0, 8);
			dest0 = _mm_add_epi16(dest0, src0);
			dest0 = _mm_packus_epi16(dest0, zero);
			dest[x+i] = _mm_cvtsi128_si32(dest0);
		}
	}
}
