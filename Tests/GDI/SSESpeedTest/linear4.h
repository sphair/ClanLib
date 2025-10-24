
void linear4(Scanline *d)
{
	unsigned int *dest = d->dest;
	unsigned int *texdata = d->src;
	unsigned int texwidth = d->src_width;
	unsigned int texheight = d->src_height;

	float startoffset = d->startx+0.5f-d->x1;
	float deltax = d->x2-d->x1;
	float deltatx = d->tx2-d->tx1;
	float deltaty = d->ty2-d->ty1;

	int dtx = (int)(deltatx/deltax*32768);
	int dty = (int)(deltaty/deltax*32768);
	int tx = (int)(d->tx1*32768+startoffset*dtx);
	int ty = (int)(d->ty1*32768+startoffset*dty);

	tx *= texwidth;
	dtx *= texwidth;
	ty *= texheight;
	dty *= texheight;

	__m128i one, half, zero;
	one = _mm_set1_epi16(0x0100);
	half = _mm_set1_epi16(0x007f);
	zero = _mm_setzero_si128();

	__m128i clampmax = _mm_set_epi16(texwidth-1, texheight-1, texwidth-1, texheight-1, texwidth-1, texheight-1, texwidth-1, texheight-1);

	for (int x = d->startx; x < d->endx; x+=2)
	{
		int tx0 = tx;
		int ty0 = ty;
		tx += dtx;
		ty += dty;
		int tx1 = tx;
		int ty1 = ty;
		tx += dtx;
		ty += dty;

		__m128i src0, dest0;
		__m128i src_alpha, invsrc_alpha;
		sampleLinearRepeat4(tx0, ty0, tx1, ty1, texwidth, clampmax, texdata, src0);
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
