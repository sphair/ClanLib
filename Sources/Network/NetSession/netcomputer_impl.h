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
*/

#pragma once


#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/weakptr.h"

class CL_NetSession_Impl;

class CL_NetComputer_Impl
{
/// \name Construction
/// \{

public:
	CL_NetComputer_Impl();

	~CL_NetComputer_Impl();


/// \}
/// \name Attributes
/// \{

public:
	CL_SocketName name;

	CL_WeakPtr<CL_NetSession_Impl> netsession;

	bool disconnected;

	CL_String disconnect_reason;


/// \}
/// \name Operations
/// \{

public:


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


