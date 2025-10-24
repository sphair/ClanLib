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
#include "datafile_inputprovider.h"
#include "API/Core/System/cl_assert.h"

#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#endif

#include <fcntl.h>
#include <cstring>

#ifdef WIN32
	#define OPENFLAGS O_RDONLY|O_BINARY
#else
	#define OPENFLAGS O_RDONLY
#endif

char datafile_id[]="ClanSoft datafile version 4.0";

CL_InputSourceProvider *CL_InputSourceProvider::create_datafile_provider(const std::string &filename)
{
	return new CL_InputSourceProvider_Datafile(filename);
}

CL_InputSourceProvider_Datafile::CL_InputSourceProvider_Datafile(const std::string &_datafile) : resource_cache(NULL), datafile_handle(-1)
{
	datafile = _datafile;
	open();
	load_resource_ids();
}

CL_InputSourceProvider_Datafile::CL_InputSourceProvider_Datafile(CL_InputSourceProvider_Datafile *other)
{
	datafile = other->datafile;
	resource_cache = other->resource_cache;
	resource_cache->addref();
	datafile_handle = dup(other->datafile_handle);
}

CL_InputSourceProvider_Datafile::~CL_InputSourceProvider_Datafile()
{
	if (resource_cache != NULL) 
		resource_cache->release();
	if (datafile_handle != -1)
		::close(datafile_handle);
}

CL_InputSource *CL_InputSourceProvider_Datafile::open_source(const std::string &filename)
{
	if (datafile_handle == -1)
		open();

	return new CL_InputSource_Datafile(filename, this);
}

CL_InputSourceProvider *CL_InputSourceProvider_Datafile::clone()
{
	return new CL_InputSourceProvider_Datafile(this);
}

void CL_InputSourceProvider_Datafile::open()
{
	datafile_handle = ::open(datafile.c_str(), OPENFLAGS);

	if (datafile_handle == -1)
	{
		std::string err = std::string("Could not open datafile ") + datafile;
		throw CL_Error(err);
	}

	int id_len = strlen(datafile_id);

	char *temp = new char[id_len+1];

	::read(datafile_handle, temp, id_len);

	temp[id_len] = 0;

	if (strcmp(temp, datafile_id) != 0)
	{
		::close(datafile_handle);
		datafile_handle = -1;
		delete[] temp;

		throw CL_Error("Invalid datafile format");
	}

	delete[] temp;
}

void CL_InputSourceProvider_Datafile::load_resource_ids()
{
	resource_cache = new IndexLocationCache;

	lseek(datafile_handle, strlen(datafile_id), SEEK_SET);	// Skip file ID
	int index_pos;
	::read(datafile_handle, &index_pos, sizeof(int));
	lseek(datafile_handle, index_pos, SEEK_SET);

	int num_indexes = 0;
	::read(datafile_handle, &num_indexes, sizeof(int));
	for (int i=0; i<num_indexes; i++)
	{
		short length;
		::read(datafile_handle, &length,sizeof(short));

		char *objname = new char[length];
		int objpos;
		int objsize;

		::read(datafile_handle, objname, length);
		::read(datafile_handle, &objpos, sizeof(int));
		::read(datafile_handle, &objsize, sizeof(int));

		resource_cache->insert(objname, objpos, objsize);

		delete[] objname;
	}
}

std::string CL_InputSourceProvider_Datafile::get_pathname(const std::string &filename)
{
	return filename;
}

CL_InputSourceProvider *CL_InputSourceProvider_Datafile::create_relative(const std::string &path)
{
	return clone();
}


/**************************
	CL_InputSource_Datafile
**************************/

CL_InputSource_Datafile::CL_InputSource_Datafile(
	const std::string &filename,
	CL_InputSourceProvider_Datafile *provider) : filename(filename), provider(provider), index_open(0)
{
	open();
}

CL_InputSource_Datafile::CL_InputSource_Datafile(const CL_InputSource_Datafile *source)
{
	filename = source->filename;
	provider = source->provider;
	index_open = 0;

	open();
}

CL_InputSource_Datafile::~CL_InputSource_Datafile()
{
	close();
}

int CL_InputSource_Datafile::read(void *addr, int size)
{
	if (compressed)
	{
		int svar = gzread(gzfile, addr, size);
		seek_pos += svar;
		return svar;
	}
	else
	{
		if (seek_pos + size > objsize) size = objsize - seek_pos;
		int svar = ::read(datafile_handle, addr, size);
		seek_pos += svar;
		return svar;
	}
}

void CL_InputSource_Datafile::open()
{
	if (index_open)
		close();

	int pos, size;
	if (provider->lookup_resource(filename, pos, size))
	{
		datafile_handle = dup(provider->get_handle());
		lseek(datafile_handle, pos, SEEK_SET);

		size--;
		::read(datafile_handle, &compressed, sizeof(char));

		if (compressed) gzfile = gzdopen(datafile_handle, "rb");
		index_open = 1;
		seek_pos = 0;
		objsize = size;
		index_pos = pos+1;
	}
	else
	{
		std::string err("ClanLib: Couldn't find datafile index: ");
		err += std::string(filename);
		throw CL_Error(err);
	}
}

void CL_InputSource_Datafile::close()
{
	if (index_open == 0) return;

	index_open=0;
	if (compressed) gzclose(gzfile);
	else ::close(datafile_handle);
}

CL_InputSource *CL_InputSource_Datafile::clone() const
{
	return new CL_InputSource_Datafile(this);
}

int CL_InputSource_Datafile::tell() const
{
	return seek_pos;
}

void CL_InputSource_Datafile::seek(int pos, SeekEnum seek_type)
{
	if (compressed)
	{
		if (seek_type == seek_cur)
		{
			if (pos > 0)
			{
				char *temp = new char[pos];
				read(temp, pos);
				delete[] temp;
			}
			else if (pos < 0)
			{
				seek(tell()+pos, seek_set);
			}
			return;
		}
		else if (seek_type == seek_set)
		{
			int cur_pos = tell();
			if (pos >= cur_pos)
			{
				seek(pos - cur_pos, seek_cur);
				return;
			}
 			else if (pos>=0)
 			{
 				/*
 				 * OK, now we have to go backwards. Since there's no
 				 * easy way to do it (think about compressed datafiles)
 				 * we close and then open the datafile. This way we are
 				 * back to position 0, and can proceed to a standard
 				 * forward seek. OK this is a  performance killer 
 				 * method, but it's better than nothing...
 				 */
 				close();
 				open();
 				seek(pos, seek_cur);
 				return;
 			}
  		}
  
		throw CL_Error("ClanLib: seek() in datafiles called with an invalid position");
	}
	else
	{
		if (seek_type == seek_set)
		{
			lseek(datafile_handle, index_pos+pos, SEEK_SET);
		}
		else if (seek_type == seek_cur)
		{
			lseek(datafile_handle, pos, SEEK_CUR);
		}
		else if (seek_type == seek_end)
		{
			lseek(datafile_handle, index_pos+objsize+pos, SEEK_SET);
		}
	}
}

int CL_InputSource_Datafile::size() const
{
	return objsize;
}

void CL_InputSource_Datafile::push_position()
{
	CL_Zipped_Position indexpos;

	indexpos.gzfile = gzfile;
//	indexpos.datafile_pos = lseek(datafile_handle, 0, SEEK_CUR);
	indexpos.seek_pos = seek_pos;

	index_stack.push(indexpos);

	index_open=0;
}

void CL_InputSource_Datafile::pop_position()
{
//	if (index_open != 0) close_index();
//	index_open = 1;

	CL_Zipped_Position pushed_index = index_stack.top();
	index_stack.pop();
	
	gzfile = pushed_index.gzfile;
//	lseek(datafile_handle, pushed_index.datafile_pos, SEEK_SET);
	seek_pos = pushed_index.seek_pos;

	if (!compressed) seek(seek_pos, seek_set);
}


