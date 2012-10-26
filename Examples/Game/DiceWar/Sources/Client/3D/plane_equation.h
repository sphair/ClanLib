
#pragma once

class PlaneEquation
{
public:
	PlaneEquation()
	: a(0.0), b(0.0), c(0.0), d(0.0)
	{
	}

	PlaneEquation(double a, double b, double c, double d)
	: a(a), b(b), c(c), d(d)
	{
	}

	double a, b, c, d;

	void normalize()
	{
		double length = sqrt(a*a+b*b+c*c);
		a /= length;
		b /= length;
		c /= length;
		d /= length;
	}

	double distance(const Vec3d &point) const
	{
		return a * point.x + b * point.y + c * point.z + d;
	}

	Vec3d get_normal() const
	{
		return Vec3d(a, b, c);
	}

	Vec3d get_origin() const
	{
		return Vec3d(-a*d, -b*d, -c*d);
	}

	static PlaneEquation left_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] + matrix[0+0*4],
			matrix[3+1*4] + matrix[0+1*4],
			matrix[3+2*4] + matrix[0+2*4],
			matrix[3+3*4] + matrix[0+3*4]);
	}

	static PlaneEquation right_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] - matrix[0+0*4],
			matrix[3+1*4] - matrix[0+1*4],
			matrix[3+2*4] - matrix[0+2*4],
			matrix[3+3*4] - matrix[0+3*4]);
	}

	static PlaneEquation top_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] - matrix[1+0*4],
			matrix[3+1*4] - matrix[1+1*4],
			matrix[3+2*4] - matrix[1+2*4],
			matrix[3+3*4] - matrix[1+3*4]);
	}

	static PlaneEquation bottom_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] + matrix[1+0*4],
			matrix[3+1*4] + matrix[1+1*4],
			matrix[3+2*4] + matrix[1+2*4],
			matrix[3+3*4] + matrix[1+3*4]);
	}

	static PlaneEquation near_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] + matrix[2+0*4],
			matrix[3+1*4] + matrix[2+1*4],
			matrix[3+2*4] + matrix[2+2*4],
			matrix[3+3*4] + matrix[2+3*4]);
	}

	static PlaneEquation far_frustum(const Mat4f &matrix)
	{
		return PlaneEquation(
			matrix[3+0*4] - matrix[2+0*4],
			matrix[3+1*4] - matrix[2+1*4],
			matrix[3+2*4] - matrix[2+2*4],
			matrix[3+3*4] - matrix[2+3*4]);
	}
};

#endif
