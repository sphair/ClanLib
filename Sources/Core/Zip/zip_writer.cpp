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

#include "Core/precomp.h"
#include "API/Core/Zip/zip_writer.h"
#include "API/Core/Text/string_help.h"
#include "zip_archive_impl.h"
#include "zip_local_file_header.h"
#include "zip_compression_method.h"
#include "zip_file_header.h"
#include "zip_end_of_central_directory_record.h"
#include "zip_flags.h"
#include "Core/Zip/miniz.h"

namespace clan
{
	class ZipWriter_Impl
	{
	public:
		ZipWriter_Impl(IODevice &output, bool storeFilenamesAsUTF8)
			: output(output), storeFilenamesAsUTF8(storeFilenamesAsUTF8), file_begun(false),
			local_header_offset(0), uncompressed_length(0), compressed_length(0), compress(false)
		{
		}

		~ZipWriter_Impl()
		{
			if (file_begun && compress)
			{
				mz_deflateEnd(&zs);
			}
		}

		struct FileEntry
		{
			ZipLocalFileHeader local_header;
			int64_t local_header_offset;
		};

		IODevice output;
		bool storeFilenamesAsUTF8;
		bool file_begun;
		ZipLocalFileHeader local_header;
		int64_t local_header_offset;
		int64_t uncompressed_length;
		int64_t compressed_length;
		uint32_t crc32;
		bool compress;
		mz_stream zs;
		char zbuffer[16 * 1024];
		std::vector<FileEntry> written_files;
	};

	ZipWriter::ZipWriter(IODevice &output, bool storeFilenamesAsUTF8)
		: impl(std::make_shared<ZipWriter_Impl>(output, storeFilenamesAsUTF8))
	{
	}

	void ZipWriter::begin_file(const std::string &filename, bool compress)
	{
		if (impl->file_begun)
			throw Exception("ZipWriter already writing a file");
		impl->file_begun = true;

		impl->uncompressed_length = 0;
		impl->compressed_length = 0;
		impl->compress = compress;
		impl->crc32 = ZIP_CRC_START_VALUE;

		impl->local_header_offset = impl->output.get_position();
		impl->local_header = ZipLocalFileHeader();
		impl->local_header.version_needed_to_extract = 20;
		if (impl->storeFilenamesAsUTF8)
			impl->local_header.general_purpose_bit_flag = ZIP_USE_UTF8;
		else
			impl->local_header.general_purpose_bit_flag = 0;
		impl->local_header.compression_method = compress ? zip_compress_deflate : zip_compress_store;
		ZipArchive_Impl::calc_time_and_date(
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
			std::string filename_cp437 = StringHelp::text_to_cp437(filename);
			std::string filename_utf8 = filename;
			DataBuffer unicode_path(9 + filename_utf8.length());
			uint16_t *extra_id = (uint16_t *)(unicode_path.get_data());
			uint16_t *extra_len = (uint16_t *)(unicode_path.get_data() + 2);
			uint8_t *extra_version = (uint8_t *)(unicode_path.get_data() + 4);
			uint32_t *extra_crc32 = (uint32_t *)(unicode_path.get_data() + 5);
			*extra_id = 0x7075;
			*extra_len = 5 + filename_utf8.length();
			*extra_version = 1;
			*extra_crc32 = ZipArchive_Impl::calc_crc32(filename_cp437.data(), filename_cp437.size());
			memcpy(unicode_path.get_data() + 9, filename_utf8.data(), filename_utf8.length());
			impl->local_header.extra_field_length = unicode_path.get_size();
			impl->local_header.extra_field = unicode_path;
		}

		impl->local_header.save(impl->output);

		if (compress)
		{
			memset(&impl->zs, 0, sizeof(mz_stream));
			int result = mz_deflateInit2(&impl->zs, MZ_DEFAULT_COMPRESSION, MZ_DEFLATED, -15, 8, MZ_DEFAULT_STRATEGY); // Undocumented: if wbits is negative, zlib skips header check
			if (result != MZ_OK)
				throw Exception("Zlib deflateInit failed for zip index!");
		}
	}

	void ZipWriter::write_file_data(const void *data, int64_t size)
	{
		if (!impl->file_begun)
			throw Exception("ZipWriter::begin_file not called prior ZipWriter::write_file_data");

		impl->uncompressed_length += size;

		if (impl->compress)
		{
			impl->zs.next_in = (unsigned char *)data;
			impl->zs.avail_in = size;

			while (impl->zs.avail_in > 0)
			{
				impl->zs.next_out = (unsigned char *)impl->zbuffer;
				impl->zs.avail_out = 16 * 1024;
				int result = mz_deflate(&impl->zs, MZ_NO_FLUSH);
				if (result == MZ_NEED_DICT) throw Exception("Zlib deflate wants a dictionary!");
				if (result == MZ_DATA_ERROR) throw Exception("Zip data stream is corrupted");
				if (result == MZ_STREAM_ERROR) throw Exception("Zip stream structure was inconsistent!");
				if (result == MZ_MEM_ERROR) throw Exception("Zlib did not have enough memory to compress file!");
				if (result == MZ_BUF_ERROR) throw Exception("Not enough data in buffer when Z_FINISH was used");
				if (result != MZ_OK) throw Exception("Zlib deflate failed while compressing zip file!");

				int64_t zsize = 16 * 1024 - impl->zs.avail_out;
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

		impl->crc32 = ZipArchive_Impl::calc_crc32(data, size, impl->crc32, false);
	}

	void ZipWriter::end_file()
	{
		if (!impl->file_begun)
			return;

		if (impl->compress)
		{
			impl->zs.next_in = nullptr;
			impl->zs.avail_in = 0;

			while (true)
			{
				impl->zs.next_out = (unsigned char *)impl->zbuffer;
				impl->zs.avail_out = 16 * 1024;
				int result = mz_deflate(&impl->zs, MZ_FINISH);
				if (result == MZ_NEED_DICT) throw Exception("Zlib deflate wants a dictionary!");
				if (result == MZ_DATA_ERROR) throw Exception("Zip data stream is corrupted");
				if (result == MZ_STREAM_ERROR) throw Exception("Zip stream structure was inconsistent!");
				if (result == MZ_MEM_ERROR) throw Exception("Zlib did not have enough memory to compress file!");
				if (result == MZ_BUF_ERROR) throw Exception("Not enough data in buffer when Z_FINISH was used");
				if (result != MZ_OK && result != MZ_STREAM_END) throw Exception("Zlib deflate failed while compressing zip file!");
				int64_t zsize = 16 * 1024 - impl->zs.avail_out;
				if (zsize == 0)
					break;
				impl->output.write(impl->zbuffer, zsize);
				impl->compressed_length += zsize;

				if (result == MZ_STREAM_END)
					break;
			}

			mz_deflateEnd(&impl->zs);
			impl->compress = false;
		}

		impl->local_header.uncompressed_size = impl->uncompressed_length;
		impl->local_header.compressed_size = impl->compressed_length;
		impl->local_header.crc32 = ZipArchive_Impl::calc_crc32(nullptr, 0, impl->crc32, true);

		int64_t current_offset = impl->output.get_position();
		impl->output.seek(impl->local_header_offset);
		impl->local_header.save(impl->output);
		impl->output.seek(current_offset);

		ZipWriter_Impl::FileEntry file_entry;
		file_entry.local_header = impl->local_header;
		file_entry.local_header_offset = impl->local_header_offset;
		impl->written_files.push_back(file_entry);

		impl->file_begun = false;
	}

	void ZipWriter::write_toc()
	{
		if (impl->file_begun)
			throw Exception("Cannot write zip TOC when already writing a file entry");

		int64_t offset_start_central_dir = impl->output.get_position();

		// write central directory entries.
		std::vector<ZipWriter_Impl::FileEntry>::size_type index;
		for (index = 0; index < impl->written_files.size(); index++)
		{
			ZipFileHeader file_header;
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
			ZipDigitalSignature digi_sign;
			digi_sign.size_of_data = 0;
			digi_sign.save(output);
			*/
		int64_t central_dir_size = impl->output.get_position() - offset_start_central_dir;

		ZipEndOfCentralDirectoryRecord central_dir_end;
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
}
