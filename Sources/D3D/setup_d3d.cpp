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
**    Mark Page
*/

#include "D3D/precomp.h"
#include "setup_d3d_impl.h"
#include "API/D3D/setup_d3d.h"
#include "API/D3D/d3d_target.h"

//#pragma comment(lib, "D3D11.lib")
//#pragma comment(lib, "D3dcompiler.lib")

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SetupSWRender Construction:

Mutex SetupD3D_Impl::cl_d3d_mutex;
int SetupD3D_Impl::cl_d3d_refcount = 0;
D3DTarget *SetupD3D_Impl::cl_d3d_target = 0;

SetupD3D::SetupD3D()
{
	SetupD3D_Impl::init();

}

SetupD3D::~SetupD3D()
{
	SetupD3D_Impl::deinit();
}

void SetupD3D_Impl::init()
{
	MutexSection mutex_lock(&SetupD3D_Impl::cl_d3d_mutex);
	if (SetupD3D_Impl::cl_d3d_refcount == 0)
		SetupD3D_Impl::cl_d3d_target = new D3DTarget();
	SetupD3D_Impl::cl_d3d_refcount++;
}

void SetupD3D_Impl::deinit()
{
	MutexSection mutex_lock(&SetupD3D_Impl::cl_d3d_mutex);
	SetupD3D_Impl::cl_d3d_refcount--;
	if (SetupD3D_Impl::cl_d3d_refcount == 0)
		delete SetupD3D_Impl::cl_d3d_target;
}


}
