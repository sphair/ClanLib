/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "precomp.h"
#include "workspace_generator_msvc8.h"

/////////////////////////////////////////////////////////////////////////////
// Workspace generator class:

WorkspaceGenerator_MSVC8::ConfigurationType WorkspaceGenerator_MSVC8::types[] =
{
	false, "DebugMT", runtime_static_debug,
	false, "DebugMTDLL", runtime_dll_debug,
	false, "ReleaseMT", runtime_static_release,
	false, "ReleaseMTDLL", runtime_dll_release,
	false, 0, runtime_static_debug
};

WorkspaceGenerator_MSVC8::WorkspaceGenerator_MSVC8()
{
	is_enable_sse2 = true;	// Set by set_platforms()
	is_debug_optimize = false;	// Set by set_platforms()
	is_whole_program_optimize = false;	// Set by set_platforms()
}

void WorkspaceGenerator_MSVC8::enable_configurations(bool include_mtdll)
{
	int i;
	for (i = 0; types[i].name != 0; i++)
	{
		types[i].included = true;

		if (types[i].runtime_type == runtime_dll_debug || types[i].runtime_type == runtime_dll_release)
		{
			if (include_mtdll == false)
				types[i].included = false;
		}

	}
}

void WorkspaceGenerator_MSVC8::set_target_version(int version)
{
	target_version = version;
}

void WorkspaceGenerator_MSVC8::set_platforms(bool include_win32, bool include_x64, bool include_sse2, bool include_intrinsics, bool enable_debug_optimise, bool enable_whole_program_optimize)
{
	is_debug_optimize = enable_debug_optimise;
	is_enable_sse2 = include_sse2;
	is_enable_intrinsics = include_intrinsics;
	is_whole_program_optimize = enable_whole_program_optimize;
	include_platform_win32 = include_win32;
	include_platform_x64 = include_x64;
}

void WorkspaceGenerator_MSVC8::set_android(bool enable)
{
	target_android = enable;
}

void WorkspaceGenerator_MSVC8::write(const Workspace &workspace)
{
	write_solution(workspace);
	write_property_sheet(workspace);

	std::list<Project>::const_iterator it;
	for (it = workspace.projects.begin(); it != workspace.projects.end(); ++it)
	{
		write_project(workspace, *it);
	}
}

void WorkspaceGenerator_MSVC8::write_solution(const Workspace &workspace)
{
	std::list<Project>::const_iterator it;
	
	std::string sln_filename;
	std::string vcproj_filename;
	sln_filename = "ClanLib.sln";
	vcproj_filename = ".vcxproj";

	OutputWriter writer(sln_filename);

	if (target_version == 1200)
	{
		writer.write_line(0, "Microsoft Visual Studio Solution File, Format Version 12.00");
		writer.write_line(0, "# Visual C++ Express 2013");
	}
	else	// (target_version == 1400)
	{
		writer.write_line(0, "Microsoft Visual Studio Solution File, Format Version 12.00");	// Note, format version has not changed
		writer.write_line(0, "# Visual Studio 14");
		writer.write_line(0, "VisualStudioVersion = 14.0.22310.1");
		writer.write_line(0, "MinimumVisualStudioVersion = 10.0.40219.1");
	}

	for (it = workspace.projects.begin(); it != workspace.projects.end(); ++it)
	{
		const Project &project = *it;
		std::string project_guid = get_project_guid(project.name);
		std::string project_filename = "Projects\\" + project.libname + vcproj_filename;

		writer.write_line(0, "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"" + project.name + "\", \"" + project_filename + "\", \"" + project_guid + "\"");
		writer.write_line(0, "EndProject");
	}

	writer.write_line(0, "Global");

	writer.write_line(1, "GlobalSection(SolutionConfigurationPlatforms) = preSolution");
	int i;
	for (i = 0; types[i].name != 0; i++)
	{
		if (types[i].included == false)
			continue;
		char line[256];
		if (target_android)
		{
			sprintf_s(line, 256, "%s|Androidx86 = %s|Androidx86", types[i].name, types[i].name);
			writer.write_line(2, line);
			sprintf_s(line, 256, "%s|AndroidARM = %s|AndroidARM", types[i].name, types[i].name);
			writer.write_line(2, line);
		}
		else
		{
			sprintf_s(line, 256, "%s|Win32 = %s|Win32", types[i].name, types[i].name);
			writer.write_line(2, line);
			if (include_platform_x64)
			{
				sprintf_s(line, 256, "%s|x64 = %s|x64", types[i].name, types[i].name);
				writer.write_line(2, line);
			}
		}
	}
	writer.write_line(1, "EndGlobalSection");

	writer.write_line(1, "GlobalSection(ProjectConfigurationPlatforms) = postSolution");
	for (it = workspace.projects.begin(); it != workspace.projects.end(); ++it)
	{
		std::string project_guid = get_project_guid(it->name);
		for (i = 0; types[i].name != 0; i++)
		{
			if (types[i].included == false)
				continue;
			char line[256];

			if (target_android)
			{
				sprintf_s(line, 256, "%s.%s|Androidx86.ActiveCfg = %s|x86", project_guid.c_str(), types[i].name, types[i].name);
				writer.write_line(2, line);
				sprintf_s(line, 256, "%s.%s|Androidx86.Build.0 = %s|x86", project_guid.c_str(), types[i].name, types[i].name);
				writer.write_line(2, line);
				sprintf_s(line, 256, "%s.%s|AndroidARM.ActiveCfg = %s|ARM", project_guid.c_str(), types[i].name, types[i].name);
				writer.write_line(2, line);
				sprintf_s(line, 256, "%s.%s|AndroidARM.Build.0 = %s|ARM", project_guid.c_str(), types[i].name, types[i].name);
				writer.write_line(2, line);
			}
			else
			{
				sprintf_s(line, 256, "%s.%s|Win32.ActiveCfg = %s|Win32", project_guid.c_str(), types[i].name, types[i].name);
				writer.write_line(2, line);
				sprintf_s(line, 256, "%s.%s|Win32.Build.0 = %s|Win32", project_guid.c_str(), types[i].name, types[i].name);
				writer.write_line(2, line);

				if (include_platform_x64)
				{
					sprintf_s(line, 256, "%s.%s|x64.ActiveCfg = %s|x64", project_guid.c_str(), types[i].name, types[i].name);
					writer.write_line(2, line);
					sprintf_s(line, 256, "%s.%s|x64.Build.0 = %s|x64", project_guid.c_str(), types[i].name, types[i].name);
					writer.write_line(2, line);
				}
			}

		}
	}
	writer.write_line(1, "EndGlobalSection");

	writer.write_line(1, "GlobalSection(SolutionProperties) = preSolution");
	writer.write_line(2, "HideSolutionNode = FALSE");
	writer.write_line(1, "EndGlobalSection");

	writer.write_line(0, "EndGlobal");
}

void WorkspaceGenerator_MSVC8::write_property_sheet(const Workspace &workspace)
{
	MSVC8_PropertySheet propertysheet(target_version, target_android);
	propertysheet.name = "External Directories";

	propertysheet.input_include_dir_vs100 = workspace.input_include_dir;
	propertysheet.input_lib_dir_vs100 = workspace.input_lib_dir;

	std::string propertysheet_filename = "Projects\\Sheets\\ExternalDirectories.props";

	OutputWriter writer(propertysheet_filename.c_str());
	propertysheet.write(writer, 0);
}

void WorkspaceGenerator_MSVC8::write_project(const Workspace &workspace, const Project &project)
{
	std::string precomp_header;
	bool has_precomp = false;
	{
		std::list<std::string>::const_iterator it;
		for (it = project.files.begin(); it != project.files.end(); ++it)
		{
			if ((*it).find("precomp.h") != std::string::npos)
			{
				has_precomp = true;
				precomp_header = *it;
				std::string::size_type pos = precomp_header.find_last_of("/\\");
				if (pos == std::string::npos)
					precomp_header = project.name + "/" + precomp_header;
				else
					precomp_header = project.name + "/" + precomp_header.substr(pos+1);
				break;
			}
		}
	}

	MSVC8_Project vc80proj;
	vc80proj.name = project.name;
	vc80proj.target_version = target_version;
	vc80proj.target_android = target_android;
	vc80proj.project_guid = get_project_guid(vc80proj.name);

	if (target_android)
	{
		vc80proj.platforms.push_back("Androidx86");
		vc80proj.platforms.push_back("AndroidARM");
	}
	if (include_platform_win32)
		vc80proj.platforms.push_back("Win32");
	if (include_platform_x64)
		vc80proj.platforms.push_back("x64");

	for (int i = 0; types[i].name != 0; i++)
	{
		if (types[i].included == false)
			continue;

		switch (types[i].runtime_type)
		{
		case runtime_static_debug:
			if(include_platform_win32)
				vc80proj.configurations.push_back(create_debug_mt_config("Win32", project.name, types[i], has_precomp, precomp_header, is_enable_sse2));
			if(include_platform_x64)
				vc80proj.configurations.push_back(create_debug_mt_config("x64", project.name, types[i], has_precomp, precomp_header, false));
			if (target_android)
			{
				vc80proj.configurations.push_back(create_android_config("x86", project.name, types[i], has_precomp, precomp_header));
				vc80proj.configurations.push_back(create_android_config("ARM", project.name, types[i], has_precomp, precomp_header));
			}
			break;
		case runtime_static_release:
			if(include_platform_win32)
				vc80proj.configurations.push_back(create_release_mt_config("Win32", project.name, types[i], has_precomp, precomp_header, is_enable_sse2));
			if(include_platform_x64)
				vc80proj.configurations.push_back(create_release_mt_config("x64", project.name, types[i], has_precomp, precomp_header, false));
			if (target_android)
			{
				vc80proj.configurations.push_back(create_android_config("x86", project.name, types[i], has_precomp, precomp_header));
				vc80proj.configurations.push_back(create_android_config("ARM", project.name, types[i], has_precomp, precomp_header));
			}
			break;
		case runtime_dll_debug:
			if(include_platform_win32)
				vc80proj.configurations.push_back(create_debug_mtdll_config("Win32", project.name, types[i], has_precomp, precomp_header, is_enable_sse2));
			if(include_platform_x64)
				vc80proj.configurations.push_back(create_debug_mtdll_config("x64", project.name, types[i], has_precomp, precomp_header, false));
			break;
		case runtime_dll_release:
			if(include_platform_win32)
				vc80proj.configurations.push_back(create_release_mtdll_config("Win32", project.name, types[i], has_precomp, precomp_header, is_enable_sse2));
			if(include_platform_x64)
				vc80proj.configurations.push_back(create_release_mtdll_config("x64", project.name, types[i], has_precomp, precomp_header, false));
			break;
		}
	}

	generate_source_files(vc80proj, project);

	std::string project_filename = "Projects\\" + project.libname + ".vcxproj";
	OutputWriter writer(project_filename.c_str());
	vc80proj.write(writer, 0);

	std::string filters_filename = "Projects\\" + project.libname + ".vcxproj.filters";
	OutputWriter writer_filters(filters_filename.c_str());
	vc80proj.write_filters(writer_filters, 0);

	write_install_batch_file(workspace, project);
}

void WorkspaceGenerator_MSVC8::write_install_batch_file(const Workspace &workspace, const Project &project)
{
	// create the install_libname.bat installation batch file:
	{
		std::string bat_file = "Projects\\install_clan";
		bat_file += project.name.c_str();
		bat_file += ".bat";

		std::ofstream bat(bat_file.c_str());
		bat << "mode con cp select=" << GetACP() << " > nul" << std::endl;

		install_mkdir(bat, workspace.output_lib_dir);

		std::string instdir = workspace.output_include_dir.c_str();
		instdir += "\\ClanLib";
		install_mkdir(bat, "API\\", std::string(instdir), &project);
		install_copydir(bat, "API\\", std::string(instdir), &project);

		bat << "copy %1 \"" << workspace.output_lib_dir.c_str() << "\\%4\" > nul" << std::endl;
		if (target_android)	//TODO: Fixme
			bat << "rem ";
		bat << "copy %2 \"" << workspace.output_lib_dir.c_str() << "\\%4\\%3\" > nul" << std::endl;
	}
}

void WorkspaceGenerator_MSVC8::install_mkdir(
	std::ofstream &bat,
	const std::string &dest_dir)
{
	static OSVERSIONINFO versionInfo;
	static bool firstCall = true;
	if (firstCall)
	{
		memset(&versionInfo, 0, sizeof(OSVERSIONINFO));
		GetVersionEx(&versionInfo);
		firstCall = false;
	}

	bool win9x = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);

	if (win9x)
	{
		bat << "if not exist \"" << dest_dir << "\\%4\\nul\"" << " mkdir \"" << dest_dir << "\\%4\"" << std::endl;
	}
	else
	{
		bat << "if not exist \"" << dest_dir << "\\%4\"" << " mkdir \"" << dest_dir << "\\%4\"" << std::endl;
	}
}

void WorkspaceGenerator_MSVC8::install_mkdir(
	std::ofstream &bat,
	const std::string &src_dir,
	const std::string &dest_dir,
	const Project *project)
{
	static OSVERSIONINFO versionInfo;
	static bool firstCall = true;
	if (firstCall)
	{
		memset(&versionInfo, 0, sizeof(OSVERSIONINFO));
		GetVersionEx(&versionInfo);
		firstCall = false;
	}

	bool win9x = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);

	if (win9x)
	{
		bat << "if not exist \"" << dest_dir << "\\nul\"" << " mkdir \"" << dest_dir << "\"" << std::endl;
	}
	else
	{
		bat << "if not exist \"" << dest_dir << "\"" << " mkdir \"" << dest_dir << "\"" << std::endl;
	}

	std::string path = src_dir.c_str();
	if (path[path.length() - 1] != '\\')
		path += '\\';

	std::string path_dest = dest_dir.c_str();
	if (path_dest[path_dest.length() - 1] != '\\')
		path_dest += '\\';

	if (project)
	{
		// first time call: 
		// - make sure we create Module specific directories

		path = path + project->name.c_str();
		if (path[path.length() - 1] != '\\')
			path += '\\';

		path_dest = path_dest + project->name.c_str();
		if (win9x)
		{
			bat << "if not exist \"" << std::string(path_dest) << "\\nul\"" << " mkdir \"" << std::string(path_dest) << "\"" << std::endl;
		}
		else
		{
			bat << "if not exist \"" << std::string(path_dest) << "\"" << " mkdir \"" << std::string(path_dest) << "\"" << std::endl;
		}

		if (path_dest[path_dest.length() - 1] != '\\')
			path_dest += '\\';
	}

	std::string prefix = "Sources\\";

	WIN32_FIND_DATAA data;
	HANDLE handle = FindFirstFileA((prefix + path + "*.*").c_str(), &data);
	if (handle == INVALID_HANDLE_VALUE) return;

	static const char *exclude_from_build[] =
	{
		".",
		"..",
		"CVS",
		".svn",
		".#", // don't add CVS revision backups.
		NULL
	};

	do
	{
		bool skip = false;

		for (int i=0; exclude_from_build[i] != NULL; i++)
			if (stricmp(data.cFileName, exclude_from_build[i]) == 0) skip = true;

		if (skip) continue;

		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			std::string subdir = data.cFileName;
			install_mkdir(
				bat,
				std::string(path + subdir),
				std::string(path_dest + subdir),
				0);
		}
	} while (FindNextFileA(handle, &data));
}

void WorkspaceGenerator_MSVC8::install_copydir(
	std::ofstream &bat,
	const std::string &src_dir,
	const std::string &dest_dir,
	const Project *project)
{
	std::string path = src_dir.c_str();
	if (path[path.length() - 1] != '\\')
		path += '\\';

	std::string path_dest = dest_dir.c_str();
	if (path_dest[path_dest.length() - 1] != '\\')
		path_dest += '\\';

	if (project)
	{
		// first time call: 
		// - make sure we copy the 'module' api header
		// - make sure we get API\\ModuleName\*.h in this run

		bat << "copy \"..\\Sources\\" << path.c_str() << project->headername.c_str() << "\" \"" << path_dest.c_str() << project->headername.c_str() << "\" > nul" << std::endl; // "\"\t";

		path += project->name.c_str();
		if (path[path.length() - 1] != '\\') path += '\\';
		
		path_dest += project->name.c_str();
		if (path_dest[path_dest.length() - 1] != '\\') path_dest += '\\';

	}

	std::string prefix = "Sources\\";

	WIN32_FIND_DATAA data;
	HANDLE handle = FindFirstFileA((prefix + path + "*.*").c_str(), &data);
	if (handle == INVALID_HANDLE_VALUE) return;

	static const char *exclude_from_build[] =
	{
		".",
		"..",
		"CVS",
		".svn",
		".#", // don't add CVS revision backups.
		NULL
	};

	do
	{
		bool skip = false;

		for (int i=0; exclude_from_build[i] != NULL; i++)
			if (stricmp(data.cFileName, exclude_from_build[i]) == 0) skip = true;

		if (skip) continue;

		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			std::string subdir = data.cFileName;
			install_copydir(
				bat,
				std::string(path + subdir),
				std::string(path_dest + subdir),
				0);
		}
		else
		{
			std::string file = data.cFileName;
			bat << "copy \"..\\Sources\\" << path.c_str() << file.c_str() << "\" \"" << path_dest.c_str() << file.c_str() << "\" > nul" << std::endl; // "\"\t";
		}

	} while (FindNextFileA(handle, &data));
}

WorkspaceGenerator_MSVC8::SharedConfig WorkspaceGenerator_MSVC8::create_shared_config(
	const std::string &platform,
	const std::string &project_name,
	const ConfigurationType &config,
	bool has_precomp,
	const std::string &precomp_header)
{
	SharedConfig shared;
	shared.config = new MSVC8_Configuration;
	shared.config->name = std::string(config.name) + "|" + platform;
	shared.config->name_without_platform = std::string(config.name);
	shared.config->name_without_config = platform;

	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\BuildDirectory.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\ExternalDirectories.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\WindowsVersion.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DirectXVersion.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\LocalIncludes.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\MultiprocessorBuilding.props");

	if (!is_enable_intrinsics)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DisableIntrinsics.props");
	}

	if (has_precomp)
	{
		shared.config->use_precompiled_header= "2";
		shared.config->precompiled_header_through = precomp_header;
	}

	return shared;
}

MSVC8_Configuration *WorkspaceGenerator_MSVC8::create_android_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header)
{
	SharedConfig shared;
	shared.config = new MSVC8_Configuration;
	shared.config->is_this_android = true;
	shared.config->name = std::string(config.name) + "|" + platform;
	shared.config->name_without_platform = std::string(config.name);
	shared.config->name_without_config = platform;

	if (has_precomp)
	{
		shared.config->use_precompiled_header = "2";
		shared.config->precompiled_header_through = "../Sources/" + precomp_header;
	}

	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\" + platform + "Platform.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\AndroidBuildDirectory.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\ExternalDirectories.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\AndroidLocalIncludes.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\AndroidRuntime.props");

	//FIXME: if (!is_enable_intrinsics)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DisableIntrinsics.props");
	}

	std::string output_file = "clan$(ProjectName)-static";
	if (config.runtime_type == runtime_static_debug || config.runtime_type == runtime_dll_debug)
	{
		output_file += "-debug";
		shared.config->android_debug_libraries = "true";


		//if (is_debug_optimize)
		//{
		//	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DebugBuildOptimized.props");
		//}
		//else
		//{
		//	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DebugBuild.props");
		//}

	}
	else
	{
		//shared.config->inherited_property_sheets_vs100.push_back("Sheets\\ReleaseBuild.props");

	}

	shared.config->target_name_vs100 = output_file;

	return shared.config;
}

MSVC8_Configuration *WorkspaceGenerator_MSVC8::create_debug_mt_config(
	const std::string &platform,
	const std::string &project_name,
	const ConfigurationType &config,
	bool has_precomp,
	const std::string &precomp_header, bool write_sse2_props)
{
	SharedConfig shared = create_shared_config(platform, project_name, config, has_precomp, precomp_header);

	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\" + platform + "Platform.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\MTDebugRuntime.props");

	if (is_debug_optimize)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DebugBuildOptimized.props");
	}
	else
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DebugBuild.props");
	}

	if (write_sse2_props)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\SSE2Build.props");
	}

	shared.config->target_name_vs100 = make_target_name(config, platform, project_name);

	return shared.config;
}

MSVC8_Configuration *WorkspaceGenerator_MSVC8::create_release_mt_config(
	const std::string &platform,
	const std::string &project_name,
	const ConfigurationType &config,
	bool has_precomp,
	const std::string &precomp_header, bool write_sse2_props)
{
	SharedConfig shared = create_shared_config(platform, project_name, config, has_precomp, precomp_header);

	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\" + platform + "Platform.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\MTReleaseRuntime.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\ReleaseBuild.props");

	if (write_sse2_props)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\SSE2Build.props");
	}

	if (is_whole_program_optimize)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\ReleaseWholeProgramOptimization.props");
	}

	shared.config->target_name_vs100 = make_target_name(config, platform, project_name);

	return shared.config;
}

MSVC8_Configuration *WorkspaceGenerator_MSVC8::create_debug_mtdll_config(
	const std::string &platform,
	const std::string &project_name,
	const ConfigurationType &config,
	bool has_precomp,
	const std::string &precomp_header, bool write_sse2_props)
{
	SharedConfig shared = create_shared_config(platform, project_name, config, has_precomp, precomp_header);

	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\" + platform + "Platform.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\MTDLLDebugRuntime.props");

	if (is_debug_optimize)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DebugBuildOptimized.props");
	}
	else
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DebugBuild.props");
	}

	if (write_sse2_props)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\SSE2Build.props");
	}

	shared.config->target_name_vs100 = make_target_name(config, platform, project_name);

	return shared.config;
}

MSVC8_Configuration *WorkspaceGenerator_MSVC8::create_release_mtdll_config(
	const std::string &platform,
	const std::string &project_name,
	const ConfigurationType &config,
	bool has_precomp,
	const std::string &precomp_header, bool write_sse2_props)
{
	SharedConfig shared = create_shared_config(platform, project_name, config, has_precomp, precomp_header);
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\" + platform + "Platform.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\MTDLLReleaseRuntime.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\ReleaseBuild.props");

	if (write_sse2_props)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\SSE2Build.props");
	}

	shared.config->target_name_vs100 = make_target_name(config, platform, project_name);

	return shared.config;
}

MSVC8_Configuration *WorkspaceGenerator_MSVC8::create_debug_dll_config(
	const std::string &platform,
	const std::string &project_name,
	const ConfigurationType &config,
	bool has_precomp,
	const std::string &precomp_header, bool write_sse2_props)
{
	SharedConfig shared = create_shared_config(platform, project_name, config, has_precomp, precomp_header);

	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\" + platform + "Platform.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\MTDLLDebugRuntime.props");

	if (is_debug_optimize)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DebugBuildOptimized.props");
	}
	else
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\DebugBuild.props");
	}

	if (write_sse2_props)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\SSE2Build.props");
	}

	shared.config->target_name_vs100 = make_target_name(config, platform, project_name);

	return shared.config;
}

MSVC8_Configuration *WorkspaceGenerator_MSVC8::create_release_dll_config(
	const std::string &platform,
	const std::string &project_name,
	const ConfigurationType &config,
	bool has_precomp,
	const std::string &precomp_header, bool write_sse2_props)
{
	SharedConfig shared = create_shared_config(platform, project_name, config, has_precomp, precomp_header);

	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\" + platform + "Platform.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\MTDLLReleaseRuntime.props");
	shared.config->inherited_property_sheets_vs100.push_back("Sheets\\ReleaseBuild.props");

	if (write_sse2_props)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\SSE2Build.props");
	}

	if (is_whole_program_optimize)
	{
		shared.config->inherited_property_sheets_vs100.push_back("Sheets\\ReleaseWholeProgramOptimization.props");
	}

	shared.config->target_name_vs100 = make_target_name(config, platform, project_name);

	return shared.config;
}

std::string WorkspaceGenerator_MSVC8::make_target_name(
	const ConfigurationType &config,
	const std::string &platform,
	const std::string &project_name)
{
	std::string output_file = "clan$(ProjectName)";

	if (config.runtime_type != runtime_static_debug && config.runtime_type != runtime_static_release)
	{
		output_file += "-static-mtdll";
	}
	else
	{
		output_file += "-static-mt";
	}

	if (config.runtime_type == runtime_static_debug || config.runtime_type == runtime_dll_debug)
		output_file += "-debug";

	return output_file;
}
void WorkspaceGenerator_MSVC8::generate_source_files(MSVC8_Project &vcproj, const Project &project)
{
	std::string chop_str = "Sources\\";
	std::string::size_type chop_length = chop_str.length();

	std::list<std::string>::iterator itCur, itNew;
	std::list<std::string> cur_path;
	std::vector<MSVC8_Filter *> filters;

	std::list<std::string>::const_iterator itFiles;
	for (itFiles = project.files.begin(); itFiles != project.files.end(); itFiles++)
	{
		std::string file = *itFiles;

		// cut off path to files:
		if (file.length() > chop_length && file.substr(0, chop_length) == chop_str)
		{
			file = file.substr(chop_length);
		}

		std::list<std::string> new_path = extract_path(file);

		itCur = cur_path.begin();
		itNew = new_path.begin();
		while (itCur != cur_path.end())
		{
			if (itNew == new_path.end() || *itCur != *itNew) // path is no longer identical
			{
				while (itCur != cur_path.end())
				{
					filters.pop_back();
					itCur++;
				}
				break;
			}

			itCur++;
			itNew++;
		}
		while (itNew != new_path.end())
		{
			std::string filter_name = *itNew;

			MSVC8_Filter *filter = new MSVC8_Filter;
			filter->name = filter_name;
			if (filters.empty())
				vcproj.files.push_back(filter);
			else
				filters.back()->files.push_back(filter);
			filters.push_back(filter);

			itNew++;
		}

		cur_path = new_path;

		MSVC8_File *vcfile = new MSVC8_File;
		vcfile->relative_path = "..\\" + chop_str + file;

		if (filters.empty())
			vcproj.files.push_back(vcfile);
		else
			filters.back()->files.push_back(vcfile);
	}

	for (itCur = cur_path.begin(); itCur != cur_path.end(); itCur++)
		filters.pop_back();
}

std::list<std::string> WorkspaceGenerator_MSVC8::extract_path(const std::string &fullname)
{
	std::list<std::string> path;
	std::string::size_type old_pos = 0;

	while (true)
	{
		std::string::size_type pos = fullname.find("\\", old_pos);
		if (pos == std::string::npos) break;

		path.push_back(fullname.substr(old_pos, pos-old_pos));

		old_pos = pos + 1;
	}

	return path;
}

std::string WorkspaceGenerator_MSVC8::get_project_guid(const std::string &project)
{
	std::string returnGUID;

	// Check if we already made a GUID earlier. Reuse it if we did.
	LONG result;
	HKEY key = 0;
	result = RegCreateKeyA(HKEY_CURRENT_USER, "SOFTWARE\\Clanlib.org\\ClanLib\\ProjectGUIDs", &key);
	if (result != ERROR_SUCCESS)
		key = 0;
	if (key)
	{
		DWORD type = 0, needed = 0;
		result = RegQueryValueExA(key, project.c_str(), 0, &type, 0, &needed);
		if (result == ERROR_SUCCESS && type == REG_SZ)
		{
			char *buffer = new char[needed];
			result = RegQueryValueExA(key, project.c_str(), 0, &type, (LPBYTE) buffer, &needed);
			if (result == ERROR_SUCCESS)
			{
				returnGUID = buffer;
				delete[] buffer;
				RegCloseKey(key);
				return returnGUID;
			}
			delete[] buffer;
		}
	}

	// Create a new GUID:
	unsigned char *projectGUID = 0;
	GUID guid;
	UuidCreate(&guid);
	UuidToStringA(&guid, &projectGUID);
	_strupr((char *) projectGUID);
	returnGUID = std::string("{") + ((char *) projectGUID) + std::string("}");
	RpcStringFreeA(&projectGUID);

	// Save it to registry:
	if (key)
	{
		result = RegSetValueExA(
			key,
			project.c_str(),
			0,
			REG_SZ,
			(LPBYTE) returnGUID.c_str(),
			(DWORD) returnGUID.length()+1);
		RegCloseKey(key);
	}

	return returnGUID;
}

std::string WorkspaceGenerator_MSVC8::make_upper(const std::string &s)
{
	std::string u = s;
	std::string::size_type i, len;
	len = u.length();
	for (i = 0; i < len; i++)
	{
		if (u[i] >= 'a' && u[i] <= 'z')
			u[i] -= 'a' - 'A';
	}
	return u;
}

/////////////////////////////////////////////////////////////////////////////
// OutputWriter class:

OutputWriter::OutputWriter(const std::string &filename)
: output(filename.c_str())
{
}

void OutputWriter::write_line(int indent, const std::string &line)
{
	for (int i = 0; i < indent; i++)
		output << "\t";
	output << line;
	output << std::endl;
}

/////////////////////////////////////////////////////////////////////////////
// MSVC8_PropertySheet class:

MSVC8_PropertySheet::MSVC8_PropertySheet(int target_version, bool target_android) : target_version(target_version), target_android(target_android)
{
}

MSVC8_PropertySheet::~MSVC8_PropertySheet()
{
}

void MSVC8_PropertySheet::write(OutputWriter &output, int indent)
{
	output.write_line(indent, "<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	output.write_line(indent, "<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">");
	output.write_line(indent, "  <PropertyGroup>");
	output.write_line(indent, "    <_ProjectFileVersion>10.0.30319.1</_ProjectFileVersion>");
	output.write_line(indent, "    <_PropertySheetDisplayName>External Directories</_PropertySheetDisplayName>");
	output.write_line(indent, "  </PropertyGroup>");
	output.write_line(indent, "  <ItemDefinitionGroup>");
	output.write_line(indent, "    <ClCompile>");
	output.write_line(indent, "      <AdditionalIncludeDirectories>" + input_include_dir_vs100 + ";%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>");
	output.write_line(indent, "    </ClCompile>");

	if (!target_android)
	{
		output.write_line(indent, "    <Lib>");
		output.write_line(indent, "      <AdditionalLibraryDirectories>" + input_lib_dir_vs100 + "\\$(Platform)$(Configuration);%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>");
		output.write_line(indent, "    <LinkTimeCodeGeneration>true</LinkTimeCodeGeneration>");	// Can the set when the "whole program optimisation" is disabled?
		output.write_line(indent, "    </Lib>");
	}
	output.write_line(indent, "  </ItemDefinitionGroup>");
	output.write_line(indent, "</Project>");
}

/////////////////////////////////////////////////////////////////////////////
// MSVC8_Project class:

MSVC8_Project::MSVC8_Project()
{
}

MSVC8_Project::~MSVC8_Project()
{
	std::vector<MSVC8_Configuration *>::size_type index_conf;
	std::vector<MSVC8_FileItem *>::size_type index_files;

	for (index_conf = 0; index_conf < configurations.size(); index_conf++)
		delete configurations[index_conf];
	configurations.clear();

	for (index_files = 0; index_files < files.size(); index_files++)
		delete files[index_files];
	files.clear();
}

void MSVC8_Project::write(OutputWriter &output, int indent) const
{
	std::vector<std::string>::size_type index;
	std::vector<std::string>::size_type index2;

	output.write_line(indent, "<?xml version=\"1.0\" encoding=\"utf-8\"?>");

	std::string tools_version;
	if (target_version == 1200)
		tools_version = "12.0";
	else
		tools_version = "14.0";

	output.write_line(indent, "<Project DefaultTargets=\"Build\" ToolsVersion=\"" + tools_version + "\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">");
  	output.write_line(indent, "<ItemGroup Label=\"ProjectConfigurations\">");
		
	for (index = 0; index < configurations.size(); index++)
	{
  		output.write_line(indent, "    <ProjectConfiguration Include=\"" + configurations[index]->name + "\">");
		output.write_line(indent, "      <Configuration>" + configurations[index]->name_without_platform + "</Configuration>");
		output.write_line(indent, "      <Platform>" + configurations[index]->name_without_config + "</Platform>");
  		output.write_line(indent, "    </ProjectConfiguration>");
	}

  	output.write_line(indent, "  </ItemGroup>");
  	output.write_line(indent, "  <PropertyGroup Label=\"Globals\">");
  	output.write_line(indent, "    <ProjectName>" + name + "</ProjectName>");
  	output.write_line(indent, "    <ProjectGuid>" + project_guid + "</ProjectGuid>");

	if (target_android)
	{
		output.write_line(indent, "		<Keyword>Android</Keyword>");
		output.write_line(indent, "		<ApplicationType>Android</ApplicationType>");
		output.write_line(indent, "		<ApplicationTypeRevision>1.0</ApplicationTypeRevision>");
	}

  	output.write_line(indent, "  </PropertyGroup>");

  	output.write_line(indent, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />");

	for (index = 0; index < configurations.size(); index++)
	{
	  	output.write_line(indent, "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[index]->name + "'\" Label=\"Configuration\">");
		output.write_line(indent, "    <ConfigurationType>StaticLibrary</ConfigurationType>");

		if (target_android)
		{
			output.write_line(indent, "    <PlatformToolset>Clang_3_6</PlatformToolset>");
			output.write_line(indent, "    <AndroidAPILevel>android-21</AndroidAPILevel>");
			output.write_line(indent, "    <UseOfStl>gnustl_static</UseOfStl>");		// TODO: Optional dynamic (like -mtdll on windows)
			if (!configurations[index]->android_debug_libraries.empty())
			{
				output.write_line(indent, "    <UseDebugLibraries>" + configurations[index]->android_debug_libraries + "</UseDebugLibraries>");
			}
		}
		else
		{
			output.write_line(indent, "    <UseOfMfc>false</UseOfMfc>");
			output.write_line(indent, "    <CharacterSet>Unicode</CharacterSet>");
			if (target_version == 1200)
				output.write_line(indent, "    <PlatformToolset>v120</PlatformToolset>");
			else
				output.write_line(indent, "    <PlatformToolset>v140</PlatformToolset>");
		}
		output.write_line(indent, "  </PropertyGroup>");
	}

  	output.write_line(indent, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />");
  	output.write_line(indent, "  <ImportGroup Label=\"ExtensionSettings\">");
  	output.write_line(indent, "  </ImportGroup>");

	for (index = 0; index < configurations.size(); index++)
	{
	 	output.write_line(indent, "  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[index]->name + "'\" Label=\"PropertySheets\">");
		output.write_line(indent, "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />");

		for (index2 = 0; index2 < configurations[index]->inherited_property_sheets_vs100.size(); index2++)
		{	
			output.write_line(indent, "    <Import Project=\"" + configurations[index]->inherited_property_sheets_vs100[index2] + "\" />");
		}
		output.write_line(indent, "  </ImportGroup>");
	}

  	output.write_line(indent, "    <PropertyGroup Label=\"UserMacros\" />");

	for (index = 0; index < configurations.size(); index++)
	{
	  	output.write_line(indent, "  <PropertyGroup>");
		output.write_line(indent, "    <_ProjectFileVersion>10.0.30319.1</_ProjectFileVersion>");
		output.write_line(indent, "    <TargetName Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[index]->name + "'\">" + configurations[index]->target_name_vs100 + "</TargetName>");
		output.write_line(indent, "  </PropertyGroup>");
	}

	for (index = 0; index < configurations.size(); index++)
	{
		output.write_line(indent, "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[index]->name + "'\">");

		if (configurations[index]->use_precompiled_header == "2")
		{
 			output.write_line(indent, "    <ClCompile>");
 			output.write_line(indent, "      <PrecompiledHeader>Use</PrecompiledHeader>");
			output.write_line(indent, "      <PrecompiledHeaderFile>" + configurations[index]->precompiled_header_through + "</PrecompiledHeaderFile>");

			if (target_android)
				output.write_line(indent, "    <CompileAs>CompileAsCpp</CompileAs>");

 			output.write_line(indent, "    </ClCompile>");
		}

  		output.write_line(indent, "    <Lib>");
		if (target_android)
		{
			output.write_line(indent, "      <OutputFile>$(OutDir)" + configurations[index]->target_name_vs100 + "$(TargetExt)</OutputFile>");
		}
		else
		{
			output.write_line(indent, "      <OutputFile>$(OutDir)" + configurations[index]->target_name_vs100 + ".lib</OutputFile>");
		}
  		output.write_line(indent, "    </Lib>");
  		output.write_line(indent, "    <PostBuildEvent>");
  		output.write_line(indent, "      <Message>Installing library and API headers...</Message>");
  		output.write_line(indent, "      <Command>call install_clan" + name + ".bat \"$(TargetPath)\" \"$(TargetDir)$(TargetName).pdb\" \"$(TargetName).pdb\" \"$(Platform)\"</Command>");
  		output.write_line(indent, "    </PostBuildEvent>");
  		output.write_line(indent, "  </ItemDefinitionGroup>");
	}

	for (index = 0; index < files.size(); index++)
		files[index]->write_vs100(output, indent+1, configurations);

  	output.write_line(indent, "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />");
  	output.write_line(indent, "  <ImportGroup Label=\"ExtensionTargets\">");
  	output.write_line(indent, "  </ImportGroup>");
  	output.write_line(indent, "</Project>");
}

void MSVC8_Project::write_filters(OutputWriter &output, int indent) const
{
	std::vector<std::string>::size_type index;

	output.write_line(indent, "<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	// Note, the tools version here is always 4.0 with new projects (maybe visual studio bug?)
	output.write_line(indent, "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">");
	output.write_line(indent, "  <ItemGroup>");

	for (index = 0; index < files.size(); index++)
		files[index]->write_filter_name_vs100(output, indent, "");

	output.write_line(indent, " </ItemGroup>");

	for (index = 0; index < files.size(); index++)
		files[index]->write_filter_files_vs100(output, indent+1, "");

	output.write_line(indent, "</Project>");

}

/////////////////////////////////////////////////////////////////////////////
// MSVC8_Configuration class:

MSVC8_Configuration::MSVC8_Configuration()
{
}

MSVC8_Configuration::~MSVC8_Configuration()
{
}

/////////////////////////////////////////////////////////////////////////////
// MSVC8_FileItem class:

MSVC8_FileItem::~MSVC8_FileItem()
{
}

/////////////////////////////////////////////////////////////////////////////
// MSVC8_Filter class:

MSVC8_Filter::MSVC8_Filter()
{
}

MSVC8_Filter::~MSVC8_Filter()
{
	std::vector<MSVC8_FileItem *>::size_type index;

	for (index = 0; index < files.size(); index++)
		delete files[index];
	files.clear();
}

void MSVC8_Filter::write_filter_name_vs100(OutputWriter &output, int indent, const std::string &parent) const
{
	std::string new_filter;
	if (parent.length())
	{
		new_filter = parent + "\\" + name;
	}
	else
	{
		new_filter = name;
	}

	output.write_line(indent, "    <Filter Include=\"" + new_filter + "\">");

	// Create a new GUID:
	unsigned char *projectGUID = 0;
	GUID guid;
	UuidCreate(&guid);
	UuidToStringA(&guid, &projectGUID);
	_strupr((char *) projectGUID);
	std::string returnGUID = std::string("{") + ((char *) projectGUID) + std::string("}");
	RpcStringFreeA(&projectGUID);

	output.write_line(indent, "      <UniqueIdentifier>" + returnGUID + "</UniqueIdentifier>");
	output.write_line(indent, "    </Filter>");

	std::vector<MSVC8_FileItem *>::size_type index;
	for (index = 0; index < files.size(); index++)
	{
		files[index]->write_filter_name_vs100(output, indent, new_filter);
	}
}

void MSVC8_Filter::write_vs100(OutputWriter &output, int indent, const std::vector<MSVC8_Configuration *> &configurations) const
{
	std::vector<MSVC8_FileItem *>::size_type index;

	// It appears VS 100 places all files in the same ItemGroup

	if (indent <= 1)
		output.write_line(indent, "<ItemGroup>");

	for (index = 0; index < files.size(); index++)
		files[index]->write_vs100(output, 2, configurations);

	if (indent <= 1)
		output.write_line(indent, "</ItemGroup>");

}

void MSVC8_Filter::write_filter_files_vs100(OutputWriter &output, int indent, const std::string &parent) const
{
	std::vector<MSVC8_FileItem *>::size_type index;

	std::string new_filter;
	if (parent.length())
	{
		new_filter = parent + "\\" + name;
	}
	else
	{
		new_filter = name;
	}

	// It appears VS 100 places all files in the same ItemGroup

	if (indent <= 1)
		output.write_line(indent, "    <ItemGroup>");

	for (index = 0; index < files.size(); index++)
		files[index]->write_filter_files_vs100(output, 2, new_filter);

	if (indent <= 1)
		output.write_line(indent, "    </ItemGroup>");

}
/////////////////////////////////////////////////////////////////////////////
// MSVC8_File class:

MSVC8_File::MSVC8_File()
{
}

MSVC8_File::~MSVC8_File()
{
}

void MSVC8_File::write_filter_name_vs100(OutputWriter &output, int indent, const std::string &parent) const
{
}

void MSVC8_File::write_vs100(OutputWriter &output, int indent, const std::vector<MSVC8_Configuration *> &configurations) const
{
	if ( (relative_path.substr(relative_path.size()-2) == ".h") || (relative_path.substr(relative_path.size()-2) == ".H") )
	{
	   	output.write_line(indent, "<ClInclude Include=\"" + relative_path + "\" />");
	}
	else
	{
		if (relative_path.substr(relative_path.size()-11) == "precomp.cpp")
		{
		   	output.write_line(indent, "<ClCompile Include=\"" + relative_path + "\">");

			std::vector<std::string>::size_type index;

			for (index = 0; index < configurations.size(); index++)
			{
	  			output.write_line(indent, "  <PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[index]->name + "'\">Create</PrecompiledHeader>");
				if (configurations[index]->is_this_android)			// TODO: Is this correct?
					output.write_line(indent, "    <CompileAs>CompileAsCpp</CompileAs>");
			}


		   	output.write_line(indent, "</ClCompile>");
		}
		// Todo, Fix, hack hack hack - instead of this, 
		// we should keep a list somewhere of special files that should not have precompilation turned on!
		else if (relative_path.substr(relative_path.size()-14) == "init_guids.cpp")
		{
		   	output.write_line(indent, "<ClCompile Include=\"" + relative_path + "\">");

			std::vector<std::string>::size_type index;

			for (index = 0; index < configurations.size(); index++)
			{
	  			output.write_line(indent, "  <PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[index]->name + "'\">NotUsing</PrecompiledHeader>");
			}

		   	output.write_line(indent, "</ClCompile>");
		}
		else if (relative_path.substr(relative_path.size()-2) == ".c")
		{
		   	output.write_line(indent, "<ClCompile Include=\"" + relative_path + "\">");

			std::vector<std::string>::size_type index;

			for (index = 0; index < configurations.size(); index++)
			{
	  			output.write_line(indent, "  <PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[index]->name + "'\">");
	  			output.write_line(indent, "  </PrecompiledHeader>");
			}

		   	output.write_line(indent, "</ClCompile>");
		}
		else
		{
		   	output.write_line(indent, "<ClCompile Include=\"" + relative_path + "\" />");
		}
	}
}

void MSVC8_File::write_filter_files_vs100(OutputWriter &output, int indent, const std::string &parent) const
{
	if (relative_path.substr(relative_path.size()-2) == ".h")
	{
	   	output.write_line(indent, "<ClInclude Include=\"" + relative_path + "\" >");
		output.write_line(indent, "<Filter>" + parent + "</Filter>");
	   	output.write_line(indent, "</ClInclude>");
	}
	else
	{
	   	output.write_line(indent, "<ClCompile Include=\"" + relative_path + "\" >");
		output.write_line(indent, "<Filter>" + parent + "</Filter>");
		output.write_line(indent, "</ClCompile>");
	}
}
