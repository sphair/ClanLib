
int wrapClampToEdge(int x, int length)
{
	if (x < 0)
		x = 0;
	if (x >= length)
		x = length-1;
	return x;
}

int wrapRepeat(int x, int length)
{
	x %= length;
	if (x < 0)
		x += length;
	return x;
}

float frac(float x)
{
	return x-floorf(x);
/*	__m128 x0 = _mm_load_ss(&x);
	__m128 xfloor = _mm_cvtsi32_ss(x0, _mm_cvtss_si32(x0));
	x0 = _mm_sub_ss(x0, xfloor);
	_mm_store_ss(&x, x0);
	return x; */
}

unsigned int sampleNearestRepeat(float x, float y, int texwidth, int texheight, unsigned int *texdata)
{
	int texoffset = wrapRepeat((int)(x*texwidth), texwidth) + wrapRepeat((int)(y*texheight), texheight)*texwidth;
	return texdata[texoffset];
}

unsigned int sampleLinearRepeat(float x, float y, int texwidth, int texheight, unsigned int *texdata)
{
	x = x*texwidth;
	y = y*texheight;
	float a = frac(x);
	float b = frac(y);
	float inv_a = 1.0f-a;
	float inv_b = 1.0f-b;
	int tx = (int) x;
	int ty = (int) y;

	int tx0 = tx&511;//wrapRepeat(tx, texwidth);
	int ty0 = ty&511;//wrapRepeat(ty, texheight);
	int tx1 = (tx+1)&511;//wrapRepeat(tx+1, texwidth);
	int ty1 = (ty+1)&511;//wrapRepeat(ty+1, texheight);

	int c1 = (int)(a*inv_b*128+0.5f);
	int c2 = (int)(inv_a*b*128);
	int c3 = (int)(a*b*128);
	int c0 = 128-c1-c2-c3; //(int)(inv_a*inv_b*128+0.5f);

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
