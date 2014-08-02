/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
*/


#pragma once


namespace clan
{
/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

class DisplayTarget;
class DisplayTargetProvider;

/// \brief Top level display class.
///
///    <p>The display class provides a static function interface to
///    DisplayWindow and other clanDisplay classes. It uses a selected
///    display window (by default the first created window) to call the
///    equalent functions in DisplayWindow, GraphicContext and such.</p>
///    <p>The entire point of this is to allow applications with only one
///    window to not pass around a pointer to the display window.</p>
class Display
{
/// \name Attributes
/// \{

public:
	/// \brief Returns the currently selected display target
	static DisplayTarget get_current_target();


/// \}
/// \name Operations
/// \{

public:
	/// \brief Sets the currently selected display target.
	static void set_current_target(const DisplayTarget &target);

	/// \brief Remove the target from the target list
	static void remove_target(const DisplayTarget &target);

/// \}
/// \name Signals
/// \{

public:

/// \}
};

}

/// \}
