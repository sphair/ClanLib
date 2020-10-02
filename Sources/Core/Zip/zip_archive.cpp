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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "API/Core/Zip/zip_archive.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/memory_device.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "zip_archive_impl.h"
#include "zip_file_header.h"
#include "zip_64_end_of_central_directory_record.h"
#include "zip_64_end_of_central_directory_locator.h"
#include "zip_end_of_central_directory_record.h"
#include "zip_file_entry_impl.h"
#include "zip_iodevice_fileentry.h"
#include "zip_compression_method.h"
#include "zip_digital_signature.h"
#include <ctime>
#include <mutex>

namespace clan
{
	ZipArchive::ZipArchive()
		: impl(std::make_shared<ZipArchive_Impl>())
	{
	}

	ZipArchive::ZipArchive(const std::string &filename)
		: impl(std::make_shared<ZipArchive_Impl>())
	{
		IODevice input = File(filename);
		impl->input = input;
		load(input);
	}

	ZipArchive::ZipArchive(IODevice &input)
		: impl(std::make_shared<ZipArchive_Impl>())
	{
		load(input);
	}

	ZipArchive::ZipArchive(const ZipArchive &copy) : impl(copy.impl)
	{
	}

	ZipArchive::~ZipArchive()
	{
	}

	std::vector<ZipFileEntry> ZipArchive::get_file_list()
	{
		return impl->files;
	}

	std::vector<ZipFileEntry> ZipArchive::get_file_list(const std::string &dirpath)
	{
		std::string path = dirpath;
		if (path.empty())
			path = "/";

		// Zip files expect the folders to be in the form of "/Folder/"
		path = PathHelp::make_absolute("/", path, PathHelp::path_type_virtual);
		path = PathHelp::add_trailing_slash(path, PathHelp::path_type_virtual);

		std::vector<ZipFileEntry> files;
		std::vector<std::string> added_directories;

		for (auto & elem : impl->files)
		{
			std::string filename = elem.get_archive_filename();
			if (filename[0] != '/')
			{
				filename.insert(filename.begin(), '/');
				elem.set_archive_filename(filename);
			}

			if (filename.substr(0, path.size()) == path)
			{
				std::string::size_type subdir_slash_pos = filename.find('/', path.size());

				if (subdir_slash_pos != std::string::npos) // subdirectory or files in a subdirectory
				{
					bool dir_added = false;
					std::string directory_name = filename.substr(path.size(), subdir_slash_pos - path.size());

					for (auto & added_directory : added_directories)
					{
						if (added_directory == directory_name)
							dir_added = true;
					}

					if (!dir_added)
					{
						ZipFileEntry dir_entry;
						dir_entry.set_archive_filename(directory_name);
						dir_entry.set_directory(true);
						files.push_back(dir_entry);

						added_directories.push_back(directory_name);
					}
				}
				else if (elem.get_archive_filename() != path)
				{
					std::string file_name = filename.substr(path.size(), std::string::npos);
					ZipFileEntry file_entry;
					file_entry.set_archive_filename(file_name);
					files.push_back(file_entry);
				}
			}
		}

		return files;
	}

	IODevice ZipArchive::open_file(const std::string &filename)
	{
		int size = impl->files.size();
		for (int i = 0; i < size; i++)
		{
			ZipFileEntry &entry = impl->files[i];

			std::string entry_filename = entry.impl->record.filename;
			if (entry_filename[0] == '/')
				entry_filename = entry_filename.substr(1, std::string::npos);

			if (entry_filename == filename)
			{
				switch (entry.impl->type)
				{
				case ZipFileEntry_Impl::type_file:
				{
					IODevice dupe = impl->input.duplicate();
					return IODevice(new ZipIODevice_FileEntry(dupe, entry));
				}

				case ZipFileEntry_Impl::type_removed:
					throw Exception(string_format("Unable to zip open file entry %1. The entry has been removed!", filename));
					break;

				case ZipFileEntry_Impl::type_added_memory:
					return MemoryDevice(entry.impl->data);

				case ZipFileEntry_Impl::type_added_file:
					return File(entry.impl->filename);
				}
				throw Exception(string_format("Unknown zip file entry type %1", filename));
			}
		}
		throw Exception(string_format("Unable to find zip index %1", filename));
	}

	std::string ZipArchive::get_pathname(const std::string &filename)
	{
		throw Exception("ZipArchive::get_pathname: function not implemented.");
	}

	IODevice ZipArchive::create_file(const std::string &filename, bool compress)
	{
		throw Exception("ZipArchive::create_file: function not implemented.");
	}

	void ZipArchive::add_file(const std::string &input_filename, const std::string &archive_filename)
	{
		ZipFileEntry file_entry;
		file_entry.set_input_filename(input_filename);
		file_entry.set_archive_filename(archive_filename);
		impl->files.push_back(file_entry);
	}

	void ZipArchive::save()
	{
		throw Exception("ZipArchive::save: function not implemented.");
	}

	void ZipArchive::save(const std::string &filename)
	{
		File output(filename, File::create_always, File::access_read_write);

		std::vector<int> local_header_offsets;
		std::vector<uint32_t> crc32_codes;

		int16_t dos_date = 0, dos_time = 0;
		ZipArchive_Impl::calc_time_and_date(dos_date, dos_time);

		std::vector<ZipFileEntry>::iterator it;
		for (it = impl->files.begin(); it != impl->files.end(); ++it)
		{
			local_header_offsets.push_back(output.get_position());

			std::string input_filename = (*it).get_input_filename();
			std::string archive_filename = (*it).get_archive_filename();
			File input(input_filename);

			DataBuffer data(input.get_size());
			input.read(data.get_data(), data.get_size());

			uint32_t crc32 = ZipArchive_Impl::calc_crc32(data.get_data(), data.get_size());
			crc32_codes.push_back(crc32);

			// 1. local file header
			ZipLocalFileHeader local_header;
			local_header.version_needed_to_extract = 20;
			local_header.general_purpose_bit_flag = ZIP_USE_UTF8;
			local_header.compression_method = zip_compress_store;
			local_header.last_mod_file_time = dos_time;
			local_header.last_mod_file_date = dos_date;
			local_header.crc32 = crc32;
			local_header.uncompressed_size = data.get_size();
			local_header.compressed_size = data.get_size();
			local_header.file_name_length = archive_filename.size();
			local_header.extra_field_length = 0;
			local_header.filename = archive_filename;
			local_header.extra_field = DataBuffer();
			local_header.save(output);

			// 2. Data (uncompressed for now)
			output.write(data.get_data(), data.get_size());
		}

		int index = 0;
		int offset_start_central_dir = output.get_position();

		// write central directory entries.
		for (it = impl->files.begin(); it != impl->files.end(); ++it)
		{
			std::string input_filename = (*it).get_input_filename();
			std::string filename = (*it).get_archive_filename();
			File input(input_filename);

			(*it).impl->record.version_made_by = 20;
			(*it).impl->record.version_needed_to_extract = 20;
			(*it).impl->record.general_purpose_bit_flag = ZIP_USE_UTF8;
			(*it).impl->record.compression_method = zip_compress_store;
			(*it).impl->record.last_mod_file_time = dos_time;
			(*it).impl->record.last_mod_file_date = dos_date;
			(*it).impl->record.crc32 = crc32_codes[index];
			(*it).impl->record.uncompressed_size = input.get_size();
			(*it).impl->record.compressed_size = input.get_size(); // todo
			(*it).impl->record.file_name_length = filename.size();
			(*it).impl->record.extra_field_length = 0;
			(*it).impl->record.filename = filename;
			(*it).impl->record.extra_field = DataBuffer();
			(*it).impl->record.file_comment_length = 0;
			(*it).impl->record.disk_number_start = 0;
			(*it).impl->record.internal_file_attributes = 0;
			(*it).impl->record.external_file_attributes = 0;
			(*it).impl->record.relative_offset_of_local_header = local_header_offsets[index];
			(*it).impl->record.save(output);
			index++;
		}

		/*	ZipDigitalSignature digi_sign;
			digi_sign.size_of_data = 0;
			digi_sign.save(output);*/

		int central_dir_size = output.get_position() - offset_start_central_dir;

		ZipEndOfCentralDirectoryRecord central_dir_end;

		central_dir_end.number_of_this_disk = 0;
		central_dir_end.number_of_disk_with_start_of_central_directory = 0;
		central_dir_end.number_of_entries_on_this_disk = local_header_offsets.size();
		central_dir_end.number_of_entries_in_central_directory = local_header_offsets.size();
		central_dir_end.size_of_central_directory = central_dir_size;
		central_dir_end.offset_to_start_of_central_directory = offset_start_central_dir;
		central_dir_end.file_comment_length = 0;
		central_dir_end.file_comment = "";
		central_dir_end.save(output);
	}

	void ZipArchive::load(IODevice &input)
	{
		impl->input = input;
		// Load zip file structures:

		// indicate the file is little-endian
		input.set_little_endian_mode();

		// Find end of central directory record:

		int size_file = input.get_size();

		char buffer[32 * 1024];
		if (size_file > 32 * 1024) input.seek(-32 * 1024, IODevice::SeekMode::end);
		int size_buffer = input.read(buffer, 32 * 1024);

		int end_record_pos = -1;
		for (int pos = size_buffer - 4; pos >= 0; pos--)
		{
#ifdef USE_BIG_ENDIAN
			if (*(unsigned int *)(buffer+pos) == 0x504b0506)
#else
			if (*(unsigned int *)(buffer + pos) == 0x06054b50)
#endif		
			{
				end_record_pos = size_file - size_buffer + pos;
				break;
			}
		}
		if (end_record_pos == -1)
		{
			throw Exception("This appear not to be a zip file");
		}

		// Load end of central directory record:

		ZipEndOfCentralDirectoryRecord end_of_directory;
		input.seek(end_record_pos, IODevice::SeekMode::set);
		end_of_directory.load(input);

		// Look for zip64 central directory locator:

		bool zip64 = false;
		Zip64EndOfCentralDirectoryLocator zip64_locator;
		Zip64EndOfCentralDirectoryRecord zip64_end_of_directory;

		int end64_locator = end_record_pos - 20;
		input.seek(end64_locator, IODevice::SeekMode::set);
		if (input.read_uint32() == 0x07064b50)
		{
			// Load zip64 structures:

			input.seek(end64_locator, IODevice::SeekMode::set);
			zip64_locator.load(input);

			input.seek(int(end64_locator + zip64_locator.relative_offset_of_zip64_end_of_central_directory), IODevice::SeekMode::set);
			zip64_end_of_directory.load(input);

			zip64 = true;
		}

		// Load central directory records:

		if (zip64) input.seek(int(zip64_end_of_directory.offset_to_start_of_central_directory), IODevice::SeekMode::set);
		else input.seek(int(end_of_directory.offset_to_start_of_central_directory), IODevice::SeekMode::set);

		int64_t num_entries = end_of_directory.number_of_entries_in_central_directory;
		if (zip64) num_entries = zip64_end_of_directory.number_of_entries_in_central_directory;

		for (int i = 0; i < num_entries; i++)
		{
			ZipFileEntry entry;
			entry.impl->record.load(input);
			impl->files.push_back(entry);
		}
	}

	/////////////////////////////////////////////////////////////////////////////

	void ZipArchive_Impl::calc_time_and_date(int16_t &out_date, int16_t &out_time)
	{
		uint32_t day_of_month = 0;
		uint32_t month = 0;
		uint32_t year_from_1980 = 0;
		uint32_t hour = 0;
		uint32_t min = 0;
		uint32_t sec = 0;

#if _MSC_VER >= 1400
		time_t t = time(0);
		tm tm_time;
		localtime_s(&tm_time, &t);
		day_of_month = tm_time.tm_mday;
		month = tm_time.tm_mon + 1;
		year_from_1980 = tm_time.tm_year - 80;
		hour = tm_time.tm_hour;
		min = tm_time.tm_min;
		sec = tm_time.tm_sec;
#else
		static std::recursive_mutex mutex;
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
		time_t t = time(nullptr);
		tm *tm_time = gmtime(&t);
		day_of_month = tm_time->tm_mday;
		month = tm_time->tm_mon + 1;
		year_from_1980 = tm_time->tm_year - 80;
		hour = tm_time->tm_hour;
		min = tm_time->tm_min;
		sec = tm_time->tm_sec;
		mutex_lock.unlock();
#endif

		out_date = (int16_t)(day_of_month + (month << 5) + (year_from_1980 << 9));
		out_time = (int16_t)(sec / 2 + (min << 5) + (hour << 11));
	}

	uint32_t ZipArchive_Impl::calc_crc32(const void *data, int64_t size, uint32_t crc, bool last_block)
	{
		const char *d = (const char *)data;
		for (uint32_t data_index = 0; data_index < size; data_index++)
		{
			uint8_t table_index = ((crc ^ d[data_index]) & 0xff);
			crc = ((crc >> 8) & 0x00ffffff) ^ crc32_table[table_index];
		}
		if (last_block)
			return ~crc;
		else
			return crc;
	}

	uint32_t ZipArchive_Impl::crc32_table[256] =
	{
		0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
		0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
		0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
		0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
		0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
		0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
		0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
		0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
		0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
		0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
		0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
		0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
		0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
		0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
		0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
		0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
		0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
		0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
		0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
		0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
		0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
		0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
		0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
		0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
		0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
		0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
		0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
		0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
		0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
		0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
		0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
		0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
		0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
		0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
		0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
		0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
		0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
		0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
		0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
		0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
		0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
		0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
		0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
		0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
		0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
		0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
		0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
		0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
		0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
		0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
		0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
		0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
		0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
		0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
		0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
		0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
		0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
		0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
		0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
		0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
		0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
		0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
		0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
		0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
	};
}
