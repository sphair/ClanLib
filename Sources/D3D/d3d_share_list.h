/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include <memory>
#include <list>

namespace clan
{
	class D3DSharedResource;

	class D3DShareList
	{
	public:
		static void device_destroyed(ID3D11Device *device);
		static std::list<D3DSharedResource *>::iterator resource_created(D3DSharedResource *resource);
		static void resource_destroyed(std::list<D3DSharedResource *>::iterator it);

	private:
		static std::list<D3DSharedResource *> resources;
	};

	class D3DSharedResource
	{
	public:
		D3DSharedResource() { shared_it = D3DShareList::resource_created(this); }
		virtual ~D3DSharedResource() { D3DShareList::resource_destroyed(shared_it); }
		virtual void device_destroyed(ID3D11Device *device) = 0;

	private:
		std::list<D3DSharedResource *>::iterator shared_it;
	};
}
