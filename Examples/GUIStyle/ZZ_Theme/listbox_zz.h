
#ifndef header_listbox_style_zz
#define header_listbox_style_zz

#if _MSC_VER > 1000
#pragma once
#endif

#include <ClanLib/gui.h>
#include "listboxitem_zz.h"

class CL_Font;
class CL_ListBox;
class CL_StyleManager_ZZ;
class CL_ResourceManager;
class CL_DomElement;

class CL_ListBox_ZZ : public CL_ComponentStyle
{
    enum ListBoxType {
        type_normal,
        type_levels_list
    };
public:
	CL_ListBox_ZZ(
		CL_ListBox * listbox,
		CL_StyleManager_ZZ * style);
	
	virtual ~CL_ListBox_ZZ();
private:
	CL_StyleManager_ZZ * style_;
	CL_ResourceManager * resources_;
	CL_Font * font_;

    bool first_paint_;
    CL_SlotContainer slots_;
    ListBoxType listbox_type_;

	void on_mouse_down(const CL_InputEvent &key);
	void on_set_options(const CL_DomElement &options);
	void on_scroll_change(int new_offset);
	void on_resize(int old_width, int old_height);
	void on_paint_background();
	void on_paint_listbox();
	void on_item_added(int index);
	void on_item_removed(int index);
	void on_clear();
	
	void update_scrollbar(bool resize);

	CL_ListBox *listbox_;
	CL_ScrollBar *scrollbar_;
	
	bool visible_scrollbar_;
};

#endif
