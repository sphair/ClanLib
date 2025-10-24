
class ScanlineSpan
{
public:
	ScanlineSpan() : dest(0), src(0), src_width(0), src_height(0), x1(0.0f), x2(0.0f), tx1(0.0f), tx2(0.0f), ty1(0.0f), ty2(0.0f), startx(0), endx(0), viewport_center(0), half_viewport_width(0) { }

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

class ScanlineRow
{
public:
	const int max_spans = 16;
	ScanlineSpan spans[max_spans];
	int count;
};

class TriangleRenderer
{
public:
	TriangleRenderer();
	void triangle(unsigned int v1, unsigned int v2, unsigned int v3, const CL_Rect &clip_rect);
	void sort_triangle_vertices(unsigned int &v1, unsigned int &v2, unsigned int &v3);

	std::vector<ScanlineRow> lines;
	float *x;
	float *y;
	float *tx;
	float *ty;
	float *red;
	float *green;
	float *blue;
	float *alpha;
};

TriangleRenderer::TriangleRenderer()
: x(0), y(0), tx(0), ty(0), red(0), green(0), blue(0), alpha(0)
{
}

void TriangleRenderer::triangle(unsigned int v1, unsigned int v2, unsigned int v3, const CL_Rect &clip_rect)
{
	sort_triangle_vertices(v1, v2, v3);

	int start_y = (int)(floor(y[v1]+0.5f));
	int middle_y = ((int)(floor(y[v2]-0.5f)))+1;
	int end_y = ((int)(floor(y[v3]-0.5f)))+1;

	start_y = cl_max(cl_min(start_y, clip_rect.bottom), clip_rect.top);
	middle_y = cl_max(cl_min(middle_y, clip_rect.bottom), clip_rect.top);
	end_y = cl_max(cl_min(end_y, clip_rect.bottom), clip_rect.top);

	// Band for the area covered by v1 to v2
	for (int y = start_y; y < middle_y; y++)
	{
/*		LinePoint p0, p1;
		get_left_line_x(v1, v2, y, p0);
		get_right_line_x(v1, v3, y, p1);
		render_scanline_nearest(y, p0, p1);*/
	}

	// Band for the area covered by v2 to v3
	for (int y = middle_y; y < end_y; y++)
	{
/*		LinePoint p0, p1;
		get_left_line_x(v2, v3, y, p0);
		get_right_line_x(v1, v3, y, p1);
		render_scanline_nearest(y, p0, p1);*/
	}
}

void TriangleRenderer::sort_triangle_vertices(unsigned int &v1, unsigned int &v2, unsigned int &v3)
{
	if ((y[v1] <= y[v2]) && (y[v2] <= y[v3]))
	{
		// (v1 < v2 < v3)
	}
	else if (y[v1] <= y[v3] && (y[v3] <= y[v2]))
	{
		// (v1 < v3 < v2)
		unsigned int t = v3;
		v3 = v2;
		v2 = t;
	}
	else if ((y[v2] <= y[v3]) && (y[v3] <= y[v1]))
	{
		// (v2 < v3 < v1)
		unsigned int t = v1;
		v1 = v2;
		v2 = v3;
		v3 = t;
	}
	else if ((y[v2] <= y[v1]) && (y[v1] <= y[v3]))
	{
		// (v2 < v1 < v3)
		unsigned int t = v1;
		v1 = v2;
		v2 = t;
	}
	else if ((y[v3] <= y[v2]) && (y[v2] <= y[v1]))
	{
		// (v3 < v2 < v1)
		unsigned int t = v1;
		v1 = v3;
		v3 = t;
	}
	else
	{
		// (v3 < v1 < v2)
		unsigned int t = v1;
		v1 = v3;
		v3 = v2;
		v2 = t;
	}
}
/*
void TriangleRenderer::get_left_line_x(unsigned int v1, unsigned int v2, unsigned int yposi, LinePoint &out_point)
{
	get_line_x(v1, v2, v1, yposi, out_point);
}

void TriangleRenderer::get_right_line_x(unsigned int v1, unsigned int v2, unsigned int yposi, LinePoint &out_point)
{
	get_line_x(v1, v2, v2, yposi, out_point);
}

void TriangleRenderer::get_line_x(unsigned int v1, unsigned int v2, unsigned int horz_v, unsigned int yposi, LinePoint &out_point)
{
	if (y[v1] == y[v2]) // Horizontal line
	{
		out_point.x = x[horz_v];
		out_point.tx = tx[horz_v];
		out_point.ty = ty[horz_v];
		out_point.r = red[horz_v];
		out_point.g = green[horz_v];
		out_point.b = blue[horz_v];
		out_point.a = alpha[horz_v];
	}
	else
	{
		float ypos = yposi+0.5f;
		float dy = y[v2]-y[v1];
		float slope_x = (x[v2]-x[v1])/dy;
		float slope_tx = (tx[v2]-tx[v1])/dy;
		float slope_ty = (ty[v2]-ty[v1])/dy;
		float slope_r = (red[v2]-red[v1])/dy;
		float slope_g = (green[v2]-green[v1])/dy;
		float slope_b = (blue[v2]-blue[v1])/dy;
		float slope_a = (alpha[v2]-alpha[v1])/dy;
		float t = ypos-y[v1];
		out_point.x = x[v1]+slope_x*t;
		out_point.tx = tx[v1]+slope_tx*t;
		out_point.ty = ty[v1]+slope_ty*t;
		out_point.r = red[v1]+slope_r*t;
		out_point.g = green[v1]+slope_g*t;
		out_point.b = blue[v1]+slope_b*t;
		out_point.a = alpha[v1]+slope_a*t;
	}
}
*/
