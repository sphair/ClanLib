
#include "precomp.h"
#include "form_view.h"

/////////////////////////////////////////////////////////////////////////////
// FormView construction:

FormView::FormView(const CL_Rect &pos, CL_Component *parent)
: CL_Component(pos, parent)
{
	slots.connect(sig_paint(), this, &FormView::on_paint);
}

FormView::~FormView()
{
}

/////////////////////////////////////////////////////////////////////////////
// FormView attributes:


/////////////////////////////////////////////////////////////////////////////
// FormView operations:


/////////////////////////////////////////////////////////////////////////////
// FormView implementation:

void FormView::on_paint()
{
	int width = get_width();
	int height = get_height();

	CL_Display::fill_rect(CL_Rect(1, 1, width-1, 20), CL_Color(240,240,240));
	CL_Display::fill_rect(CL_Rect(1, 1, 20, height-1), CL_Color(240,240,240));
	CL_Display::draw_line(20, 20, width-1, 20, CL_Color(128, 142, 159));
	CL_Display::draw_line(20, 20, 20, height-1, CL_Color(128, 142, 159));

	for (int x=0; x<width-21; x+=5)
	{
		switch (x % 100)
		{
		case 0:
			CL_Display::draw_line(20+x, 10, 20+x, 20, CL_Color(128, 142, 159));
			break;

		case 50:
			CL_Display::draw_line(20+x, 13, 20+x, 20, CL_Color(128, 142, 159));
			break;

		case 25:
		case 75:
			CL_Display::draw_line(20+x, 15, 20+x, 20, CL_Color(128, 142, 159));
			break;

		default:
			CL_Display::draw_line(20+x, 17, 20+x, 20, CL_Color(128, 142, 159));
			break;
		}
	}

	for (int y=0; y<height-21; y+=5)
	{
		switch (y % 100)
		{
		case 0:
			CL_Display::draw_line(10, 20+y, 20, 20+y, CL_Color(128, 142, 159));
			break;

		case 50:
			CL_Display::draw_line(13, 20+y, 20, 20+y, CL_Color(128, 142, 159));
			break;

		case 25:
		case 75:
			CL_Display::draw_line(15, 20+y, 20, 20+y, CL_Color(128, 142, 159));
			break;

		default:
			CL_Display::draw_line(17, 20+y, 20, 20+y, CL_Color(128, 142, 159));
			break;
		}
	}

	CL_Display::fill_rect(CL_Rect(21, 21, 400, 300), CL_Color::white);
	for (int x=0; x<400-20; x+=5) CL_Display::draw_line(400-x, 300, 400-x-2, 300, CL_Color::black);
	for (int y=0; y<300-20; y+=5) CL_Display::draw_line(400, 300-y, 400, 300-y-2, CL_Color::black);

//	CL_Display::fill_rect(CL_Rect(400/2-3, 300-3, 400/2+2, 300+2), CL_Color::black);
//	CL_Display::fill_rect(CL_Rect(400-3, 300/2-3, 400+2, 300/2+2), CL_Color::black);
//	CL_Display::fill_rect(CL_Rect(400-3, 300-3, 400+2, 300+2), CL_Color::black);

	CL_Display::draw_rect(CL_Rect(0, 0, width, height), CL_Color(128, 142, 159));
}
