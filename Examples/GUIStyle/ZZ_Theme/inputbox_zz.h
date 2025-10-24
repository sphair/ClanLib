
#ifndef header_inputbox_style_zz
#define header_inputbox_style_zz

#if _MSC_VER > 1000
#pragma once
#endif

#include <ClanLib/gui.h>
#include <ClanLib/GUI/inputbox_stylebase.h>

class CL_Font;
class CL_InputBox;
class CL_StyleManager_ZZ;

class CL_InputBox_ZZ
	: public CL_ComponentStyle
	, public CL_InputBox_StyleBase
{
public:
	CL_InputBox_ZZ(
		CL_InputBox *inputbox,
		CL_StyleManager_ZZ *style);
	
	virtual ~CL_InputBox_ZZ();

private:
	CL_SlotContainer slots;
	void on_paint();
	void on_changed();
	void on_paint_default();
	void on_paint_input();

	//CL_StyleManager_ZZ *style;
	//CL_ResourceManager *resources;

    bool first_paint;
};

#endif
