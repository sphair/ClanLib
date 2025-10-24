
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/swrender.h>
#include <emmintrin.h>
#include "scanline.h"
#include "sampler.h"
#include "sampler2.h"
#include "sampler3.h"
#include "sampler4.h"
#include "clip.h"
#include "blend1.h"
#include "blend2.h"
#include "nearest1.h"
#include "linear1.h"
#include "linear2.h"
#include "linear3.h"
#include "linear4.h"
#include "linear5.h"
#include "linear6.h"
#include "linear7.h"
#include "linear8.h"
#include "perspective1.h"
#include "perspective2.h"

void time_algorithm(const CL_String &name, void(*func)(Scanline *), CL_DisplayWindow *window)
{
	CL_PixelBuffer pb_colorbuffer0(1600, 1200, cl_argb8);
	CL_PixelBuffer pb_texture0(512, 512, cl_argb8);
	unsigned int *colorbuffer0 = (unsigned int*)pb_colorbuffer0.get_data();
	unsigned int *texture0 = (unsigned int*)pb_texture0.get_data();

	CL_ImageProviderFactory::load("tux.png").convert(pb_texture0);

	// unsigned int *colorbuffer0 = (unsigned int*)_aligned_malloc(1600*1200*4, 64);
	// unsigned int *texture0 = (unsigned int*)_aligned_malloc(512*512*4, 64);

	Scanline d;
	d.dest = colorbuffer0;
	d.src = texture0;
	d.src_width = 512;
	d.src_height = 512;
	d.x1 = -1.0f;
	d.x2 = 1.0f;
	d.w1 = 1.0f;
	d.w2 = 1.0f;
	d.tx1 = 0.0f;
	d.tx2 = 1600.0f/512.0f;
	d.ty1 = 0.0f;
	d.ty2 = 0.0f;
	d.viewport_center = 1600/2;
	d.half_viewport_width = 1600/2;

	unsigned int start = CL_System::get_time();
	for (int i = 0; i < 100; i++)
	{
		for (int y = 0; y < 1200; y++)
		{
			d.dest = colorbuffer0 + y*1600;
			d.ty1 = (y+0.5f)/512.0f;
			d.ty2 = (y+0.5f)/512.0f;
			clipScanline(&d, 0, 1600);
			func(&d);
		}
		if (window)
			break;
	}
	unsigned int end = CL_System::get_time();
	if (!window)
		CL_Console::write_line("%3: %1 ms elapsed, %2 fps", end-start, 100000/(end-start), name);

	if (window)
	{
		while (!window->get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
		{
			window->get_gc().draw_pixels(0.0f, 0.0f, pb_colorbuffer0, CL_Rect(0,0,1600,1200));
			window->flip();
			CL_KeepAlive::process();
		}
	}

	// _aligned_free(colorbuffer0);
	// _aligned_free(texture0);
}

int main(int, char **)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupSWRender setup_swrender;

	CL_DisplayWindow window("SSE Speed Test", 1600, 1200);
	//time_algorithm("perspective1", perspective1, &window);
	//time_algorithm("perspective1", perspective1, 0);
	time_algorithm("perspective2", perspective2, &window);
	time_algorithm("perspective2", perspective2, 0);
	return 0;
}
