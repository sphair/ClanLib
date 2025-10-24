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

#ifndef header_input_device_linuxevent
#define header_input_device_linuxevent

#include "API/Core/Math/point.h"
#include "API/Core/System/keep_alive.h"
#include "../../Display/input_device_generic.h"
#include "../../Display/display_window_generic.h"

class CL_InputDevice_LinuxEvent : public CL_InputDevice_Generic,
											 public CL_KeepAlive
{
private: 
	// Absolute Input Event
	struct Axis
	{
		Axis(int code_, int min_, int max_, int pos_)
			: code(code_), min(min_), max(max_), pos(pos_) {}

		int code;
		int min;
		int max;
		int pos;

		inline float get_posf() const {
			return float(pos - min)/(max - min) * 2.0f - 1.0f;
		}
	};
	
	// Relative Input Event
	struct Ball 
	{
		Ball(int code_)
			: code(code_),
			  pos(0)
		{}
		
		int code;
		int pos;
	};

	// Key Input Event
	struct Button
	{
		Button(int code_)
			: code(code_),
			  pressed(false)
		{}

		int code;
		bool pressed;
	};

	CL_DisplayWindow_Generic* parent;

	int fd;
	int version;

	std::vector<Axis>   axes;
	std::vector<Button> buttons;
	std::vector<Ball>   balls;

	std::string name;
	std::string device;

public:
	CL_InputDevice_LinuxEvent(CL_DisplayWindow_Generic* parent, const std::string& device);
	~CL_InputDevice_LinuxEvent();

	std::string get_key_name(int num) const;

	float get_axis(int num)   const;
	bool  get_keycode(int num) const;

	std::string get_name() const;
	std::string get_device_name() const;

   int get_axis_count() const;
   int get_button_count() const;

	void keep_alive();	

private:
	int get_ball_index_by_code(int code);
	int get_button_index_by_code(int code);
	int get_axis_index_by_code(int code);
};

#endif 

// Local Variables: ***
// mode: clanlib ***
// End: ***
