
#pragma once

class AnchorPopup : public CL_GUIComponent
{
public:
	AnchorPopup(CL_ComponentAnchorPoint tl, CL_ComponentAnchorPoint br, CL_GUIComponent *parent);
	~AnchorPopup();

	CL_ComponentAnchorPoint get_anchor_tl() const { return cap_tl; }
	CL_ComponentAnchorPoint get_anchor_br() const { return cap_br; }

private:
	static CL_GUITopLevelDescription get_toplevel_description();
	void on_resized();
	void on_anchoring_changed(CL_RadioButton *rb);

	CL_ComponentAnchorPoint cap_tl, cap_br;
	CL_Label *label_tl;
	CL_Label *label_br;
	CL_RadioButton *tl;
	CL_RadioButton *tr;
	CL_RadioButton *bl;
	CL_RadioButton *br;
	CL_RadioButton *scale;
	CL_RadioButton *br_tl;
	CL_RadioButton *br_tr;
	CL_RadioButton *br_bl;
	CL_RadioButton *br_br;
	CL_RadioButton *br_scale;
};
