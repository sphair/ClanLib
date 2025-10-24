
#ifndef header_form_view
#define header_form_view

class FormView : public CL_Component
{
//! Construction:
public:
	FormView(const CL_Rect &pos, CL_Component *parent);

	~FormView();

//! Attributes:
public:

//! Operations:
public:

//! Implementation:
private:
	void on_paint();

	CL_SlotContainer slots;
};

#endif
