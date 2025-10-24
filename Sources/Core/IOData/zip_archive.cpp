/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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

#include "Core/precomp.h"
#include "API/Core/IOData/zip_archive.h"
#include "API/Core/IOData/inputsource_file.h"
#include "API/Core/IOData/outputsource_file.h"
#include "API/Core/IOData/inputsource_memory.h"
#include "API/Core/System/clanstring.h"
#include "zip_archive_generic.h"
#include "zip_file_header.h"
#include "zip_64_end_of_central_directory_record.h"
#include "zip_64_end_of_central_directory_locator.h"
#include "zip_end_of_central_directory_record.h"
#include "zip_file_entry_generic.h"
#include "inputsource_zip_fileentry.h"
#include "zip_local_file_descriptor.h"
#include "zip_compression_method.h"
#include <time.h>
#include "zip_flags.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_Archive construction:

CL_Zip_Archive::CL_Zip_Archive() : impl(new CL_Zip_Archive_Generic)
{
}
	
CL_Zip_Archive::CL_Zip_Archive(const std::string &filename) : impl(new CL_Zip_Archive_Generic)
{
	impl->filename = filename;

	// Load zip file structures:

	CL_InputSource *input = new CL_InputSource_File(filename);
	
	// indicate the file is little-endian
	input->set_little_endian_mode();
	
	// Find end of central directory record:

	int size_file = input->size();
	
	char buffer[32*1024];
	if (size_file > 32*1024) input->seek(-32*1024, CL_InputSource::seek_end);
	int size_buffer = input->read(buffer, 32*1024);
	
	int end_record_pos = -1;
	for (int pos = size_buffer-4; pos >= 0; pos--)
	{
		if( buffer[pos] == 0x50 && buffer[pos+1] == 0x4b && buffer[pos+2] == 0x05  && buffer[pos+3] == 0x06 )
		{
			end_record_pos = size_file-size_buffer+pos;
			input->tell();
			break;
		}
	}
	if (end_record_pos == -1)
	{
		delete input;
		throw CL_Error("This appear not to be a zip file");
	}
	
	// Load end of central directory record:
	
	CL_Zip_EndOfCentralDirectoryRecord end_of_directory;
	input->seek(end_record_pos, CL_InputSource::seek_set);
	end_of_directory.load(input);
	
	// Look for zip64 central directory locator:
	
	bool zip64 = false;
	CL_Zip_64EndOfCentralDirectoryLocator zip64_locator;
	CL_Zip_64EndOfCentralDirectoryRecord zip64_end_of_directory;
	
	int end64_locator = end_record_pos-20;
	input->seek(end64_locator, CL_InputSource::seek_set);
	if (input->read_uint32() == 0x07064b50)
	{
		// Load zip64 structures:
		
		input->seek(end64_locator, CL_InputSource::seek_set);
		zip64_locator.load(input);
		
		input->seek(end64_locator+zip64_locator.relative_offset_of_zip64_end_of_central_directory, CL_InputSource::seek_set);
		zip64_end_of_directory.load(input);
		
		zip64 = true;
	}
	
	// Load central directory records:
	
	if (zip64) input->seek(zip64_end_of_directory.offset_to_start_of_central_directory, CL_InputSource::seek_set);
	else input->seek(end_of_directory.offset_to_start_of_central_directory, CL_InputSource::seek_set);
	
	cl_int64 num_entries = end_of_directory.number_of_entries_in_central_directory;
	if (zip64) num_entries = zip64_end_of_directory.number_of_entries_in_central_directory;

	for (int i=0; i<num_entries; i++)
	{
		CL_Zip_FileEntry entry;
		entry.impl->record.load(input);
		impl->files.push_back(entry);
	}

	delete input;
}

CL_Zip_Archive::CL_Zip_Archive(const CL_Zip_Archive &copy) : impl(copy.impl)
{
}
	
CL_Zip_Archive::~CL_Zip_Archive()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_Archive attributes:

std::vector<CL_Zip_FileEntry> &CL_Zip_Archive::get_file_list()
{
	return impl->files;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_Archive operations:

CL_InputSource *CL_Zip_Archive::open_source(const std::string &filename)
{
	int size = impl->files.size();
	for (int i=0; i<size; i++)
	{
		CL_Zip_FileEntry &entry = impl->files[i];
		if (entry.impl->record.filename == filename)
		{
			switch (entry.impl->type)
			{
			case CL_Zip_FileEntry_Generic::type_file:
				return new CL_InputSource_Zip_FileEntry(impl->filename, entry);

			case CL_Zip_FileEntry_Generic::type_removed:
				break;

			case CL_Zip_FileEntry_Generic::type_added_memory:
				return new CL_InputSource_Memory(entry.impl->data);

			case CL_Zip_FileEntry_Generic::type_added_file:
				return new CL_InputSource_File(entry.impl->filename);
			}
			throw CL_Error(CL_String::format("Found but hates %1", filename));
		}
	}
	throw CL_Error(CL_String::format("Unable to find zip index %1", filename));
	return 0;
}

CL_InputSourceProvider *CL_Zip_Archive::clone()
{
	return new CL_Zip_Archive(*this);
}

std::string CL_Zip_Archive::get_pathname(const std::string &filename)
{
	return filename;
}

CL_InputSourceProvider *CL_Zip_Archive::create_relative(const std::string &path)
{
	// This is wrong. Should seperate the inputsource provider from CL_Zip_Archive,
	// but don't really bother doing it right now.
	// -- mbn may 10, 2003
	return clone();
}

CL_OutputSource *CL_Zip_Archive::create_file(const std::string &filename, bool compress)
{
	return 0;
}

void CL_Zip_Archive::add_file(const std::string &filename, bool compress)
{
	 add_file("", filename, compress);
}

void CL_Zip_Archive::add_file(const std::string &input_filename, const std::string &filename_in_archive, bool compress)
{
	CL_Zip_FileEntry file;
	file.impl->type = CL_Zip_FileEntry_Generic::type_file;
	file.impl->record.compression_method = compress ? zip_compress_deflate : zip_compress_store;
	file.set_filename(filename_in_archive);
	file.set_local_filename(input_filename);
	impl->files.push_back(file);
}

void CL_Zip_Archive::save()
{
	if (impl->filename.empty())
	{
		throw CL_Error("CL_Zip_Archive: No filename specified, call save(filename) instead");
	} else
	{
		throw CL_Error("Todo: Write to a temp file, then rename");
	}
}

void CL_Zip_Archive::write_zip_file(CL_Zip_FileEntry *pEntry, CL_OutputSource *output)
{	

	//set some last minute data
	pEntry->impl->record.relative_offset_of_local_header = output->tell(); //remember our location for later
	
	CL_Zip_Archive_Generic::calc_time_and_date(pEntry->impl->record.last_mod_file_date, pEntry->impl->record.last_mod_file_time);
	output->write_int32(0x04034b50);
	output->write_int16(pEntry->impl->record.version_needed_to_extract);
	output->write_int16(pEntry->impl->record.general_purpose_bit_flag);
	output->write_int16(pEntry->impl->record.compression_method);
	output->write_int16(pEntry->impl->record.last_mod_file_time);
	output->write_int16(pEntry->impl->record.last_mod_file_date);
	output->write_int32(pEntry->impl->record.crc32);
	output->write_int32(pEntry->impl->record.compressed_size);
	output->write_int32(pEntry->impl->record.uncompressed_size);
	output->write_int16(pEntry->impl->record.filename.size());
	output->write_int16(pEntry->impl->record.extra_field.size());

	output->write(pEntry->impl->record.filename.data(), pEntry->impl->record.filename.size());
	output->write(pEntry->impl->record.extra_field.data(), pEntry->impl->record.extra_field.size());

	if (pEntry->impl->type != CL_Zip_FileEntry_Generic::type_file)
	{
		throw CL_Error("Don't know how to compress standard added files yet");
	}
	//save out the actual file
	std::string fileName = pEntry->get_filename();

	if (!pEntry->get_local_filename().empty())
	{
		fileName = pEntry->get_local_filename();
	}
	CL_InputSource *input = new CL_InputSource_File(fileName);

	const int bufferSize = 16384;

	cl_uint8 buffer[bufferSize];
	
	int bytesRead;
	int totalBytes = 0;
	int totalBytesCompressed = 0;
	cl_uint32 crc = 0;

	if (pEntry->impl->record.compression_method == 0)
	{
		//not compressed
		while (bytesRead = input->read(buffer, bufferSize))
		{
			crc = crc32(crc, buffer, bytesRead);
			totalBytes += output->write(buffer, bytesRead);
		}
		totalBytesCompressed = totalBytes;

	} else
	{
		//compression version
		int ret, flush;
		z_stream strm;
		cl_uint8 bufferOut[bufferSize];
		int bytesCompressed;

		/* allocate deflate state */
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, MAX_MEM_LEVEL, 0);

		if (ret != Z_OK) 
		{
			throw CL_Error("zLib init error:");
		}

		do
		{
			bytesRead = input->read(buffer, bufferSize);
			crc = crc32(crc, buffer, bytesRead);
			totalBytes += bytesRead;
			strm.avail_in = bytesRead;
			flush = input->tell() >= input->size() ? Z_FINISH : Z_NO_FLUSH;
			strm.next_in = buffer;
		
		/* run deflate() on input until output buffer not full, finish
		compression if all of source has been read in */
		do 
		{
			strm.avail_out = bufferSize;
			strm.next_out = bufferOut;
			ret = deflate(&strm, flush); 
			if (ret == Z_STREAM_ERROR)
			{
				throw CL_Error("zLib error zipping:" + CL_String::from_int(ret));
			}

			bytesCompressed = (bufferSize)-strm.avail_out;
			output->write(bufferOut, bytesCompressed) ;

		} while (strm.avail_out == 0);

	} while(flush != Z_FINISH);


		//guess we're done
		ret = deflateEnd(&strm);
		totalBytesCompressed = strm.total_out;
		if (ret != Z_OK)
		{
			throw CL_Error("zLib deflateEnd error:" + CL_String::from_int(ret));
		}
	}

	delete input;

	//actually now that we know what is going on, copy it over here:
	pEntry->impl->record.compressed_size = totalBytesCompressed;
	pEntry->impl->record.uncompressed_size = totalBytes;
	pEntry->impl->record.crc32  = crc;
	
	//7-zip coughs if you do this
	//pEntry->impl->record.general_purpose_bit_flag = 0; //notify that we now have good data

	//write post-file header now that we know the specifics
	output->write_int32(0x08074b50);  //osx's built-in zip reader coughs if you don't add this header
	
	output->write_int32(pEntry->impl->record.crc32);
	output->write_int32(pEntry->impl->record.compressed_size);
	output->write_int32(pEntry->impl->record.uncompressed_size);
}
	
void CL_Zip_Archive::save(const std::string &filename)
{
	// Load zip file structures:

	CL_OutputSource *output = new CL_OutputSource_File(filename);

	// indicate the file is little-endian
	output->set_little_endian_mode();

	//write out all our files

	for (unsigned int i=0; i < impl->files.size(); i++)
	{
		CL_Zip_FileEntry *pFile = &impl->files[i];
		write_zip_file(pFile, output);
	}
	cl_int32 offset_start_central_dir = output->tell();

	CL_Zip_EndOfCentralDirectoryRecord end_of_directory;
	end_of_directory.number_of_this_disk = 0;
	end_of_directory.number_of_disk_with_start_of_central_directory = 0;
	end_of_directory.number_of_entries_on_this_disk = impl->files.size();
	end_of_directory.number_of_entries_in_central_directory = impl->files.size();
	end_of_directory.offset_to_start_of_central_directory = offset_start_central_dir;
	end_of_directory.file_comment_length = 0;
	
	//save out all the file headers again

	for (unsigned int i=0; i < impl->files.size(); i++)
	{
		CL_Zip_FileEntry *pFile = &impl->files[i];
		pFile->impl->record.save(output); //write the header, which now has all offsets/etc correctly specified
	}

	// write end of central directory record:
	end_of_directory.size_of_central_directory = (output->tell() - offset_start_central_dir);

	end_of_directory.save(output);
	delete output;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_Archive implementation:



void CL_Zip_Archive_Generic::calc_time_and_date(cl_int16 &out_date, cl_int16 &out_time)
{
	cl_uint32 day_of_month = 0;
	cl_uint32 month = 0;
	cl_uint32 year_from_1980 = 0;
	cl_uint32 hour = 0;
	cl_uint32 min = 0;
	cl_uint32 sec = 0;

#if !defined(_WINDOWS)

	//unix style way, slightly different than windows

	time_t t = time(0);
	tm *tm_time;
	tm_time = localtime(&t);
	day_of_month = tm_time->tm_mday;
	month = tm_time->tm_mon + 1;
	year_from_1980 = tm_time->tm_year - 80;
	hour = tm_time->tm_hour;
	min = tm_time->tm_min;
	sec = tm_time->tm_sec;

#elif (_MSC_VER >= 1400)
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
	static CL_Mutex mutex;
	CL_MutexSection mutex_lock(&mutex);
	time_t t = time(0);
	tm *tm_time = gmtime(&t);
	day_of_month = tm_time->tm_mday;
	month = tm_time->tm_mon + 1;
	year_from_1980 = tm_time->tm_year - 80;
	hour = tm_time->tm_hour;
	min = tm_time->tm_min;
	sec = tm_time->tm_sec;
	mutex_lock.unlock();
#endif

	out_date = (cl_int16) (day_of_month + (month << 5) + (year_from_1980 << 9));
	out_time = (cl_int16) (sec/2 + (min << 5) + (hour << 11));
}

