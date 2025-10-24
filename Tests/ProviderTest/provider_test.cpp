/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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

#include "provider_test.h"

ProviderTest app;

int
ProviderTest::main(int argc, char** argv)
{
        CL_SetupCore::init();
        CL_SetupDisplay::init();
#ifdef USE_SDL        
        CL_SetupSDL::init();
#else
        CL_SetupGL::init();
#endif
        // Set a videomode - 640x480x16bpp
        // false means that this program should not run full-screen
        CL_DisplayWindow window("ProviderTest", 640, 480);

        CL_Surface colorkeyed_png("colorkeyed_png.png");
        CL_Surface indexed_png("indexed_png.png");
        CL_Surface grayscaled_png("grayscaled_png.png");
        CL_Surface grayscaled_alpha_png("grayscaled_alpha_png.png");
        CL_Surface grayscaled_colorkey_png("grayscaled_colorkey_png.png");
        CL_Surface rgb_png("rgb_png.png");
        CL_Surface rgba_png("rgba_png.png");
        CL_Surface grayscaled_jpg("grayscaled_jpg.jpg");
        CL_Surface truecolor_jpg("truecolor_jpg.jpg");

        while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
        {
                const int check_size = 16;
                CL_Display::clear(CL_Color(0, 0, 0));
                for (int y = 0; y < 640; y += check_size*2)
                {
                        for (int x = 0; x < 640; x += check_size*2)
                                CL_Display::fill_rect(CL_Rect(x, y, x + check_size, y + check_size),
                                                      CL_Color(100, 100, 100));
                        for (int x = check_size; x < 640; x += check_size*2)
                                CL_Display::fill_rect(CL_Rect(x, y + check_size, 
                                                              x + check_size, y + check_size*2),
                                                      CL_Color(100, 100, 100));
                }

                colorkeyed_png.draw(40, 30);
                indexed_png.draw(240, 30);
                rgba_png.draw(440, 30);
	   
                grayscaled_png.draw(40, 180);
                grayscaled_alpha_png.draw(240, 180);
                grayscaled_colorkey_png.draw(440, 180);

                rgb_png.draw(40, 330);
                grayscaled_jpg.draw(240, 330);
                truecolor_jpg.draw(440, 330);

                CL_Display::flip();
                CL_System::keep_alive();
                CL_System::sleep(10);
        }
        
#ifdef USE_SDL        
        CL_SetupSDL::deinit();
#else
        CL_SetupGL::deinit();
#endif
        CL_SetupDisplay::deinit();
        CL_SetupCore::deinit();

        return 0;
}

/* EOF */
