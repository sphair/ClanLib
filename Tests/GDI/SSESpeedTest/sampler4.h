
inline void sampleLinearRepeat4(int xx0, int yy0, int xx1, int yy1, int texwidth, __m128i &clampmax, unsigned int *texdata, __m128i &pixel)
{
	__m128i tmp0, tmp1, pos, frac, inv_frac, c3120;
	tmp0 = _mm_set_epi32(yy1, xx1, yy0, xx0);
	pos = _mm_srai_epi32(tmp0, 15);

	frac = _mm_and_si128(tmp0, _mm_set1_epi32(0x7fff));
	inv_frac = _mm_sub_epi32(_mm_set1_epi32(0x8000), frac);

	tmp0 = _mm_packs_epi32(inv_frac, frac);                 // tmp0 = [ib1 ia1  ib0 ia0  b1 a1  b0  a0 ]
	tmp0 = _mm_shuffle_epi32(tmp0, _MM_SHUFFLE(3,1,2,0));   // tmp0 = [ib1 ia1  b1  a1   b0 a0  ib0 ia0]
	tmp1 = _mm_shufflehi_epi16(tmp0, _MM_SHUFFLE(2,0,3,1));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(2,0,3,1)); // tmp1 = [a1  ia1  b1  ib1  a0 ia0 b0  ib0]

	c3120 = _mm_mulhi_epi16(tmp0, tmp1);
	c3120 = _mm_srli_epi16(c3120, 7);

	unsigned short c[8];
	_mm_storeu_si128((__m128i*)c, c3120);
	c[0] = 128-c[1]-c[2]-c[3];
	c[4] = 128-c[5]-c[6]-c[7];
	c3120 = _mm_loadu_si128((__m128i*)c);

	// wrapClampToEdge:

	pos = _mm_packs_epi32(pos, pos);
	pos = _mm_add_epi16(pos, _mm_set_epi16(1, 1, 1, 1, 0, 0, 0, 0));
	pos = _mm_max_epi16(pos, _mm_setzero_si128());
	pos = _mm_min_epi16(pos, clampmax);
	short texpos[8]; // x0, y0, x1, y1, x0+1, y0+1, x1+1, y1+1
	_mm_storeu_si128((__m128i*)texpos, pos);

	int tx0 = texpos[0];
	int ty0 = texpos[1];
	int tx2 = texpos[2];
	int ty2 = texpos[3];
	int tx1 = texpos[4];
	int ty1 = texpos[5];
	int tx3 = texpos[6];
	int ty3 = texpos[7];

	tmp0 = _mm_set_epi32(0, 0, texdata[tx2+ty2*texwidth], texdata[tx0+ty0*texwidth]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c3120, _MM_SHUFFLE(0,0,0,0));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(0,0,0,0));
	pixel = _mm_mullo_epi16(tmp0, tmp1);

	tmp0 = _mm_set_epi32(0, 0, texdata[tx3+ty2*texwidth], texdata[tx1+ty0*texwidth]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c3120, _MM_SHUFFLE(2,2,2,2));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(2,2,2,2));
	pixel = _mm_add_epi16(pixel, _mm_mullo_epi16(tmp0, tmp1));

	tmp0 = _mm_set_epi32(0, 0, texdata[tx2+ty3*texwidth], texdata[tx0+ty1*texwidth]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c3120, _MM_SHUFFLE(1,1,1,1));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
	pixel = _mm_add_epi16(pixel, _mm_mullo_epi16(tmp0, tmp1));

	tmp0 = _mm_set_epi32(0, 0, texdata[tx3+ty3*texwidth], texdata[tx1+ty1*texwidth]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c3120, _MM_SHUFFLE(3,3,3,3));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
	pixel = _mm_add_epi16(pixel, _mm_mullo_epi16(tmp0, tmp1));

	pixel = _mm_add_epi16(pixel, _mm_set1_epi16(0x0040));
	pixel = _mm_srli_epi16(pixel, 7);
	pixel = _mm_packus_epi16(pixel, _mm_setzero_si128());


#ifdef JUNK
	__m128i tmp0, tmp1, pos, frac, inv_frac, c0213;
	tmp0 = _mm_set_epi32(xx1, yy1, xx0, yy0);

	pos = _mm_srai_epi32(tmp0, 15);
	frac = _mm_and_si128(_mm_srai_epi32(tmp0, 8), _mm_set1_epi32(0x7f));
	inv_frac = _mm_sub_epi32(_mm_set1_epi32(128), frac);

	// frac =     [000000aa 000000bb  000000aa  000000bb]
	// inv_frac = [000000ia 000000ib  000000ia  000000ib]
	// tmp0 =     [00aa 00bb 00aa 00bb  00ia 00ib 00ia 00ib]
	// tmp0 =     [00aa 00bb 00ia 00ib  00aa 00bb 00ia 00ib]
	// tmp1 =     [00bb 00ib 00aa 00ia  00bb 00ib 00aa 00ia]
	// c0213 =    [ c3   c1   c2   c0    c3   c1   c2   c0 ]
	//
	// int c0 = (inv_a*inv_b+64)>>7;
	// int c1 = (a*inv_b+64)>>7;
	// int c2 = (inv_a*b)>>7;
	// int c3 = (a*b)>>7;

	tmp0 = _mm_packs_epi32(inv_frac, frac);
	tmp0 = _mm_shuffle_epi32(tmp0, _MM_SHUFFLE(3,1,2,0));
	tmp1 = _mm_shufflehi_epi16(tmp0, _MM_SHUFFLE(2,0,3,1));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(2,0,3,1));
	c0213 = _mm_mullo_epi16(tmp0, tmp1);
	c0213 = _mm_add_epi16(c0213, _mm_set_epi16(0, 64, 0, 64, 0, 64, 0, 64));
	c0213 = _mm_srli_epi16(c0213, 7);
	//c0213 = _mm_set_epi16(0, 0, 0, 128, 0, 0, 0, 128);

	// wrapClampToEdge:

	pos = _mm_packs_epi32(pos, pos);
	pos = _mm_add_epi16(pos, _mm_set_epi16(1, 1, 1, 1, 0, 0, 0, 0));
	pos = _mm_max_epi16(pos, _mm_setzero_si128());
	pos = _mm_min_epi16(pos, clampmax);
	short texpos[8]; // x0, y0, x1, y1, x0+1, y0+1, x1+1, y1+1
	_mm_storeu_si128((__m128i*)texpos, pos);

	int tx0 = texpos[1];
	int ty0 = texpos[0];
	int tx2 = texpos[3];
	int ty2 = texpos[2];
	int tx1 = texpos[5];
	int ty1 = texpos[4];
	int tx3 = texpos[7];
	int ty3 = texpos[6];
/*
	short constants[8];
	_mm_storeu_si128((__m128i*)constants, c0213);

	__m128i zero, half, p0, p1, p2, p3, result;
	zero = _mm_setzero_si128();
	half = _mm_set1_epi16(0x003f);
	p0 = _mm_set_epi32(0, 0, texdata[tx2+ty2*texwidth], texdata[tx0+ty0*texwidth]);

	tmp0 = _mm_unpacklo_epi8(p0, zero);
	tmp1 = _mm_set_epi16(constants[0], constants[0], constants[0], constants[0], constants[4], constants[4], constants[4], constants[4]);
	result = _mm_mullo_epi16(tmp0, tmp1);

	p1 = _mm_set_epi32(0, 0, texdata[tx3+ty2*texwidth], texdata[tx1+ty0*texwidth]);
	tmp0 = _mm_unpacklo_epi8(p1, zero);
	tmp1 = _mm_set_epi16(constants[2], constants[2], constants[2], constants[2], constants[6], constants[6], constants[6], constants[6]);
	result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

	p2 = _mm_set_epi32(0, 0, texdata[tx2+ty3*texwidth], texdata[tx0+ty1*texwidth]);
	tmp0 = _mm_unpacklo_epi8(p2, zero);
	tmp1 = _mm_set_epi16(constants[1], constants[1], constants[1], constants[1], constants[5], constants[5], constants[5], constants[5]);
	result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

	p3 = _mm_set_epi32(0, 0, texdata[tx3+ty3*texwidth], texdata[tx1+ty1*texwidth]);
	tmp0 = _mm_unpacklo_epi8(p3, zero);
	tmp1 = _mm_set_epi16(constants[3], constants[3], constants[3], constants[3], constants[7], constants[7], constants[7], constants[7]);
	result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));
	
	result = _mm_add_epi16(result, half);
	result = _mm_srli_epi16(result, 7);
	pixel = _mm_packus_epi16(result, zero);
*/

	tmp0 = _mm_set_epi32(0, 0, texdata[tx2+ty2*texwidth], texdata[tx0+ty0*texwidth]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c0213, _MM_SHUFFLE(0,0,0,0));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(0,0,0,0));
	pixel = _mm_mullo_epi16(tmp0, tmp1);

	tmp0 = _mm_set_epi32(0, 0, texdata[tx3+ty2*texwidth], texdata[tx1+ty0*texwidth]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c0213, _MM_SHUFFLE(2,2,2,2));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(2,2,2,2));
	pixel = _mm_add_epi16(pixel, _mm_mullo_epi16(tmp0, tmp1));

	tmp0 = _mm_set_epi32(0, 0, texdata[tx2+ty3*texwidth], texdata[tx0+ty1*texwidth]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c0213, _MM_SHUFFLE(1,1,1,1));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
	pixel = _mm_add_epi16(pixel, _mm_mullo_epi16(tmp0, tmp1));

	tmp0 = _mm_set_epi32(0, 0, texdata[tx3+ty3*texwidth], texdata[tx1+ty1*texwidth]);
	tmp0 = _mm_unpacklo_epi8(tmp0, _mm_setzero_si128());
	tmp1 = _mm_shufflehi_epi16(c0213, _MM_SHUFFLE(3,3,3,3));
	tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
	pixel = _mm_add_epi16(pixel, _mm_mullo_epi16(tmp0, tmp1));

	pixel = _mm_add_epi16(pixel, _mm_set1_epi16(0x0040));
	pixel = _mm_srli_epi16(pixel, 7);
	pixel = _mm_packus_epi16(pixel, _mm_setzero_si128());
#endif
}
