
#include "precomp.h"
#include "anchor_popup.h"

AnchorPopup::AnchorPopup(CL_ComponentAnchorPoint cap_tl, CL_ComponentAnchorPoint cap_br, CL_GUIComponent *parent)
: CL_GUIComponent(parent/*, get_toplevel_description()*/), cap_tl(cap_tl), cap_br(cap_br)
{
	set_type_name("tooltip");
	func_resized().set(this, &AnchorPopup::on_resized);

	label_tl = new CL_Label(this);
	label_br = new CL_Label(this);
	tl = new CL_RadioButton(this);
	tr = new CL_RadioButton(this);
	bl = new CL_RadioButton(this);
	br = new CL_RadioButton(this);
	scale = new CL_RadioButton(this);
	br_tl = new CL_RadioButton(this);
	br_tr = new CL_RadioButton(this);
	br_bl = new CL_RadioButton(this);
	br_br = new CL_RadioButton(this);
	br_scale = new CL_RadioButton(this);
}

CL_GUITopLevelDescription AnchorPopup::get_toplevel_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_decorations(false);
	return desc;
}

AnchorPopup::~AnchorPopup()
{
}

void AnchorPopup::on_resized()
{
	if (get_height() != 150)
	{
		set_geometry(CL_Rect(get_geometry().get_top_left(), CL_Size(get_width(), 150)));
	}
	else
	{
		int top = 0;
		int left = 20;

		label_tl->set_text(cl_text("Top Left:"));
		label_tl->set_geometry(CL_RectPS(left-10,top+6, 80,23));

		tl->set_id_name(cl_text("tl")); tr->set_id_name(cl_text("tr")); bl->set_id_name(cl_text("bl")); br->set_id_name(cl_text("br"));
		scale->set_id_name(cl_text("%"));

		top += 20;

		CL_Size s = get_geometry().get_size();
		//top = rect.bottom+4+32;
		tl->set_geometry(CL_RectPS(        left, top,    20, 23));
 		tr->set_geometry(CL_RectPS(     50+left, top,    20, 23));
		bl->set_geometry(CL_RectPS(        left, top+32, 20, 23));
		br->set_geometry(CL_RectPS(     50+left, top+32, 20, 23));
		scale->set_geometry(CL_RectPS(  25+left, top+16, 20, 23));

		tl->set_group_name(cl_text("TL"));
		tr->set_group_name(cl_text("TL"));
		bl->set_group_name(cl_text("TL"));
		br->set_group_name(cl_text("TL"));
		scale->set_group_name(cl_text("TL"));

		br_tl->set_id_name(cl_text("tl")); br_tr->set_id_name(cl_text("tr")); br_bl->set_id_name(cl_text("bl")); br_br->set_id_name(cl_text("br"));
		br_scale->set_id_name(cl_text("%"));

		top += 50;

		label_br->set_text(cl_text("Bottom Right:"));
		label_br->set_geometry(CL_RectPS(left-10,top+6, 80,23));

		top += 20;

		br_tl->set_geometry(CL_RectPS(        left, top,    20, 23));
		br_tr->set_geometry(CL_RectPS(     left+50, top,    20, 23));
		br_bl->set_geometry(CL_RectPS(        left, top+32, 20, 23));
		br_br->set_geometry(CL_RectPS(     left+50, top+32, 20, 23));
		br_scale->set_geometry(CL_RectPS(  left+25, top+16, 20, 23));

		br_tl->set_group_name(cl_text("BR"));
		br_tr->set_group_name(cl_text("BR"));
		br_bl->set_group_name(cl_text("BR"));
		br_br->set_group_name(cl_text("BR"));
		br_scale->set_group_name(cl_text("BR"));

		scale->func_group_selection_changed().set(this, &AnchorPopup::on_anchoring_changed);
		br_scale->func_group_selection_changed().set(this, &AnchorPopup::on_anchoring_changed);

//		CL_ComponentAnchorPoint cap_tl = selected_holder->get_anchor_tl();
//		CL_ComponentAnchorPoint cap_br = selected_holder->get_anchor_br();

		switch (cap_tl)
		{
		case cl_anchor_top_left: tl->set_selected(true); break;
		case cl_anchor_top_right: tr->set_selected(true); break;
		case cl_anchor_bottom_left: bl->set_selected(true); break;
		case cl_anchor_bottom_right: br->set_selected(true); break;
		case cl_anchor_relative: scale->set_selected(true); break;
		default:break;
		}

		switch (cap_br)
		{
		case cl_anchor_top_left: br_tl->set_selected(true); break;
		case cl_anchor_top_right: br_tr->set_selected(true); break;
		case cl_anchor_bottom_left: br_bl->set_selected(true); break;
		case cl_anchor_bottom_right: br_br->set_selected(true); break;
		case cl_anchor_relative: br_scale->set_selected(true); break;
		default:break;
		}
	}
}

void AnchorPopup::on_anchoring_changed(CL_RadioButton *rb)
{
	CL_String id = rb->get_id_name();

	if (rb->get_group_name() == cl_text("TL"))
	{
		if (id == cl_text("tl"))
		{
			cap_tl = cl_anchor_top_left;
		}
		else if (id == cl_text("tr"))
		{
			cap_tl = cl_anchor_top_right;
		}
		else if (id == cl_text("bl"))
		{
			cap_tl = cl_anchor_bottom_left;
		}
		else if (id == cl_text("br"))
		{
			cap_tl = cl_anchor_bottom_right;
		}
		else if (id == cl_text("%"))
		{
			cap_tl = cl_anchor_relative;
		}
	}
	else
	{
		if (id == cl_text("tl"))
		{
			cap_br = cl_anchor_top_left;
		}
		else if (id == cl_text("tr"))
		{
			cap_br = cl_anchor_top_right;
		}
		else if (id == cl_text("bl"))
		{
			cap_br = cl_anchor_bottom_left;
		}
		else if (id == cl_text("br"))
		{
			cap_br = cl_anchor_bottom_right;
		}
		else if (id == cl_text("%"))
		{
			cap_br = cl_anchor_relative;
		}
	}
}
