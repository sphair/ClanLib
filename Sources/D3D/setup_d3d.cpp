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

#include "D3D/precomp.h"
#include "setup_d3d_impl.h"
#include "setup_d3d.h"
#include "API/D3D/d3d_target.h"
#include <mutex>
#include "../Display/setup_display.h"
#include "API/Core/Math/cl_math.h"

//#pragma comment(lib, "D3D11.lib")
//#pragma comment(lib, "D3dcompiler.lib")

namespace clan
{
	SetupD3D_Impl *SetupD3D_Impl::instance = nullptr;

	void SetupD3D::start()
	{
		std::lock_guard<std::recursive_mutex> lock(SetupCore::instance.mutex);

		if (SetupCore::instance.module_d3d)
			return;

		SetupDisplay::start();	// D3D depends on display
		SetupCore::instance.module_d3d = clan::make_unique<SetupD3D_Impl>();
	}

	SetupD3D_Impl::SetupD3D_Impl()
	{
		instance = this;
	}

	SetupD3D_Impl::~SetupD3D_Impl()
	{
		instance = nullptr;
	}
}
