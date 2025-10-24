
#ifndef header_stylemanager_zz
#define header_stylemanager_zz

#if _MSC_VER > 1000
#pragma once
#endif

#include <ClanLib/gui.h>
#include <iosfwd>

class CL_Color;

class CL_StyleManager_ZZ : public CL_StyleManager
{
public:
	CL_StyleManager_ZZ(CL_ResourceManager *resources);
	virtual ~CL_StyleManager_ZZ();

// Operations:
	static void fill_rect(
		int x1, int y1, int x2, int y2,
		float r, float g, float b,
		float alpha0, float alpha1, float alpha2, float alpha3);

	static void fill_rect(
		int x1, int y1, int x2, int y2,
		float r0, float g0, float b0, 
		float r1, float g1, float b1, 
		float r2, float g2, float b2, 
		float r3, float g3, float b3,
		float alpha);
    static void draw_rounded_rect(CL_Rect const & rc, CL_Color const & clr);
    static void fill_rounded_rect(CL_Rect const & rc, CL_Color const & clr);

// Overridables:
	virtual void connect_styles(
		const std::string &type,
		CL_Component *owner);
};

// this need for debug
/*
std::ostream & operator<<(std::ostream & os, CL_Rect const & rc);
std::ostream & operator<<(std::ostream & os, CL_Point const & pt);
std::ostream & operator<<(std::ostream & os, CL_Size const & sz);
*/


#endif
