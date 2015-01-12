/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "fontblowup.h"

#include <stdlib.h>

FontBlowUp::FontBlowUp(Canvas &canvas, const char *_text, int x, int y, clan::Font _font, Colorf _color)
{
	text = _text;
	initial_x = x;
	initial_y = y;

	font = _font;
	color = _color;

	create_letters(canvas);
}

FontBlowUp::~FontBlowUp()
{
	delete[] letters_x;
	delete[] letters_y;
	delete[] letters_z;
	delete[] letters_delta_x;
	delete[] letters_delta_y;
	delete[] letters_delta_z;
}

void FontBlowUp::create_letters(Canvas &canvas)
{
	const char *letters = text.data();
	int num = text.length();

	int total_length = font.measure_text(canvas, text).advance.width;
	int x = initial_x - total_length/2;

	letters_x = new float[num];
	letters_y = new float[num];
	letters_z = new float[num];
	letters_delta_x = new float[num];
	letters_delta_y = new float[num];
	letters_delta_z = new float[num];

	char output[2];
	output[1] = 0;

	for (int i=0; i<num; i++)
	{
		letters_x[i] = (float) x;
		letters_y[i] = (float) initial_y;
		letters_z[i] = 1.0f;
		letters_delta_x[i] = (rand()%2000-1000)/(float) 20;
		letters_delta_y[i] = -(rand()%2000)/(float) 20;
		letters_delta_z[i] = (rand()%2000-1000)/(float) 2000;

		output[0] = letters[i];

		x += font.measure_text(canvas, output).advance.width;
	}
}

bool FontBlowUp::show(Canvas &canvas, float time_elapsed)
{
	bool still_visible = false;

	time_elapsed *= 5; // scott, more power on the warp engines...

	const char *letters = text.data();
	int num = text.length();

	char output[2];
	output[1] = 0;

	for (int i=0; i<num; i++)
	{
		output[0] = letters[i];
		font.draw_text(canvas,
			(int) letters_x[i],
			(int) letters_y[i],
//			letters_z[i],
//			letters_z[i],
			output,
			color);

		letters_x[i] += letters_delta_x[i]*time_elapsed;
		letters_y[i] += letters_delta_y[i]*time_elapsed;
		letters_z[i] += letters_delta_z[i]*time_elapsed;

		letters_delta_y[i] += time_elapsed*20;

		if (letters_y[i] < canvas.get_height()) still_visible = true;
	}

	return still_visible;
}
