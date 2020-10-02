/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Mark Page
*/

#include "Network/precomp.h"
#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif
#include "setupnetwork.h"
#include "API/Core/System/exception.h"
#include "../Core/System/setup_core.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{
	class SetupNetwork_Impl : public SetupModule
	{
	public:
		SetupNetwork_Impl();
		virtual ~SetupNetwork_Impl();
	};

	void SetupNetwork::start()
	{
		std::lock_guard<std::recursive_mutex> lock(SetupCore::instance.mutex);

		if (SetupCore::instance.module_network)
			return;

		SetupCore::instance.module_network = clan::make_unique<SetupNetwork_Impl>();

	}

	SetupNetwork_Impl::SetupNetwork_Impl()
	{
		WORD winsock_version = MAKEWORD(2, 2);
		WSADATA wsaData;
		int err = WSAStartup(winsock_version, &wsaData);
		if (err != 0)
			throw Exception("Failed to initialize winsockets");
	}

	SetupNetwork_Impl::~SetupNetwork_Impl()
	{
	}
}

