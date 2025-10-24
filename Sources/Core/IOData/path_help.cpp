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
**    Magnus Norddahl
**    Kenneth Gangstoe
*/

#include "Core/precomp.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#ifdef WIN32
#include <tchar.h>
#include <direct.h>
#include <wchar.h>
#endif

#ifndef WIN32
#include <cstring>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_PathHelp Operations:

CL_String CL_PathHelp::make_absolute(
	const CL_String &base_path,
	const CL_String &relative_path,
	PathType path_type)
{
	CL_String base = normalize(base_path, path_type);
	CL_String relative = normalize(relative_path, path_type);
	if (path_type == path_type_file)
	{
		if (!is_absolute(base, path_type_file))
		{
			// make base_path absolute using current drive and directory
#ifdef WIN32
			TCHAR absolute_base[MAX_PATH];
			memset(absolute_base, 0, sizeof(TCHAR) * MAX_PATH);
			if (_tfullpath(absolute_base, CL_StringHelp::utf8_to_ucs2(base).c_str(), MAX_PATH) == 0)
				throw CL_Exception(cl_format("Unable to make base path absolute: %1", base_path));
			base = absolute_base;
#else
			char working_dir[1024];
			memset(working_dir, 0, 1024);
			if (getcwd(working_dir, 1024) == 0)
				throw CL_Exception("Unable to get current working directory!");
			base = add_trailing_slash(working_dir, path_type) + base;
#endif
		}

		CL_String base_location = get_location(base_path, path_type_file);
		CL_String relative_location = get_location(relative_path, path_type_file);
		if (relative_location.empty() || CL_StringHelp::compare(relative_location, base_location, true) == 0)
		{
			if (is_absolute(relative, path_type))
			{
				if (relative_location.empty())
					return base_location + relative;
				else
					return relative;
			}
			CL_String absolute = add_trailing_slash(base, path_type) + relative.substr(relative_location.length());
			return normalize(absolute, path_type);
		}
		else
		{
#ifdef WIN32
			if (is_absolute(relative, path_type))
				return relative;

			if (relative_location.length() == 2 && relative_location[1] == ':')
			{
				int drive = 0;
				if (relative_location[0] >= 'A' && relative_location[0] <= 'Z')
					drive = relative_location[0] - 'A' + 1;
				else if (relative_location[0] >= 'a' && relative_location[0] <= 'z')
					drive = relative_location[0] - 'a' + 1;
				else
					throw CL_Exception(cl_format("Invalid drive: %1", relative_location));
				TCHAR working_dir[MAX_PATH];
				memset(working_dir, 0, sizeof(TCHAR)*MAX_PATH);
				if (_tgetdcwd(drive, working_dir, MAX_PATH) == 0)
					throw CL_Exception(cl_format("Unable to get current working directory for %1!", relative_location));

				return add_trailing_slash(working_dir, path_type) + relative.substr(relative_location.length());
			}
			else
			{
				return relative; // UNC path
			}
#else
			throw CL_Exception("Error in CL_PathHelp::make_absolute");
#endif
		}
	}
	else
	{
		if (is_absolute(relative, path_type))
			return relative;
		CL_String absolute = add_trailing_slash(base, path_type) + relative;
		return normalize(absolute, path_type);
	}
}

CL_String CL_PathHelp::make_relative(
	const CL_String &base_path,
	const CL_String &absolute_path,
	PathType path_type)
{
	CL_String base = add_trailing_slash(normalize(base_path, path_type), path_type);
	CL_String absolute = normalize(absolute_path, path_type);

	if (path_type == path_type_file)
	{
		CL_String base_location = get_location(base, path_type_file);
		CL_String absolute_location = get_location(absolute, path_type_file);

		if (is_relative(base, path_type))
		{
#ifdef WIN32
			if (base_location.length() == 2 && base_location[1] == ':')
			{
				int drive = 0;
				if (base_location[0] >= 'A' && base_location[0] <= 'Z')
					drive = base_location[0] - 'A' + 1;
				else if (base_location[0] >= 'a' && base_location[0] <= 'z')
					drive = base_location[0] - 'a' + 1;
				else
					throw CL_Exception(cl_format("Invalid drive: %1", base_location));
				TCHAR working_dir[MAX_PATH];
				memset(working_dir, 0, sizeof(TCHAR)*MAX_PATH);
				if (_tgetdcwd(drive, working_dir, MAX_PATH) == 0)
					throw CL_Exception(cl_format("Unable to get current working directory for %1!", base_location));

				base = add_trailing_slash(working_dir, path_type) + base;
			}
			else if (base_location.empty())
			{
				TCHAR working_dir[MAX_PATH];
				memset(working_dir, 0, sizeof(TCHAR)*MAX_PATH);
				if (GetCurrentDirectory(MAX_PATH, working_dir) == FALSE)
					throw CL_Exception(cl_format("Unable to get current working directory for %1!", base_location));

				base = add_trailing_slash(working_dir, path_type) + base;
			}
			else
			{
				throw CL_Exception(cl_format("Error in make_relative with base path: %1", base_path));
			}
#else
			char working_dir[1024];
			memset(working_dir, 0, 1024);
			if (getcwd(working_dir, 1024) == 0)
				throw CL_Exception("Unable to get current working directory!");
			base = add_trailing_slash(working_dir, path_type) + base;
#endif
		}
		if (is_relative(absolute, path_type))
		{
#ifdef WIN32
			if (absolute_location.length() == 2 && absolute_location[1] == ':')
			{
				int drive = 0;
				if (absolute_location[0] >= 'A' && absolute_location[0] <= 'Z')
					drive = absolute_location[0] - 'A' + 1;
				else if (absolute_location[0] >= 'a' && absolute_location[0] <= 'z')
					drive = absolute_location[0] - 'a' + 1;
				else
					throw CL_Exception(cl_format("Invalid drive: %1", absolute_location));
				TCHAR working_dir[MAX_PATH];
				memset(working_dir, 0, sizeof(TCHAR)*MAX_PATH);
				if (_tgetdcwd(drive, working_dir, MAX_PATH) == 0)
					throw CL_Exception(cl_format("Unable to get current working directory for %1!", absolute_location));

				absolute = add_trailing_slash(working_dir, path_type) + absolute;
			}
			else if (absolute_location.empty())
			{
				TCHAR working_dir[MAX_PATH];
				memset(working_dir, 0, sizeof(TCHAR)*MAX_PATH);
				if (GetCurrentDirectory(MAX_PATH, working_dir) == FALSE)
					throw CL_Exception(cl_format("Unable to get current working directory for %1!", absolute_location));

				absolute = add_trailing_slash(working_dir, path_type) + absolute;
			}
			else
			{
				throw CL_Exception(cl_format("Error in make_relative with absolute path: %1", absolute_path));
			}
#else
			char working_dir[1024];
			memset(working_dir, 0, 1024);
			if (getcwd(working_dir, 1024) == 0)
				throw CL_Exception("Unable to get current working directory!");
			absolute = add_trailing_slash(working_dir, path_type) + absolute;
#endif
		}

		base_location = get_location(base, path_type_file);
		absolute_location = get_location(absolute, path_type_file);
		if (CL_StringHelp::compare(absolute_location, base_location, true) != 0)
			return absolute_path;
	}

	if (is_relative(base, path_type))
		throw CL_Exception(cl_format("Relative path %1 used as base path for make_relative", base_path));
	if (is_relative(absolute, path_type))
		throw CL_Exception(cl_format("Relative path %1 used as absolute path for make_relative", absolute_path));

	CL_String relative;
	CL_String relative_end;

	bool differs = false;
	CL_String::size_type start_pos = 0, end_pos = 0;
	while (true)
	{
		if (path_type == path_type_file)
		{
			end_pos = base.find_first_of("\\/", start_pos);
		}
		else
		{
			end_pos = base.find('/', start_pos);
		}
		if (end_pos == CL_String::npos)
			break;

		if (!differs)
		{
			CL_String base_element = base.substr(start_pos, end_pos - start_pos + 1);
			CL_String absolute_element = absolute.substr(start_pos, end_pos - start_pos + 1);

			bool same_element = false;
			if (path_type == path_type_file)
			{
#ifdef WIN32
				same_element = (CL_StringHelp::compare(base_element, absolute_element, true) == 0);
#else
				same_element = (base_element == absolute_element);
#endif
			}
			else
			{
				same_element = (base_element == absolute_element);
			}

			if (!same_element)
			{
				relative_end = absolute.substr(start_pos);
				differs = true;
			}
			else
			{
				relative_end = absolute.substr(end_pos+1);
			}
		}

		if (differs)
		{
			if (path_type_file)
			{
#ifdef WIN32
				relative += "..\\";
#else
				relative += "../";
#endif
			}
			else
			{
				relative += "../";
			}
		}

		start_pos = end_pos + 1;
	}

	return relative + relative_end;
}

bool CL_PathHelp::is_absolute(const CL_String &path, PathType path_type)
{
	if (path_type == path_type_file)
	{
#ifdef WIN32
		if (path.length() < 3)
			return false;
		if (path[1] == ':' && (path[2] == '\\' || path[2] == '/'))
			return true;
		if (path[0] == '\\' && path[1] == '\\')
			return true;
		return false;
#else
		if (path.length() < 1)
			return false;
		if (path[0] == '/')
			return true;
		if (path[0] == '\\')
			return true;
		return false;
#endif
	}
	else
	{
		if (path.length() < 1)
			return false;
		if (path[0] == '/')
			return true;
		return false;
	}
}

bool CL_PathHelp::is_relative(const CL_String &path, PathType path_type)
{
	return !is_absolute(path, path_type);
}

CL_String CL_PathHelp::normalize(
	const CL_String &input_path,
	PathType path_type)
{
	if (input_path.empty())
		return CL_String();

	CL_String location = get_location(input_path, path_type);
	CL_String path = input_path.substr(location.length());

	bool ends_in_slash = false;

	if (input_path[input_path.size()-1] == '/' ||
		input_path[input_path.size()-1] == '\\')
	{
		ends_in_slash = true;
		if (input_path.size() == 1)
		{
#ifdef WIN32
			if (path_type == path_type_file)
			{
				return "\\";
			}
#endif
			return "/";
		}
	}

	std::vector<CL_String> elements;
	bool absolute = false;
	int level = 0;
	CL_String::size_type pos = 0, last_pos = 0;

	while (true)
	{
		pos = path.find_first_of("/\\", last_pos);

		if (pos == CL_String::npos)
			pos = path.length();

		if (pos == 0)
		{
			absolute = true;
		}
		else
		{
			CL_String element = path.substr(last_pos, pos-last_pos);
			if (element.empty() && pos != path.length())
				throw CL_Exception(cl_format("Unable to normalize invalid path %1", input_path));

			if (element.empty())
			{
			}
			else if (element == ".")
			{
			}
			else if (element == "..")
			{
				level--;
				if (!elements.empty())
				{
					if (elements[elements.size()-1] != "..")
						elements.erase(elements.begin() + elements.size() - 1);
					else
						elements.push_back("..");
				}
				else
				{
					elements.push_back("..");
				}

				if (absolute && level < 0)
					throw CL_Exception(cl_format("Unable to normalize invalid path %1", input_path));
			}
			else
			{
				elements.push_back(element);
				level++;
			}
		}

		if (pos == path.length())
			break;

		last_pos = pos+1;
	}

	CL_String normalized_path;

	if (absolute)
	{
#ifdef WIN32
		if (path_type == path_type_file)
			normalized_path += "\\";
		else
			normalized_path += "/";
#else
		normalized_path += "/";
#endif
	}

	for (CL_String::size_type i = 0; i < elements.size(); i++)
	{
#ifdef WIN32
		if (path_type == path_type_file)
			normalized_path += elements[i] + "\\";
		else
			normalized_path += elements[i] + "/";
#else
		normalized_path += elements[i] + "/";
#endif
	}
	if (!elements.empty() && !ends_in_slash)
		normalized_path = normalized_path.substr(0, normalized_path.length()-1);

	return location + normalized_path;
}

CL_String CL_PathHelp::add_trailing_slash(const CL_String &path, PathType path_type)
{
	if (path.empty())
		return path;
	if (path_type == path_type_file)
	{
#ifdef WIN32
		if (path[path.length()-1] != '/' && path[path.length()-1] != '\\')
			return path + "\\";
#else
		if (path[path.length()-1] != '/' && path[path.length()-1] != '\\')
			return path + "/";
#endif
		return path;
	}
	else
	{
		if (path[path.length()-1] != '/')
			return path + "/";
		return path;
	}
}

CL_String CL_PathHelp::remove_trailing_slash(const CL_String &path)
{
	if (path.empty())
		return path;

	if (path[path.length()-1] == '/' || path[path.length()-1] == '\\')
		return path.substr(0, path.length()-1);

	return path;
}

CL_String CL_PathHelp::get_location(
	const CL_String &fullname,
	PathType path_type)
{
#ifdef WIN32
	CL_String path = get_fullpath(fullname, path_type);
	if (path_type == path_type_file && path.length() >= 2)
	{
		if (path[1] == ':')
			return path.substr(0, 2);

		if (path[0] == '\\' && path[1] == '\\')
		{
			CL_String::size_type pos = path.find_first_of("/\\", 2);
			if (pos == CL_String::npos)
				return path;
			else
				return path.substr(0, pos);
		}
	}
#endif
	return CL_String();
}

CL_String CL_PathHelp::get_basepath(
	const CL_String &fullname,
	PathType path_type)
{
	CL_String path = get_fullpath(fullname, path_type);
#ifdef WIN32
	if (path_type == path_type_file && path.length() >= 2)
	{
		if (path[1] == ':')
			return path.substr(2);

		if (path[0] == '\\' && path[1] == '\\')
		{
			CL_String::size_type pos = path.find_first_of("/\\", 2);
			if (pos == CL_String::npos)
				return CL_String();
			else
				return path.substr(pos);
		}
	}
#endif
	return path;
}

std::vector<CL_String> CL_PathHelp::split_basepath(
	const CL_String &fullname,
	PathType path_type)
{
	CL_String basepath = get_basepath(fullname, path_type);
	std::vector<CL_String> result;
	if (path_type == path_type_file)
	{
		CL_String::size_type start_pos = 0, end_pos = 0;
		while (true)
		{
			end_pos = basepath.find_first_of("/\\", start_pos);

			if (end_pos == CL_String::npos)
			{
				if (start_pos != basepath.length())
					result.push_back(basepath.substr(start_pos));
				break;
			}

			result.push_back(basepath.substr(start_pos, end_pos - start_pos));
			start_pos = end_pos + 1;
		}
	}
	else
	{
		CL_String::size_type start_pos = 0, end_pos = 0;
		while (true)
		{
			end_pos = basepath.find_first_of('/', start_pos);
			if (end_pos == CL_String::npos)
			{
				if (start_pos != basepath.length())
					result.push_back(basepath.substr(start_pos));
				break;
			}

			result.push_back(basepath.substr(start_pos, end_pos - start_pos));
			start_pos = end_pos + 1;
		}
	}
	return result;
}

CL_String CL_PathHelp::get_fullpath(
	const CL_String &filename,
	PathType path_type)
{
	if (path_type == path_type_file)
	{

		CL_String::size_type pos = filename.find_last_of("/\\");
		if (pos == CL_String::npos)
		{
#ifdef WIN32
			if (filename.length() >= 2 && filename[1] == ':')
				return filename.substr(0, 2);
#endif
			return CL_String();
		}
		return filename.substr(0, pos+1);
	}
	else
	{
		CL_String::size_type pos = filename.find_last_of('/');
		if (pos == CL_String::npos)
			return CL_String();
		return filename.substr(0, pos+1);
	}
}

CL_String CL_PathHelp::get_filename(
	const CL_String &fullname,
	PathType path_type)
{
	if (path_type == path_type_file)
	{
		CL_String::size_type pos = fullname.find_last_of("/\\");
		if (pos == CL_String::npos)
		{
#ifdef WIN32
			if (fullname.length() >= 2 && fullname[1] == ':')
				return fullname.substr(2);
#endif
			return fullname;
		}
		return fullname.substr(pos+1);
	}
	else
	{
		CL_String::size_type pos = fullname.find_last_of('/');
		if (pos == CL_String::npos)
			return fullname;
		return fullname.substr(pos+1);
	}
}

CL_String CL_PathHelp::get_basename(
	const CL_String &fullname,
	PathType path_type)
{
	CL_String filename = get_filename(fullname, path_type);
	CL_String::size_type pos = filename.find_last_of('.');
	if (pos == CL_String::npos)
		return filename;
#ifndef WIN32
	// Files beginning with a dot is not a filename extension in Unix.
	// This is different from Windows where it is considered the extension.
	if (path_type == path_type_file && pos == 0)
		return filename;
#endif
	if (path_type == path_type_virtual && pos == 0)
		return filename;
	return filename.substr(0, pos);
}

CL_String CL_PathHelp::get_extension(
	const CL_String &fullname,
	PathType path_type)
{
	CL_String filename = get_filename(fullname, path_type);
	CL_String::size_type pos = filename.find_last_of('.');
	if (pos == CL_String::npos)
		return CL_String();
#ifndef WIN32
	// Files beginning with a dot is not a filename extension in Unix.
	// This is different from Windows where it is considered the extension.
	if (path_type == path_type_file && pos == 0)
		return CL_String();
#endif
	if (path_type == path_type_virtual && pos == 0)
		return CL_String();
	return filename.substr(pos + 1);
}

CL_String CL_PathHelp::get_fullname(
	const CL_String &fullpath,
	const CL_String &filename,
	PathType path_type)
{
	return add_trailing_slash(fullpath, path_type) + filename;
}

CL_String CL_PathHelp::get_fullname(
	const CL_String &fullpath,
	const CL_String &filename,
	const CL_String &extension,
	PathType path_type)
{
	if (!extension.empty() && extension[0] == '.')
		return add_trailing_slash(fullpath, path_type) + filename + extension;
	else if (!extension.empty())
		return add_trailing_slash(fullpath, path_type) + filename + "." + extension;
	else
		return add_trailing_slash(fullpath, path_type) + filename;
}

CL_String CL_PathHelp::get_fullname(
	const CL_String &location,
	const CL_String &basepath,
	const CL_String &filename,
	const CL_String &extension,
	PathType path_type)
{
	CL_String fullname = location;
	fullname += add_trailing_slash(basepath, path_type);
	fullname += filename;
	if (!extension.empty() && extension[0] == '.')
		fullname += extension;
	else if (!extension.empty())
		fullname += "." + extension;
	return fullname;
}

