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
*/

#include "Core/precomp.h"
#include "API/Core/System/service.h"
#include "service_impl.h"

namespace clan
{
	Service_Impl::Service_Impl(Service *service, const std::string &service_name)
		: service_name(service_name), service(service)
	{
		if (instance != nullptr)
			throw Exception("More than one instance of Service not allowed");
		instance = this;
	}

	Service_Impl::~Service_Impl()
	{
		instance = nullptr;
	}

	Service_Impl *Service_Impl::instance = nullptr;

	void Service_Impl::service_start(std::vector<std::string> &args)
	{
		service->service_start(args);
	}

	void Service_Impl::service_stop()
	{
		service->service_stop();
	}

	void Service_Impl::service_reload()
	{
		service->service_reload();
	}
}
