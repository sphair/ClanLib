/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

/** \addtogroup clanDisplay_Window clanDisplay Window */
//@{
/** header=display.h */

#ifndef header_display_window_mode
#define header_display_window_mode

#include "../api_display.h"
#include "../../Core/Text/string_types.h"

class CL_Size;
class CL_Rect;

#include <vector>

class CL_DisplayWindowMode_Impl;

/** \brief  Display window mode class.*/
/** !group=Display/Window!
    !header=display.h!*/
/** <p>This class contains information about one display window mode available.
    It also contains a static function to get a list of all available display modes.</p>
    <p>Code example which dumps a list over all available modes:</p>
    <pre>
    std::vector<CL_DisplayWindowMode> &modes = CL_DisplayWindowMode::get_display_modes();
    for(std::size_t i=0; i < modes.size(); ++i)
        std::cout << modes[i].get_string() << std::endl;
    </pre> */
class CL_API_DISPLAY CL_DisplayWindowMode
{
/** \name Construction */
//@{
public:

	/// \brief Constructs a DisplayWindowMode
	///
	/// \param res = Size
	/// \param bpp = value
	/// \param refresh_rate = value
	CL_DisplayWindowMode(const CL_Size &res, int bpp, int refresh_rate);

	/// \brief Constructs a DisplayWindowMode
	///
	/// \param copy = Display Window Mode
	CL_DisplayWindowMode(const CL_DisplayWindowMode &copy);

	virtual ~CL_DisplayWindowMode();
//@}

/** \name Statics */
//@{
public:
	/** \brief Returns a list over all available display modes. */
	static std::vector<CL_DisplayWindowMode> &get_display_modes();
//@}

/** \name Attributes */
//@{
public:
	/** \brief Returns the resolution of the displaymode. */
	CL_Size get_resolution() const;

	/** \brief Returns the numbers of bytes per pixel of the displaymode. */
	int get_bpp() const;

	/** \brief Returns the refresh rate of the displaymode. 
	Specifies the frequency, in hertz (cycles per second).
	This value is also known as the vertical refresh rate. */
	int get_refresh_rate() const;

	/** \brief Returns the information as a string in the format "640x480x32@60" */
	CL_String get_string() const;
//@}

/** \name Operations */
//@{
public:
	/** \brief Copy assignment operator. */
	CL_DisplayWindowMode &operator =(const CL_DisplayWindowMode &copy);
//@}

/** \name Implementation */
//@{
private:
	CL_DisplayWindowMode_Impl *impl;
//@}
};

#endif
//@}
