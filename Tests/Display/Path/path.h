
#pragma once

enum class CurveType
{
	serpentine,
	cusp,
	loop,
	quadratic,
	line,
	point
};

class CurveClassification
{
public:
	CurveClassification(const clan::Vec2f &cp0, const clan::Vec2f &cp1, const clan::Vec2f &cp2, const clan::Vec2f &cp3);

	bool inside(const clan::Vec3f &uvw) const;

	CurveType type;
	clan::Vec3f uvw_cp[4];
};

class PathProgram
{
public:
	static int main(const std::vector<std::string> &args);
};
