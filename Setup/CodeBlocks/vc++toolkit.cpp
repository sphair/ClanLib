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
**    Lih-Hern (Lf3T-Hn4D)
**    (if your name is missing here, please add it)
*/

#include "vc++toolkit.h"
#include "codeblock_gen.h"

void setup_vc_clanlib_workspace(
	const std::list<std::string> &defines_list,
	const std::string &input_lib_dir,
	const std::string &input_include_dir,
	const std::string &output_lib_dir,
	const std::string &output_include_dir
	)
{
	Workspace workspace;

	// setup workspace paths
	workspace.input_lib_dir = input_lib_dir;
	workspace.input_include_dir = input_include_dir;
	workspace.output_lib_dir = output_lib_dir;
	workspace.output_include_dir = output_include_dir;

	// Write all library project files:
	std::list<std::string> libs_list_shared;
	std::list<std::string> libs_list_release;
	std::list<std::string> libs_list_debug;

	Project clanCore("Core", "clanCore", "core.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanSignals("Signals", "clanSignals", "signals.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanApp("Application", "clanApp", "application.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanNetwork("Network", "clanNetwork", "network.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanDisplay("Display", "clanDisplay", "display.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanSound("Sound", "clanSound", "sound.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanGL("GL", "clanGL", "gl.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanSDL("SDL", "clanSDL", "sdl.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanGUI("GUI", "clanGUI", "gui.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanGUIStyleSilver("GUIStyleSilver", "clanGUIStyleSilver", "guistylesilver.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanVorbis("Vorbis", "clanVorbis", "vorbis.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);
	Project clanMikMod("MikMod", "clanMikMod", "mikmod.h", libs_list_shared, libs_list_release, libs_list_debug, defines_list);

	// Add projects to workspace:
	workspace.projects.push_back(clanSignals);
	workspace.projects.push_back(clanCore);
	workspace.projects.push_back(clanApp);
	workspace.projects.push_back(clanNetwork);
	workspace.projects.push_back(clanDisplay);
	workspace.projects.push_back(clanSound);
	workspace.projects.push_back(clanGL);
	workspace.projects.push_back(clanSDL);
	workspace.projects.push_back(clanGUI);
	workspace.projects.push_back(clanGUIStyleSilver);
	workspace.projects.push_back(clanVorbis);
	workspace.projects.push_back(clanMikMod);

	std::list<Project>::iterator iter;
	for (iter = workspace.projects.begin(); iter != workspace.projects.end(); iter++)
		(*iter).defines_list.push_back("DIRECTINPUT_VERSION=0x0800");

	// generate workspace.
	gen_workspace("ClanLib", "Sources", workspace);
	for (iter = workspace.projects.begin(); iter != workspace.projects.end(); iter++)
		gen_vc_project(*iter, workspace);
}

void gen_vc_project(const Project &project, const Workspace &workspace)
{
	std::list<std::string> options_list, defines_list, targets, inc_dir, lib_dir;
	std::string filename = "Sources/" + project.libname + ".cbp";
	std::cout << "  Generating project " << filename << "... ";

	std::ofstream project_file(filename.c_str());

	gen_project_head(project_file);

	inc_dir.push_back(".");
	inc_dir.push_back(workspace.input_include_dir);
	lib_dir.push_back(workspace.input_lib_dir);

	// project title.
	project_file << "\t\t<Option title=\"" << project.libname << "\"/>\n";

	// compiler to use.
	project_file << "\t\t<Option compiler=\"1\"/>\n";

	project_file << "\t\t<Build>\n";

	// compile target: release
	project_file << "\t\t\t<Target title=\"Release\">\n";
	project_file << "\t\t\t\t<Option output=\"" << workspace.output_lib_dir << "/" << project.libname << ".lib\"/>\n";
	project_file << "\t\t\t\t<Option working_dir=\"\"/>\n";
	project_file << "\t\t\t\t<Option object_output=\"Release/" << project.libname << "\"/>\n";
	project_file << "\t\t\t\t<Option deps_output=\".deps\"/>\n";
	project_file << "\t\t\t\t<Option external_deps=\"\"/>\n";
	project_file << "\t\t\t\t<Option type=\"2\"/>\n";
	project_file << "\t\t\t\t<Option compiler=\"1\"/>\n";
	project_file << "\t\t\t\t<Option projectResourceIncludeDirsRelation=\"1\"/>\n";

	// generate compiler options
	options_list.push_back("MT");
	options_list.push_back("nologo");
	options_list.push_back("W3");
	options_list.push_back("GR");
	options_list.push_back("GX");
	options_list.push_back("Zi");
	options_list.push_back("O2");
	options_list.push_back("YX");
	options_list.push_back("FD");
	options_list.push_back("c");
	defines_list = project.defines_list;
	defines_list.push_back("WIN32");
	defines_list.push_back("NDEBUG");
	defines_list.push_back("_WINDOWS");
	gen_compiler_options(project_file, options_list, defines_list, inc_dir);

	// generate linker options
	options_list.clear();
	options_list.push_back("nologo");
	gen_linker_options(project_file, options_list, lib_dir);

	project_file << "\t\t\t</Target>\n";

	// compile target: debug
	project_file << "\t\t\t<Target title=\"Debug\">\n";
	project_file << "\t\t\t\t<Option output=\"" << workspace.output_lib_dir << "/" << project.libname << "d.lib\"/>\n";
	project_file << "\t\t\t\t<Option working_dir=\"\"/>\n";
	project_file << "\t\t\t\t<Option object_output=\"Debug/" << project.libname << "\"/>\n";
	project_file << "\t\t\t\t<Option deps_output=\".deps\"/>\n";
	project_file << "\t\t\t\t<Option external_deps=\"\"/>\n";
	project_file << "\t\t\t\t<Option type=\"2\"/>\n";
	project_file << "\t\t\t\t<Option compiler=\"1\"/>\n";
	project_file << "\t\t\t\t<Option projectResourceIncludeDirsRelation=\"1\"/>\n";

	// generate compiler options
	options_list.clear();
	options_list.push_back("MTd");
	options_list.push_back("nologo");
	options_list.push_back("W3");
	options_list.push_back("Gm");
	options_list.push_back("GR");
	options_list.push_back("GX");
	options_list.push_back("Zi");
	options_list.push_back("Od");
	options_list.push_back("YX");
	options_list.push_back("FD");
	options_list.push_back("GZ");
	options_list.push_back("c");
	defines_list = project.defines_list;
	defines_list.push_back("WIN32");
	defines_list.push_back("_DEBUG");
	defines_list.push_back("NDEBUG");
	defines_list.push_back("_WINDOWS");
	gen_compiler_options(project_file, options_list, defines_list, inc_dir);

	// generate linker options
	options_list.clear();
	options_list.push_back("nologo");
	gen_linker_options(project_file, options_list, lib_dir);

	project_file << "\t\t\t</Target>\n";

	project_file << "\t\t</Build>\n";

	project_file << "\t\t<ExtraCommands>\n";
	project_file << "\t\t\t<Add after=\"install_" << project.libname << ".bat\"/>\n";
	project_file << "\t\t</ExtraCommands>\n";

	targets.push_back("Release");
	targets.push_back("Debug");
#ifdef __BORLANDC__
	gen_file_units(project_file, "Sources", project.file_paths, targets);
#else
	gen_file_units(project_file, "Sources", project.files, targets);
#endif

	gen_project_foot(project_file);

	std::cout << "done." << std::endl;

	std::cout << "    Generating install script... ";

	gen_vc_install_bat(project, workspace);

	std::cout << "done." << std::endl;
}

// TODO!!!!!
// create the install_libname.bat installation batch file:
void gen_vc_install_bat(const Project &project, const Workspace &workspace)
{
	std::string bat_file = "Sources\\install_";
	bat_file += project.libname;
	bat_file += ".bat";

	std::ofstream bat(bat_file.c_str());

	std::string instdir = workspace.output_include_dir;
	instdir += "\\ClanLib";
	install_mkdir(bat, "API\\", instdir, &project);
	install_copydir(bat, "API\\", instdir, &project);
}

void install_mkdir(
	std::ofstream &dsp,
	const std::string &src_dir,
	const std::string &dest_dir,
	const Project *project)
{
	static OSVERSIONINFO versionInfo;
	static firstCall = true;
	if (firstCall)
	{
		memset(&versionInfo, 0, sizeof(OSVERSIONINFO));
		GetVersionEx(&versionInfo);
		firstCall = false;
	}

	bool win9x = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);

	if (win9x)
	{
		dsp << "if not exist \"" << dest_dir << "\\nul\"" << " mkdir \"" << dest_dir << "\"" << std::endl;
	}
	else
	{
		dsp << "if not exist \"" << dest_dir << "\"" << " mkdir \"" << dest_dir << "\"" << std::endl;
	}

	std::string path = src_dir;
	if (path[path.size() - 1] != '\\')
		path += '\\';

	std::string path_dest = dest_dir;
	if (path_dest[path_dest.size() - 1] != '\\')
		path_dest += '\\';

	if (project)
	{
		// first time call: 
		// - make sure we create Module specific directories

		path = path + project->name.c_str();
		if (path.find_first_of(path.size() - 1) != '\\')
			path += '\\';

		path_dest = path_dest + project->name.c_str();
		if (win9x)
		{
			dsp << "if not exist \"" << std::string(path_dest) << "\\nul\"" << " mkdir \"" << std::string(path_dest) << "\"" << std::endl;
		}
		else
		{
			dsp << "if not exist \"" << std::string(path_dest) << "\"" << " mkdir \"" << std::string(path_dest) << "\"" << std::endl;
		}

		if (path_dest[path_dest.size() - 1] != '\\')
			path_dest += '\\';
	}

	std::string prefix = "Sources\\";
	std::string file_pattern = prefix + path + "*.*";

	WIN32_FIND_DATA	data;
	HANDLE handle = FindFirstFile(file_pattern.c_str(), &data);
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
			install_mkdir(dsp, path + subdir, path_dest + subdir, 0);
		}
	}
	while (FindNextFile(handle, &data));
}

void install_copydir(
	std::ofstream &dsp,
	const std::string &src_dir,
	const std::string &dest_dir,
	const Project *project)
{
	std::string path = src_dir;
	if (path[path.size() - 1] != '\\')
		path += '\\';

	std::string path_dest = dest_dir.c_str();
	if (path_dest[path_dest.size() - 1] != '\\')
		path_dest += '\\';

	if (project)
	{
		// first time call: 
		// - make sure we copy the 'module' api header
		// - make sure we get API\\ModuleName\*.h in this run

		dsp << "copy \"" << LPCTSTR (path.c_str()) << LPCTSTR (project->headername.c_str()) << "\" \"" << LPCTSTR (path_dest.c_str()) << LPCTSTR (project->headername.c_str()) << "\" > nul" << std::endl; // "\"\t";

		path += project->name;
		if (path[path.size() - 1] != '\\') path += '\\';

		path_dest += project->name;
		if (path_dest[path_dest.size() - 1] != '\\') path_dest += '\\';

	}

	std::string prefix = "Sources\\";
	std::string file_pattern = prefix + path + "*.*";

	WIN32_FIND_DATA	data;
	HANDLE handle = FindFirstFile(file_pattern.c_str(), &data);
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
				dsp,
				std::string(path + subdir),
				std::string(path_dest + subdir),
				0);
		}
		else
		{
			std::string file = data.cFileName;
			dsp << "copy \"" << LPCTSTR (path.c_str()) << LPCTSTR (file.c_str()) << "\" \"" << LPCTSTR (path_dest.c_str()) << LPCTSTR (file.c_str()) << "\" > nul" << std::endl; // "\"\t";
		}

	} while (FindNextFile(handle, &data));
}
