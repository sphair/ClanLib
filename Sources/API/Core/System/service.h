/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once


#include "../api_core.h"
#include "../System/sharedptr.h"
#include "../Signals/signal_v1.h"

class CL_Event;
class CL_Service_Impl;

/// \brief Service/daemon class.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_Service
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a service object.
	CL_Service(const CL_String &service_name);

	~CL_Service();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the service name.
	const CL_String &get_service_name() const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Process command line and run service.
	int main(int argc, char **argv);

protected:
	/// \brief Called when the service is asked to start.
	virtual void service_start(std::vector<CL_String> &args) = 0;

	/// \brief Called when the service is asked to stop.
	virtual void service_stop() = 0;

	/// \brief Called when the service is asked to reload its configuration.
	virtual void service_reload() = 0;


/// \}
/// \name Implementation
/// \{

private:
	inline CL_Service(const CL_Service &copy) { }
	inline CL_Service &operator =(const CL_Service &) { return *this; }

	CL_SharedPtr<CL_Service_Impl> impl;

	friend class CL_Service_Impl;
/// \}
};


/// \}
