
inline unsigned int sampleLinearRepeat2(__m128 &xx, __m128 &yy, int texwidth, int texheight, unsigned int *texdata)
{
//	unsigned int old_rounding_mode = _MM_GET_ROUNDING_MODE();

	__m128 one = _mm_set_ss(128.0f);
	//_MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN);
	int tx = _mm_cvtss_si32(xx);
	int ty = _mm_cvtss_si32(yy);
	//_MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST);
	int a = _mm_cvtss_si32(_mm_mul_ss(_mm_sub_ss(xx, _mm_cvtsi32_ss(xx, tx)), one));
	int b = _mm_cvtss_si32(_mm_mul_ss(_mm_sub_ss(yy, _mm_cvtsi32_ss(yy, ty)), one));
	int inv_a = 128-a;
	int inv_b = 128-b;

	int c0 = (inv_a*inv_b+64)>>7;
	int c1 = (a*inv_b+64)>>7;
	int c2 = (inv_a*b)>>7;
	int c3 = (a*b)>>7;

//	_MM_SET_ROUNDING_MODE(old_rounding_mode);
/*
	// wrapRepeat any size:
	int tx0 = tx%texwidth;//wrapRepeat(tx, texwidth);
	int ty0 = ty%texheight;//wrapRepeat(ty, texheight);
	int tx1 = (tx+1)%texwidth;//wrapRepeat(tx+1, texwidth);
	int ty1 = (ty+1)%texheight;//wrapRepeat(ty+1, texheight);
*/
	// wrapRepeat power of two:
	int tx0 = tx&511;//wrapRepeat(tx, texwidth);
	int ty0 = ty&511;//wrapRepeat(ty, texheight);
	int tx1 = (tx+1)&511;//wrapRepeat(tx+1, texwidth);
	int ty1 = (ty+1)&511;//wrapRepeat(ty+1, texheight);

	int lineoffset1 = ty0*texwidth;
	int lineoffset2 = ty1*texwidth;
//	int sampleoffset = tx+ty*texwidth;

	__m128i zero, half, p01, c01, p23, c23, result;
	zero = _mm_setzero_si128();
	half = _mm_set1_epi16(0x003f);

	c01 = _mm_set_epi32(0, c1, 0, c0);
	c01 = _mm_shufflehi_epi16(c01, _MM_SHUFFLE(0,0,0,0));
	c01 = _mm_shufflelo_epi16(c01, _MM_SHUFFLE(0,0,0,0));
	p01 = _mm_set_epi32(0, 0, texdata[tx1+lineoffset1], texdata[tx0+lineoffset1]);
//	p01 = _mm_loadl_epi64((__m128i*)(texdata+sampleoffset));
	p01 = _mm_unpacklo_epi8(p01, zero);
	p01 = _mm_mullo_epi16(p01, c01);

	c23 = _mm_set_epi32(0, c3, 0, c2);
	c23 = _mm_shufflehi_epi16(c23, _MM_SHUFFLE(0,0,0,0));
	c23 = _mm_shufflelo_epi16(c23, _MM_SHUFFLE(0,0,0,0));
	p23 = _mm_set_epi32(0, 0, texdata[tx1+lineoffset2], texdata[tx0+lineoffset2]);
//	p23 = _mm_loadl_epi64((__m128i*)(texdata+sampleoffset+texwidth));
	p23 = _mm_unpacklo_epi8(p23, zero);
	p23 = _mm_mullo_epi16(p23, c23);

	result = _mm_add_epi16(p01, p23);
	result = _mm_add_epi16(result, _mm_srli_epi16(result, 8));
	result = _mm_add_epi16(result, half);
	result = _mm_srli_epi16(result, 7);
	result = _mm_packus_epi16(result, zero);
	return _mm_cvtsi128_si32(result);
}
