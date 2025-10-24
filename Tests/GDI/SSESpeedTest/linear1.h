
void linear1(Scanline *d)
{
	unsigned int *dest = d->dest;
	unsigned int *texdata = d->src;
	unsigned int texwidth = d->src_width;
	unsigned int texheight = d->src_height;

	float startoffset = d->startx+0.5f-d->x1;
	float deltax = d->x2-d->x1;
	float deltatx = d->tx2-d->tx1;
	float deltaty = d->ty2-d->ty1;

	float dtx = deltatx/deltax;
	float dty = deltaty/deltax;
	float tx = d->tx1+startoffset*dtx;
	float ty = d->ty1+startoffset*dty;

	__m128i one, half, zero;
	one = _mm_set1_epi16(0x0100);
	half = _mm_set1_epi16(0x007f);
	zero = _mm_setzero_si128();

	for (int x = d->startx; x < d->endx; x+=2)
	{
		unsigned int srcpixels[2];
		srcpixels[0] = sampleLinearRepeat(tx, ty, texwidth, texheight, texdata); tx += dtx; ty += dty;
		srcpixels[1] = sampleLinearRepeat(tx, ty, texwidth, texheight, texdata); tx += dtx; ty += dty;

		__m128i src0, dest0;
		__m128i src_alpha, invsrc_alpha;
		src0 = _mm_loadl_epi64((__m128i*)srcpixels);
		dest0 = _mm_loadl_epi64((__m128i*)(dest+x));

		src0 = _mm_unpacklo_epi8(src0, zero);
		dest0 = _mm_unpacklo_epi8(dest0, zero);
		src_alpha = src0;
		src_alpha = _mm_shufflelo_epi16(src_alpha, 0xff);
		src_alpha = _mm_shufflehi_epi16(src_alpha, 0xff);
		invsrc_alpha = _mm_sub_epi16(one, src_alpha);
		dest0 = _mm_mullo_epi16(dest0, invsrc_alpha);
		dest0 = _mm_add_epi16(dest0, half);
		dest0 = _mm_srli_epi16(dest0, 8);
		dest0 = _mm_add_epi16(dest0, src0);
		dest0 = _mm_packus_epi16(dest0, zero);

		_mm_storel_epi64((__m128i*)(dest+x), dest0);
	}
}
