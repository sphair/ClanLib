
inline unsigned int sampleLinearRepeat3(int xx, int yy, int texwidth, int texheight, unsigned int *texdata)
{
	int tx = xx>>15;
	int ty = yy>>15;
	int a = xx & 0x7fff;
	int b = yy & 0x7fff;
	int inv_a = 0x8000-a;
	int inv_b = 0x8000-b;

	int c1 = (a*inv_b+64)>>(15+8);
	int c2 = (inv_a*b+64)>>(15+8);
	int c3 = (a*b+64)>>(15+8);
	int c0 = 128-c1-c2-c3; //(inv_a*inv_b+64)>>(15+8);

/*
	// wrapRepeat any size:
	int tx0 = tx%texwidth;//wrapRepeat(tx, texwidth);
	int ty0 = ty%texheight;//wrapRepeat(ty, texheight);
	int tx1 = (tx+1)%texwidth;//wrapRepeat(tx+1, texwidth);
	int ty1 = (ty+1)%texheight;//wrapRepeat(ty+1, texheight);
*/
/*
	// wrapRepeat power of two:
	int tx0 = tx&511;//wrapRepeat(tx, texwidth);
	int ty0 = ty&511;//wrapRepeat(ty, texheight);
	int tx1 = (tx+1)&511;//wrapRepeat(tx+1, texwidth);
	int ty1 = (ty+1)&511;//wrapRepeat(ty+1, texheight);
*/

	// wrapClampToEdge:
	__m128i clampquad = _mm_set_epi32(tx, ty, tx+1, ty+1);
	clampquad = _mm_max_epi16(clampquad, _mm_setzero_si128());
	clampquad = _mm_min_epi16(clampquad, _mm_set_epi32(texwidth-1, texheight-1, texwidth-1, texheight-1));
	int clampedquad[4];
	_mm_storeu_si128((__m128i*)clampedquad, clampquad);
	int tx0 = clampedquad[3];
	int ty0 = clampedquad[2];
	int tx1 = clampedquad[1];
	int ty1 = clampedquad[0];
/*
	int tx0 = tx;
	int ty0 = ty;
	int tx1 = tx+1;
	int ty1 = ty+1;
	tx0 = tx0 >= 0 ? tx0 : 0;
	tx0 = tx0 < (texwidth-1) ? tx0 : (texwidth-1);
	tx1 = tx1 >= 0 ? tx1 : 0;
	tx1 = tx1 < (texwidth-1) ? tx1 : (texwidth-1);
	ty0 = ty0 >= 0 ? ty0 : 0;
	ty0 = ty0 < (texheight-1) ? ty0 : (texheight-1);
	ty1 = ty1 >= 0 ? ty1 : 0;
	ty1 = ty1 < (texheight-1) ? ty1 : (texheight-1);
*/

	__m128i zero, half, p0, p1, p2, p3, tmp0, tmp1, result;
	zero = _mm_setzero_si128();
	half = _mm_set1_epi16(0x003f);
	p0 = _mm_cvtsi32_si128(texdata[tx0+ty0*texwidth]);

	tmp0 = _mm_unpacklo_epi8(p0, zero);
	tmp1 = _mm_set1_epi16(c0);
	result = _mm_mullo_epi16(tmp0, tmp1);

	p1 = _mm_cvtsi32_si128(texdata[tx1+ty0*texwidth]);
	tmp0 = _mm_unpacklo_epi8(p1, zero);
	tmp1 = _mm_set1_epi16(c1);
	result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

	p2 = _mm_cvtsi32_si128(texdata[tx0+ty1*texwidth]);
	tmp0 = _mm_unpacklo_epi8(p2, zero);
	tmp1 = _mm_set1_epi16(c2);
	result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

	p3 = _mm_cvtsi32_si128(texdata[tx1+ty1*texwidth]);
	tmp0 = _mm_unpacklo_epi8(p3, zero);
	tmp1 = _mm_set1_epi16(c3);
	result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));
	
	result = _mm_add_epi16(result, half);
	result = _mm_srli_epi16(result, 7);
	result = _mm_packus_epi16(result, zero);
	return _mm_cvtsi128_si32(result);
}
