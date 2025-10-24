
struct Scanline
{
	Scanline() : dest(0), src(0), src_width(0), src_height(0), x1(0.0f), x2(0.0f), tx1(0.0f), tx2(0.0f), ty1(0.0f), ty2(0.0f), startx(0), endx(0), viewport_center(0), half_viewport_width(0) { }

	unsigned int *dest;
	unsigned int *src;
	unsigned int src_width;
	unsigned int src_height;
	float x1, x2;
	float w1, w2;
	float tx1, tx2;
	float ty1, ty2;
	int startx, endx;
	int viewport_center;
	int half_viewport_width;
};
