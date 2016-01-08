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

#include "Sound/precomp.h"
#include "API/Sound/SoundProviders/soundprovider_vorbis.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/path_help.h"
#include "soundprovider_vorbis_impl.h"
#include "soundprovider_vorbis_session.h"

namespace clan
{
	SoundProvider_Vorbis::SoundProvider_Vorbis(
		const std::string &filename,
		const FileSystem &fs,
		bool stream)
		: impl(std::make_shared<SoundProvider_Vorbis_Impl>())
	{
		IODevice input = fs.open_file(filename, File::open_existing, File::access_read, File::share_all);
		impl->load(input);
	}

	SoundProvider_Vorbis::SoundProvider_Vorbis(
		const std::string &fullname, bool stream)
		: impl(std::make_shared<SoundProvider_Vorbis_Impl>())
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		IODevice input = vfs.open_file(filename, File::open_existing, File::access_read, File::share_all);
		impl->load(input);
	}

	SoundProvider_Vorbis::SoundProvider_Vorbis(
		IODevice &file, bool stream)
		: impl(std::make_shared<SoundProvider_Vorbis_Impl>())
	{
		impl->load(file);
	}

	SoundProvider_Vorbis::~SoundProvider_Vorbis()
	{
	}

	SoundProvider_Session *SoundProvider_Vorbis::begin_session()
	{
		return new SoundProvider_Vorbis_Session(*this);
	}

	void SoundProvider_Vorbis::end_session(SoundProvider_Session *session)
	{
		delete session;
	}

	void SoundProvider_Vorbis_Impl::load(IODevice &input)
	{
		int size = input.get_size();
		buffer = DataBuffer(size);
		int bytes_read = input.read(buffer.get_data(), buffer.get_size());
		buffer.set_size(bytes_read);
	}
}
