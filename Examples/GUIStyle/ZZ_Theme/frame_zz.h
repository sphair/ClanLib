
#ifndef header_frame_style_zz
#define header_frame_style_zz

#if _MSC_VER > 1000
#pragma once
#endif

#include <ClanLib/gui.h>

class CL_ComponentOptions;
class CL_StyleManager_ZZ;

class CL_Frame_ZZ : public CL_ComponentStyle
{
public:
	CL_Frame_ZZ(
		CL_Frame *frame,
		CL_StyleManager_ZZ *style);

private:
	void on_draw();

	CL_Frame *frame;
	CL_Slot slot_draw;
	CL_StyleManager_ZZ *style;
};

#endif
