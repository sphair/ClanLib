/*
**  FMP_Example
**  Copyright (c) 2005  Erik Ylvisaker
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
**    Erik Ylvisaker
**    (if your name is missing here, please add it)
*/



#include <clanlib/core.h>
#include <clanlib/display.h>
#include <clanlib/gl.h>
#include <clanlib/application.h>
#include <clanlib/gui.h>

#include "CL_FlexibleMap.h"


class FMP_Example :
	public CL_ClanApplication
{
	public:
		FMP_Example(void);
		~FMP_Example(void);

		// the main function
		int main(int argc, char** argv);


		void on_window_close();
		void on_key_down(const CL_InputEvent &e);
		void on_key_up(const CL_InputEvent &e);

};
