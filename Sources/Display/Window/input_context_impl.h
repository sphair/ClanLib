/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Harry Storbacka
*/

#pragma once

#include <vector>
#include "API/Display/Window/input_device.h"
#include "API/Display/Window/input_event.h"
#include "API/Core/System/disposable_object.h"
#include "API/Core/System/mutex.h"

namespace clan
{

class InputEvent;

/// \brief Input events interface.
class InputContext_Impl : public DisposableObject
{
/// \name Construction
/// \{

public:
	InputContext_Impl();

	~InputContext_Impl();


/// \}
/// \name Attributes
/// \{

public:
	static Mutex mutex;

	std::vector<InputDevice> keyboards;

	std::vector<InputDevice> mice;

	std::vector<InputDevice> joysticks;

	std::vector<InputDevice> tablets;
	
	std::weak_ptr<InputContext_Impl> input_context;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Removes all devices from this input context.
	void clear();

	/// \brief Adds a new keyboard to the input context.
	void add_keyboard(InputDevice &keyboard);

	/// \brief Adds a new mouse to the input context.
	void add_mouse(InputDevice &mouse);

	/// \brief Adds a new joystick to the input context.
	void add_joystick(InputDevice &joystick);

	/// \brief Adds a new tablet to the input context.
	void add_tablet(InputDevice &tablet);

	/// \brief Process and dispatch messages to signals.
	void process_messages();

	/// \brief Called by input devices when events are received.
	void received_event(const InputEvent &e, std::weak_ptr<InputDevice_Impl> &input_device);

	/// \brief Update device
	///
	/// \param peek_only Treat as a request to see if an event would occur
	///
	/// \return true when the device event has occurred
	bool poll(bool peek_only);


/// \}
/// \name Implementation
/// \{

private:
	void on_dispose();

	/// \brief Update a specific device
	///
	/// \param peek_only Treat as a request to see if an event would occur
	/// \param device = The device
	/// \param message_flag = Set to true when an event has occurred
	void poll_device( bool peek_only, std::vector<InputDevice> &device, bool &message_flag);

	std::vector< std::pair<InputEvent, std::weak_ptr<InputDevice_Impl> > > events;
/// \}

};

}
