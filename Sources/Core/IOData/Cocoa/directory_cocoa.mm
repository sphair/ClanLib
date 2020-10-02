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
 */

#include "Core/precomp.h"
#include "API/Core/IOData/directory.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_help.h"

#import <Foundation/Foundation.h>

namespace clan
{
	std::string Directory::get_appdata(const std::string &company_name, const std::string &application_name, const std::string &version, bool create_dirs_if_missing)
	{
		return get_local_appdata(company_name, application_name, version, create_dirs_if_missing);
	}
	
	std::string Directory::get_local_appdata(const std::string &company_name, const std::string &application_name, const std::string &version, bool create_dirs_if_missing)
	{
		std::string configuration_path;
		
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
		NSString *libraryDirectory = [paths objectAtIndex:0];
		configuration_path = PathHelp::add_trailing_slash([libraryDirectory UTF8String]);
		
		if (!company_name.empty())
			configuration_path += PathHelp::add_trailing_slash(company_name);
		if (!application_name.empty())
			configuration_path += PathHelp::add_trailing_slash(application_name);
		if (!version.empty())
			configuration_path += PathHelp::add_trailing_slash(version);
		
		if (create_dirs_if_missing)
			create(configuration_path, true);
		
		return configuration_path;
	}
	
	std::string Directory::get_resourcedata(const std::string &application_name, const std::string &data_dir_name)
	{
		std::string resource_path = PathHelp::add_trailing_slash([[[NSBundle mainBundle] resourcePath] UTF8String]);
		return resource_path + data_dir_name;
	}
}
