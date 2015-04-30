
#include "precomp.h"
#include "path.h"

using namespace clan;

clan::ApplicationInstance<PathProgram> clanapp;

PathProgram::PathProgram()
{
	clan::OpenGLTarget::enable();

	window = DisplayWindow("Path Test", 640.f, 480.0f);
	sc.connect(window.sig_window_close(), []() { RunLoop::exit(); });

	canvas = Canvas(window);
}

bool PathProgram::update()
{
	canvas.clear(Colorf::whitesmoke);
	window.flip(1);
	return true;
}

CurveClassification::CurveClassification(const clan::Vec2f &cp0, const clan::Vec2f &cp1, const clan::Vec2f &cp2, const clan::Vec2f &cp3)
{
	// Resolution Independent Curve Rendering using Programmable Graphics Hardware
	// Charles Loop, Jim Blinn
	// http://research.microsoft.com/en-us/um/people/cloop/loopblinn05.pdf
	// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch25.html

	clan::Vec3f cp[4];
	cp[0] = Vec3f(cp0.x, cp0.y, 1.0f);
	cp[1] = Vec3f(cp1.x, cp1.y, 1.0f);
	cp[2] = Vec3f(cp2.x, cp2.y, 1.0f);
	cp[3] = Vec3f(cp3.x, cp3.y, 1.0f);

	float a1 = Vec3f::dot(cp[0], Vec3f::cross(cp[3], cp[2]));
	float a2 = Vec3f::dot(cp[1], Vec3f::cross(cp[0], cp[3]));
	float a3 = Vec3f::dot(cp[2], Vec3f::cross(cp[1], cp[1]));

	float d1 = a1 - 2.0f * a2 + 3.0f * a3;
	float d2 = -a2 + 3.0f * a3;
	float d3 = 3.0f * a3;

	float discr = d1 * d1 * (3.0f * d2 * d2 - 4.0f * d1 * d3);

	if (cp[0] == cp[1] && cp[0] == cp[2] && cp[0] == cp[3])
	{
		type = CurveType::point;
	}
	else if (d1 == 0.0f && d2 == 0.0f && d3 == 0.0f)
	{
		type = CurveType::line;
	}
	else if (d1 == 0.0f && d2 == 0.0f)
	{
		type = CurveType::quadratic;

		uvw_cp[0] = Vec3f(0.0f, 0.0f, 0.0f);
		uvw_cp[1] = Vec3f(1.0f / 3.0f, 0.0f, 1.0f / 3.0f);
		uvw_cp[2] = Vec3f(2.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f);
		uvw_cp[3] = Vec3f(1.0f, 1.0f, 1.0f);
	}
	else if (discr > 0.0f)
	{
		type = CurveType::serpentine;

		float sroot = std::sqrt(9.0f * d2 * d2 - 12.0f * d1 * d3);

		float ls = 3.0f * d2 - sroot;
		float lt = 6.0f * d1;

		float ms = 3.0f * d2 + sroot;
		float mt = lt;

		// this might be wrong because the jpg of the matrix is unreadable!
		// to do: read and compare with the research paper
		uvw_cp[0] = Vec3f(ls * ms, ls * ls * ls, ms * ms * ms);
		uvw_cp[1] = Vec3f((3.0f * ls * ms - ls * mt - lt * ms) / 3.0f, ls * ls * (ls - lt), ms * ms * (ms - mt));
		uvw_cp[2] = Vec3f((lt * (mt - 2.0f * ms) + ls * (3.0f * ms - 2.0f * mt)) / 3.0f, (lt - ls) * (lt - ls) * ls, (mt - ms) * (mt - ms) * ms);
		uvw_cp[3] = Vec3f((lt - ls) * (mt - ms), -((lt - ls) * (lt - ls) * (lt - ls)), -((mt - ms) * (mt - ms) * (mt - ms)));

		if (d1 < 0.0f)
		{
			for (int i = 0; i < 4; i++)
			{
				uvw_cp[i].x = -uvw_cp[i].x;
				uvw_cp[i].y = -uvw_cp[i].y;
			}
		}
	}
	else if (discr < -0.0f)
	{
		type = CurveType::loop;

		float sroot = std::sqrt(4.0f * d1 * d3 - 3.0f * d2 * d2);

		float ls = d2 - sroot;
		float lt = 2.0f * d1;

		float ms = d2 + sroot;
		float mt = lt;

		uvw_cp[0] = Vec3f(ls * ms, ls * ls * ms, ls * ms * ms);
		uvw_cp[1] = Vec3f(-mt * ls - lt * ms, -mt * ls * ls - 2.0f * lt * ms * ls, -lt * ms * ms - 2.0f * mt * ls * ms);
		uvw_cp[2] = Vec3f(lt * mt, ms * lt * lt + 2.0f * mt * ls *lt, ls * mt * mt + 2.0f * lt * ms * mt);
		uvw_cp[3] = Vec3f(0.0f, -lt * lt * mt, -lt * mt * mt);

		float dp1 = ls / lt;
		float dp2 = ms / mt;
		if (dp1 > 0.0f && dp1 < 1.0f)
		{
			// To do: split at (ls,lt)
			// To solve this problem, we subdivide the curve at the double point parameter—see Figure 25-5—and reverse the
			// orientation of one of the subcurves. Note that the procedural texture coordinates of the subcurves can be
			// found by subdividing the procedural texture coordinates of the original curve"
		}
		else if (dp2 > 0.0f && dp1 < 1.0f)
		{
			// To do: split at (ms,mt)
			// To solve this problem, we subdivide the curve at the double point parameter—see Figure 25-5—and reverse the
			// orientation of one of the subcurves. Note that the procedural texture coordinates of the subcurves can be
			// found by subdividing the procedural texture coordinates of the original curve"
		}

		if ((d1 > 0.0f && uvw_cp[1].x < 0.0f) || (d1 <0.0f && uvw_cp[1].x > 0.0f))
		{
			for (int i = 0; i < 4; i++)
			{
				uvw_cp[i].x = -uvw_cp[i].x;
				uvw_cp[i].y = -uvw_cp[i].y;
			}
		}
	}
	else
	{
		type = CurveType::cusp;

		if (d1 != 0.0f)
		{
			float sroot = std::sqrt(9.0f * d2 * d2 - 12.0f * d1 * d3);

			float ls = 3.0f * d2 - sroot;
			float lt = 6.0f * d1;

			float ms = 3.0f * d2 + sroot;
			float mt = lt;

			// this might be wrong because the jpg of the matrix is unreadable!
			// to do: read and compare with the research paper
			uvw_cp[0] = Vec3f(ls * ms, ls * ls * ls, ms * ms * ms);
			uvw_cp[1] = Vec3f((3.0f * ls * ms - ls * mt - lt * ms) / 3.0f, ls * ls * (ls - lt), ms * ms * (ms - mt));
			uvw_cp[2] = Vec3f((lt * (mt - 2.0f * ms) + ls * (3.0f * ms - 2.0f * mt)) / 3.0f, (lt - ls) * (lt - ls) * ls, (mt - ms) * (mt - ms) * ms);
			uvw_cp[3] = Vec3f((lt - ls) * (mt - ms), -((lt - ls) * (lt - ls) * (lt - ls)), -((mt - ms) * (mt - ms) * (mt - ms)));

			if (d1 < 0.0f)
			{
				for (int i = 0; i < 4; i++)
				{
					uvw_cp[i].x = -uvw_cp[i].x;
					uvw_cp[i].y = -uvw_cp[i].y;
				}
			}
		}
		else
		{
			float ls = d3;
			float lt = 3.0f * d2;

			uvw_cp[0] = Vec3f(ls, ls * ls * ls, 1.0f);
			uvw_cp[1] = Vec3f(ls - lt / 3.0f, ls * ls * (ls - lt), 1.0f);
			uvw_cp[2] = Vec3f(ls - 2.0f * lt / 3.0f, (ls - lt) * (ls - lt) * ls, 1.0f);
			uvw_cp[3] = Vec3f(ls - lt, (ls - lt) * (ls - lt) * (ls - lt), 1.0f);
		}
	}
}

bool CurveClassification::inside(const Vec3f &uvw) const
{
	float k = uvw.x;
	float l = uvw.y;
	float m = uvw.z;
	return k * k * k - l * m < 0.0f;
}
