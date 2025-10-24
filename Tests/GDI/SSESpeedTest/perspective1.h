
struct FragmentInput
{
	FragmentInput(float tx, float ty) : tx(tx), ty(ty) { }

	float tx;
	float ty;
};

struct FragmentOutput
{
	FragmentOutput(unsigned int color0) : color0(color0) { }

	unsigned int color0;
};

FragmentInput createFragment(Scanline &scanline, float xnormalized)
{
	float x2 = scanline.x2;
	float w2 = scanline.w2;
	float dx = scanline.x2-scanline.x1;
	float dw = scanline.w2-scanline.w1;
	float tx1 = scanline.tx1;
	float tx2 = scanline.tx2;
	float ty1 = scanline.ty1;
	float ty2 = scanline.ty2;

	float a = (x2 - xnormalized * w2) / (dx - xnormalized * dw);
	float inv_a = 1.0f - a;

	float tx = a * tx1 + inv_a * tx2;
	float ty = a * ty1 + inv_a * ty2;

	return FragmentInput(tx, ty);
}

unsigned int blendPixel(unsigned int p, unsigned int blend)
{
	unsigned int alpha = p >> 24;
	unsigned int red = (p >> 16) & 0xff;
	unsigned int green = (p >> 8) & 0xff;
	unsigned int blue = p & 0xff;

	alpha = (alpha * blend) >> 8;
	red = (red * blend) >> 8;
	green = (green * blend) >> 8;
	blue = (blue * blend) >> 8;

	return (alpha << 24) + (red << 16) + (green << 8) + blue;
}

unsigned int sampleLinear(Scanline &scanline, const FragmentInput &input)
{
	float texwidth = (float)scanline.src_width;
	float texheight = (float)scanline.src_height;
	int itexwidth = scanline.src_width;
	int itexheight = scanline.src_height;
	int itexwidthmax = itexwidth - 1;
	int itexheightmax = itexheight - 1;

	float x = input.tx * texwidth;
	float y = input.ty * texheight;
	int ix0 = (int)x;
	int iy0 = (int)y;
	int ix1 = ix0 + 1;
	int iy1 = iy0 + 1;

	float a = x - (float)ix0;
	float b = y - (float)iy0;
	float inv_a = 1.0f - a;
	float inv_b = 1.0f - b;

	float c0 = inv_a * inv_b;
	float c1 = a * inv_b;
	float c2 = inv_a * b;
	float c3 = a * b;

	ix0 = ix0 < itexwidthmax ? ix0 : itexwidthmax;
	ix1 = ix1 < itexwidthmax ? ix1 : itexwidthmax;
	iy0 = iy0 < itexheightmax ? iy0 : itexheightmax;
	iy1 = iy1 < itexheightmax ? iy1 : itexheightmax;

	ix0 = ix0 > 0 ? ix0 : 0;
	ix1 = ix1 > 0 ? ix1 : 0;
	iy0 = iy0 > 0 ? iy0 : 0;
	iy1 = iy1 > 0 ? iy1 : 0;

	iy0 *= itexwidth;
	iy1 *= itexwidth;

	int ioffsetp0 = ix0 + iy0;
	int ioffsetp1 = ix1 + iy0;
	int ioffsetp2 = ix0 + iy1;
	int ioffsetp3 = ix1 + iy1;

	unsigned int *texdata = scanline.src;
	unsigned int p0 = texdata[ioffsetp0];
	unsigned int p1 = texdata[ioffsetp1];
	unsigned int p2 = texdata[ioffsetp2];
	unsigned int p3 = texdata[ioffsetp3];

	unsigned int ic0 = (unsigned int)(c0 * 256.0f + 0.5f);
	unsigned int ic1 = (unsigned int)(c1 * 256.0f + 0.5f);
	unsigned int ic2 = (unsigned int)(c2 * 256.0f + 0.5f);
	unsigned int ic3 = (unsigned int)(c3 * 256.0f + 0.5f);

	return blendPixel(p0, ic0) + blendPixel(p1, ic1) + blendPixel(p2, ic2) + blendPixel(p3, ic3);
}

unsigned int blendFunc(unsigned int src, unsigned int dest)
{
	unsigned int src_alpha = src >> 24;
	unsigned int src_red = (src >> 16) & 0xff;
	unsigned int src_green = (src >> 8) & 0xff;
	unsigned int src_blue = src & 0xff;

	unsigned int dest_alpha = src >> 24;
	unsigned int dest_red = (src >> 16) & 0xff;
	unsigned int dest_green = (src >> 8) & 0xff;
	unsigned int dest_blue = src & 0xff;

	unsigned int alpha = src_alpha|(src_alpha >> 7); // convert alpha to be in range 0-256
	unsigned int inv_alpha = 256-alpha;

	dest_alpha = (dest_alpha * inv_alpha + src_alpha * alpha) >> 8;
	dest_red = (dest_red * inv_alpha + src_red * alpha) >> 8;
	dest_green = (dest_green * inv_alpha + src_green * alpha) >> 8;
	dest_blue = (dest_blue * inv_alpha + src_blue * alpha) >> 8;

	return (dest_alpha << 24) + (dest_red << 16) + (dest_green << 8) + dest_blue;
}

FragmentOutput fragmentProgram(Scanline &scanline, const FragmentInput &input)
{
	return FragmentOutput(sampleLinear(scanline, input));
}

void perspective1(Scanline *d)
{
	float rcp_half_viewportwidth = 1.0f / (float)d->half_viewport_width;

	for (int x = d->startx; x < d->endx; x++)
	{
		float xnormalized = (x - d->viewport_center + 0.5f) * rcp_half_viewportwidth;

		FragmentInput input = createFragment(*d, xnormalized);
		FragmentOutput output = fragmentProgram(*d, input);
		d->dest[x] = blendFunc(output.color0, d->dest[x]);
	}
}
