/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "precomp.h"
#include "soundprovider_mikmod_impl.h"
#include "soundprovider_mikmod_session.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod construction:
CL_SoundProvider_MikMod::CL_SoundProvider_MikMod(
	const std::string &filename,
	const clan::VirtualDirectory &directory,
	bool stream)
: impl(new CL_SoundProvider_MikMod_Impl)
{
	clan::VirtualDirectory new_directory = directory;
	clan::IODevice input = new_directory.open_file(filename, clan::File::open_existing, clan::File::access_read, clan::File::share_all);
	impl->load(input);
}

CL_SoundProvider_MikMod::CL_SoundProvider_MikMod(
	const std::string &fullname, bool stream)
: impl(new CL_SoundProvider_MikMod_Impl)
{
	std::string path = clan::PathHelp::get_fullpath(fullname, clan::PathHelp::path_type_file);
	std::string filename = clan::PathHelp::get_filename(fullname, clan::PathHelp::path_type_file);
	clan::VirtualFileSystem vfs(path);
	clan::VirtualDirectory dir = vfs.get_root_directory();
	clan::IODevice input = dir.open_file(filename, clan::File::open_existing, clan::File::access_read, clan::File::share_all);
	impl->load(input);
}

CL_SoundProvider_MikMod::CL_SoundProvider_MikMod(
	clan::IODevice &file, bool stream)
: impl(new CL_SoundProvider_MikMod_Impl)
{
	impl->load(file);
}

CL_SoundProvider_MikMod::~CL_SoundProvider_MikMod()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod operations:

clan::SoundProvider_Session *CL_SoundProvider_MikMod::begin_session()
{
	return new CL_SoundProvider_MikMod_Session(*this);
}

void CL_SoundProvider_MikMod::end_session(clan::SoundProvider_Session *session)
{
	delete session;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod implementation:

void CL_SoundProvider_MikMod_Impl::load(clan::IODevice &input)
{
	int size = input.get_size();
	buffer = clan::DataBuffer(size);
	int bytes_read = input.read(buffer.get_data(), buffer.get_size());
	buffer.set_size(bytes_read);
}
