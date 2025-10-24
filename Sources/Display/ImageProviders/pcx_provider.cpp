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
**    (if your name is missing here, please add it)
*/

#include "Display/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/ImageProviders/pcx_provider.h"
#include "API/Core/System/exception.h"
#include "API/Core/IOData/virtual_directory.h"
#include "pcx_provider_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PCXProvider construction:

CL_PixelBuffer CL_PCXProvider::load(
	const CL_String &filename,
	const CL_VirtualDirectory &directory)
{
	CL_IODevice datafile = directory.open_file_read(filename);
	CL_PCXProvider_Impl pcx(datafile);
	return CL_PixelBuffer(pcx.width, pcx.height, pcx.sized_format, pcx.palette, pcx.get_data());
}

CL_PixelBuffer CL_PCXProvider::load(
	CL_IODevice &file)
{
	CL_PCXProvider_Impl pcx(file);
	return CL_PixelBuffer(pcx.width, pcx.height, pcx.sized_format, pcx.palette, pcx.get_data());
}

CL_PixelBuffer CL_PCXProvider::load(
	const CL_String &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	return CL_PCXProvider::load(filename, vfs.get_root_directory());
}

void CL_PCXProvider::save(
	CL_PixelBuffer buffer,
	CL_IODevice &file)
{
	throw CL_Exception("PCXProvider doesn't support saving");
}

void CL_PCXProvider::save(
	CL_PixelBuffer buffer,
	const CL_String &filename,
	CL_VirtualDirectory &directory)
{
	throw CL_Exception("PCXProvider doesn't support saving");
}

void CL_PCXProvider::save(
	CL_PixelBuffer buffer,
	const CL_String &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_VirtualDirectory dir = vfs.get_root_directory();
	CL_PCXProvider::save(buffer, filename, dir);
}
