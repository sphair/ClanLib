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

#include "Core/precomp.h"
#include "API/Core/Zip/zip_writer.h"
#include "API/Core/Text/string_help.h"
#include "zip_archive_impl.h"
#include "zip_local_file_header.h"
#include "zip_compression_method.h"
#include "zip_file_header.h"
#include "zip_end_of_central_directory_record.h"
#include "zip_flags.h"
#include <zlib.h>

/////////////////////////////////////////////////////////////////////////////
// CL_ZipWriter_Impl class:

class CL_ZipWriter_Impl
{
public:
	CL_ZipWriter_Impl(CL_IODevice &output, bool storeFilenamesAsUTF8)
	: output(output), storeFilenamesAsUTF8(storeFilenamesAsUTF8), file_begun(false),
	  local_header_offset(0), uncompressed_length(0), compressed_length(0), compress(false)
	{
	}

	~CL_ZipWriter_Impl()
	{
		if (file_begun && compress)
		{
			deflateEnd(&zs);
		}
	}

	struct FileEntry
	{
		CL_ZipLocalFileHeader local_header;
		cl_int64 local_header_offset;
	};

	CL_IODevice output;
	bool storeFilenamesAsUTF8;
	bool file_begun;
	CL_ZipLocalFileHeader local_header;
	cl_int64 local_header_offset;
	cl_int64 uncompressed_length;
	cl_int64 compressed_length;
	cl_uint32 crc32;
	bool compress;
	z_stream zs;
	char zbuffer[16*1024];
	std::vector<FileEntry> written_files;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ZipWriter Construction:

CL_ZipWriter::CL_ZipWriter(CL_IODevice &output, bool storeFilenamesAsUTF8)
: impl(new CL_ZipWriter_Impl(output, storeFilenamesAsUTF8))
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ZipWriter Operations:

void CL_ZipWriter::begin_file(const CL_StringRef &filename, bool compress)
{
	if (impl->file_begun)
		throw CL_Exception("CL_ZipWriter already writing a file");
	impl->file_begun = true;

	impl->uncompressed_length = 0;
	impl->compressed_length = 0;
	impl->compress = compress;
	impl->crc32 = CL_ZIP_CRC_START_VALUE;

	impl->local_header_offset = impl->output.get_position();
	impl->local_header = CL_ZipLocalFileHeader();
	impl->local_header.version_needed_to_extract = 20;
	if (impl->storeFilenamesAsUTF8)
		impl->local_header.general_purpose_bit_flag = CL_ZIP_USE_UTF8;
	else
		impl->local_header.general_purpose_bit_flag = 0;
	impl->local_header.compression_method = compress ? zip_compress_deflate : zip_compress_store;
	CL_ZipArchive_Impl::calc_time_and_date(
		impl->local_header.last_mod_file_date,
		impl->local_header.last_mod_file_time);
	impl->local_header.crc32 = 0;
	impl->local_header.uncompressed_size = 0;
	impl->local_header.compressed_size = 0;
	impl->local_header.file_name_length = filename.length();
	impl->local_header.filename = filename;

	if (!impl->storeFilenamesAsUTF8) // Add UTF-8 as extra field if we aren't storing normal UTF-8 filenames
	{
		// -Info-ZIP Unicode Path Extra Field (0x7075)
		CL_String8 filename_cp437 = CL_StringHelp::text_to_cp437(filename);
		CL_String8 filename_utf8 = CL_StringHelp::text_to_utf8(filename);
		CL_DataBuffer unicode_path(9 + filename_utf8.length());
		cl_uint16 *extra_id = (cl_uint16 *) (unicode_path.get_data());
		cl_uint16 *extra_len = (cl_uint16 *) (unicode_path.get_data() + 2);
		cl_uint8 *extra_version = (cl_uint8 *) (unicode_path.get_data() + 4);
		cl_uint32 *extra_crc32 = (cl_uint32 *) (unicode_path.get_data() + 5);
		*extra_id = 0x7075;
		*extra_len = 5 + filename_utf8.length();
		*extra_version = 1;
		*extra_crc32 = CL_ZipArchive_Impl::calc_crc32(filename_cp437.data(), filename_cp437.size());
		memcpy(unicode_path.get_data() + 9, filename_utf8.data(), filename_utf8.length());
		impl->local_header.extra_field_length = unicode_path.get_size();
		impl->local_header.extra_field = unicode_path;
	}

	impl->local_header.save(impl->output);

	if (compress)
	{
		memset(&impl->zs, 0, sizeof(z_stream));
		int result = deflateInit2(&impl->zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY); // Undocumented: if wbits is negative, zlib skips header check
		if (result != Z_OK)
			throw CL_Exception("Zlib deflateInit failed for zip index!");
	}
}

void CL_ZipWriter::write_file_data(const void *data, cl_int64 size)
{
	if (!impl->file_begun)
		throw CL_Exception("CL_ZipWriter::begin_file not called prior CL_ZipWriter::write_file_data");

	impl->uncompressed_length += size;

	if (impl->compress)
	{
		impl->zs.next_in = (Bytef *) data;
		impl->zs.avail_in = size;

		while (impl->zs.avail_in > 0)
		{
			impl->zs.next_out = (Bytef *) impl->zbuffer;
			impl->zs.avail_out = 16*1024;
			int result = deflate(&impl->zs, Z_NO_FLUSH);
			if (result == Z_NEED_DICT) throw CL_Exception("Zlib deflate wants a dictionary!");
			if (result == Z_DATA_ERROR) throw CL_Exception("Zip data stream is corrupted");
			if (result == Z_STREAM_ERROR) throw CL_Exception("Zip stream structure was inconsistent!");
			if (result == Z_MEM_ERROR) throw CL_Exception("Zlib did not have enough memory to compress file!");
			if (result == Z_BUF_ERROR) throw CL_Exception("Not enough data in buffer when Z_FINISH was used");
			if (result != Z_OK) throw CL_Exception("Zlib deflate failed while compressing zip file!");

			cl_int64 zsize = 16*1024 - impl->zs.avail_out;
			if (zsize > 0)
			{
				impl->compressed_length += zsize;
				impl->output.write(impl->zbuffer, zsize);
			}
		}
	}
	else
	{
		impl->compressed_length += size;
		impl->output.write(data, size);
	}

	impl->crc32 = CL_ZipArchive_Impl::calc_crc32(data, size, impl->crc32, false);
}

void CL_ZipWriter::end_file()
{
	if (!impl->file_begun)
		return;

	if (impl->compress)
	{
		impl->zs.next_in = 0;
		impl->zs.avail_in = 0;

		while (true)
		{
			impl->zs.next_out = (Bytef *) impl->zbuffer;
			impl->zs.avail_out = 16*1024;
			int result = deflate(&impl->zs, Z_FINISH);
			if (result == Z_NEED_DICT) throw CL_Exception("Zlib deflate wants a dictionary!");
			if (result == Z_DATA_ERROR) throw CL_Exception("Zip data stream is corrupted");
			if (result == Z_STREAM_ERROR) throw CL_Exception("Zip stream structure was inconsistent!");
			if (result == Z_MEM_ERROR) throw CL_Exception("Zlib did not have enough memory to compress file!");
			if (result == Z_BUF_ERROR) throw CL_Exception("Not enough data in buffer when Z_FINISH was used");
			if (result != Z_OK && result != Z_STREAM_END) throw CL_Exception("Zlib deflate failed while compressing zip file!");
			cl_int64 zsize = 16*1024 - impl->zs.avail_out;
			if (zsize == 0)
				break;
			impl->output.write(impl->zbuffer, zsize);
			impl->compressed_length += zsize;

			if (result == Z_STREAM_END)
				break;
		}

		deflateEnd(&impl->zs);
		impl->compress = false;
	}

	impl->local_header.uncompressed_size = impl->uncompressed_length;
	impl->local_header.compressed_size = impl->compressed_length;
	impl->local_header.crc32 = CL_ZipArchive_Impl::calc_crc32(0, 0, impl->crc32, true);

	cl_int64 current_offset = impl->output.get_position();
	impl->output.seek(impl->local_header_offset);
	impl->local_header.save(impl->output);
	impl->output.seek(current_offset);

	CL_ZipWriter_Impl::FileEntry file_entry;
	file_entry.local_header = impl->local_header;
	file_entry.local_header_offset = impl->local_header_offset;
	impl->written_files.push_back(file_entry);

	impl->file_begun = false;
}

void CL_ZipWriter::write_toc()
{
	if (impl->file_begun)
		throw CL_Exception("Cannot write zip TOC when already writing a file entry");

	cl_int64 offset_start_central_dir = impl->output.get_position();

	// write central directory entries.
	std::vector<CL_ZipWriter_Impl::FileEntry>::size_type index;
	for (index = 0; index < impl->written_files.size(); index++)
	{
		CL_ZipFileHeader file_header;
		file_header.version_made_by = 20;
		file_header.version_needed_to_extract = impl->written_files[index].local_header.version_needed_to_extract;
		file_header.general_purpose_bit_flag = impl->written_files[index].local_header.general_purpose_bit_flag;
		file_header.compression_method = impl->written_files[index].local_header.compression_method;
		file_header.last_mod_file_time = impl->written_files[index].local_header.last_mod_file_time;
		file_header.last_mod_file_date = impl->written_files[index].local_header.last_mod_file_date;
		file_header.crc32 = impl->written_files[index].local_header.crc32;
		file_header.uncompressed_size = impl->written_files[index].local_header.uncompressed_size;
		file_header.compressed_size = impl->written_files[index].local_header.compressed_size;
		file_header.file_name_length = impl->written_files[index].local_header.file_name_length;
		file_header.filename = impl->written_files[index].local_header.filename;
		file_header.extra_field = impl->written_files[index].local_header.extra_field;
		file_header.extra_field_length = impl->written_files[index].local_header.extra_field_length;
		file_header.file_comment_length = 0;
		file_header.disk_number_start = 0;
		file_header.internal_file_attributes = 0;
		file_header.external_file_attributes = 0;
		file_header.relative_offset_of_local_header = impl->written_files[index].local_header_offset;
		file_header.save(impl->output);
	}
/*
	CL_ZipDigitalSignature digi_sign;
	digi_sign.size_of_data = 0;
	digi_sign.save(output);
*/
	cl_int64 central_dir_size = impl->output.get_position() - offset_start_central_dir;

	CL_ZipEndOfCentralDirectoryRecord central_dir_end;
	central_dir_end.number_of_this_disk = 0;
	central_dir_end.number_of_disk_with_start_of_central_directory = 0;
	central_dir_end.number_of_entries_on_this_disk = impl->written_files.size();
	central_dir_end.number_of_entries_in_central_directory = impl->written_files.size();
	central_dir_end.size_of_central_directory = central_dir_size;
	central_dir_end.offset_to_start_of_central_directory = offset_start_central_dir;
	central_dir_end.file_comment_length = 0;
	central_dir_end.file_comment = "";
	central_dir_end.save(impl->output);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ZipWriter Implementation:
