#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "frame_zz.h"
#include "stylemanager_zz.h"

CL_Frame_ZZ::CL_Frame_ZZ(
	CL_Frame *frame,
	CL_StyleManager_ZZ *style)
:
	CL_ComponentStyle(frame),
	frame(frame),
	style(style)
{
	slot_draw = frame->sig_paint().connect(this, &CL_Frame_ZZ::on_draw);
}

void CL_Frame_ZZ::on_draw()
{
	CL_Display::clear(CL_Color(108, 139, 138));
}
