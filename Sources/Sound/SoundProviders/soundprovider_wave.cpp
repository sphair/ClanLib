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
*/

#include "Sound/precomp.h"
#include "API/Sound/SoundProviders/soundprovider_wave.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/logger.h"
#include "soundprovider_wave_impl.h"
#include "soundprovider_wave_session.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave construction:

CL_SoundProvider_Wave::CL_SoundProvider_Wave(
	const CL_String &filename,
	const CL_VirtualDirectory &virtual_directory,
	bool stream) : impl(new CL_SoundProvider_Wave_Impl)
{
	CL_VirtualDirectory new_directory = virtual_directory;
	CL_IODevice source = new_directory.open_file(filename, CL_File::open_existing, CL_File::access_read, CL_File::share_read);
	impl->load(source);
}

CL_SoundProvider_Wave::CL_SoundProvider_Wave(
	const CL_String &fullname, bool stream)
: impl(new CL_SoundProvider_Wave_Impl)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_VirtualDirectory dir = vfs.get_root_directory();
	CL_IODevice input = dir.open_file(filename, CL_File::open_existing, CL_File::access_read, CL_File::share_all);
	impl->load(input);
}

CL_SoundProvider_Wave::CL_SoundProvider_Wave(
	CL_IODevice &file, bool stream)
: impl(new CL_SoundProvider_Wave_Impl)
{
	impl->load(file);
}

CL_SoundProvider_Wave::~CL_SoundProvider_Wave()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave operations:

CL_SoundProvider_Session *CL_SoundProvider_Wave::begin_session()
{
	return new CL_SoundProvider_Wave_Session(*this);
}

void CL_SoundProvider_Wave::end_session(CL_SoundProvider_Session *session)
{
	delete session;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave implementation:

void CL_SoundProvider_Wave_Impl::load(CL_IODevice &source)
{
	source.set_little_endian_mode();

	char chunk_id[4];
	source.read(chunk_id, 4);
	if (memcmp(chunk_id, "RIFF", 4))
		throw CL_Exception("Expected RIFF header!");
	cl_uint32 chunk_size = source.read_uint32();

	char format_id[4];
	source.read(format_id, 4);
	if (memcmp(format_id, "WAVE", 4))
		throw CL_Exception("Expected WAVE header!");

	cl_uint32 subchunk_pos = source.get_position();
	cl_uint32 subchunk1_size = find_subchunk("fmt ", source, subchunk_pos, chunk_size);

	cl_uint16 audio_format = source.read_uint16();
	num_channels = source.read_uint16();
	frequency = source.read_uint32();
	cl_uint32 byte_rate = source.read_uint32();
	cl_uint16 block_align = source.read_uint16();
	cl_uint16 bits_per_sample = source.read_uint16();

	if (bits_per_sample == 16)
		format = sf_16bit_signed;
	else if (bits_per_sample == 8)
		format = sf_8bit_unsigned;
	else
		throw CL_Exception("Unsupported wave sample format");

	cl_uint32 subchunk2_size = find_subchunk("data", source, subchunk_pos, chunk_size);

	data = new char[subchunk2_size];
	source.read(data, subchunk2_size);

	num_samples = subchunk2_size / block_align;
}

unsigned int CL_SoundProvider_Wave_Impl::find_subchunk(const char *chunk, CL_IODevice &source, unsigned int file_offset, unsigned int max_offset )
{
	char subchunk1_id[4];

	max_offset -= 8;	// Each subchunk must contains at least name and size
	while(file_offset < max_offset)
	{
		source.seek(file_offset);
		source.read(subchunk1_id, 4);
		cl_uint32 subchunk1_size = source.read_uint32();
		if (!memcmp(subchunk1_id, chunk, 4))
		{
			// Found chunk
			return subchunk1_size;
		}
		file_offset += subchunk1_size + 8;
	}
	throw CL_Exception("Block not found!");

}
