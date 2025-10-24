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

#ifndef header_datafile_inputprovider
#define header_datafile_inputprovider

#if _MSC_VER > 1000
#pragma once
#endif

#include <map>
#include <utility>
#include <zlib.h>

#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"

#include <stack>

class CL_Zipped_Position
{
public:
	gzFile gzfile;
	int datafile_pos;
	int seek_pos;
};

class CL_InputSourceProvider_Datafile;

class CL_InputSource_Datafile : public CL_InputSource
{
protected:
	std::string filename;
	CL_InputSourceProvider_Datafile *provider;

	int objsize;
	gzFile gzfile;
	int datafile_handle;
	int index_open;
	char compressed;
	int index_pos;

	std::stack<CL_Zipped_Position> index_stack;
	
	int seek_pos;

public:
	CL_InputSource_Datafile(const std::string &filename, CL_InputSourceProvider_Datafile *provider);
	CL_InputSource_Datafile(const CL_InputSource_Datafile *source);
	virtual ~CL_InputSource_Datafile();

	//: read larger amounts of data (no endian and 64 bit conversion):
	//return: num bytes actually read
	virtual int read(void *data, int size);

	virtual void open();
	virtual void close();

	//: Make a copy of the current InputSource, standing at the same position.
	virtual CL_InputSource *clone() const;

	//: Returns current position in input source
	virtual int tell() const;
	virtual void seek(int pos, SeekEnum seek_type);

	//: Returns the size of the input source
	virtual int size() const;

	virtual void push_position();
	virtual void pop_position();
};

class CL_InputSourceProvider_Datafile : public CL_InputSourceProvider
{
public:
	CL_InputSourceProvider_Datafile(const std::string &datafile);
	CL_InputSourceProvider_Datafile(CL_InputSourceProvider_Datafile *datafile);
	virtual ~CL_InputSourceProvider_Datafile();

	virtual CL_InputSource *open_source(const std::string &filename);
	virtual CL_InputSourceProvider *clone();
	virtual std::string get_pathname(const std::string &filename);
	virtual CL_InputSourceProvider *create_relative(const std::string &path);

	int get_handle() const { return datafile_handle; }
	bool lookup_resource(const std::string &resource_id, int &pos, int &size) const
	{
		return resource_cache->lookup(resource_id, pos, size);
	}

protected:
	class IndexLocationCache
	{
	public:
		IndexLocationCache() : refcount(1) { ; }

		void addref() { refcount++; }
		void release()
		{
			refcount--;
			if (!refcount) delete this;
		}

		bool lookup(const std::string &resource_id, int &data_pos, int &data_size) const
		{
			std::map<std::string, std::pair<int, int> >::const_iterator it = cache.find(resource_id);
			if (it == cache.end())
				return false;

			data_pos = (*it).second.first;
			data_size = (*it).second.second;

			return true;
		}

		void insert(const std::string &resource_id, int data_pos, int data_size)
		{
			cache.insert(
				std::make_pair<std::string const, std::pair<int, int> >(
				resource_id,
				std::make_pair<int,int>(data_pos, data_size)));
		}

	private:
		int refcount;
		std::map<std::string, std::pair<int, int> > cache;
	};
	IndexLocationCache *resource_cache;

	std::string datafile;
	int datafile_handle;
	void open();
	void load_resource_ids();
};

#endif
