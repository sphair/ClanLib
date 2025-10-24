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
*/

#include "precomp.h"
#include "workspace_generator_msvc.h"
#include <vector>

#ifdef UNICODE
std::string text_to_local8(const tstring &text)
{
	int local8_length = WideCharToMultiByte(CP_ACP, 0, text.data(), text.length(), 0, 0, 0, 0);
	if (local8_length <= 0)
		return std::string();
	char *buffer = new char[local8_length];
	if (buffer == 0)
		return std::string();
	local8_length = WideCharToMultiByte(CP_ACP, 0, text.data(), text.length(), buffer, local8_length, 0, 0);
	if (local8_length <= 0)
	{
		delete[] buffer;
		return std::string();
	}
	std::string s(buffer, local8_length);
	delete[] buffer;
	return s;
}

tstring local8_to_text(const std::string &local8)
{
	int text_length = MultiByteToWideChar(CP_ACP, 0, local8.data(), local8.length(), 0, 0);
	if (text_length <= 0)
		return tstring();
	WCHAR *buffer = new WCHAR[text_length];
	if (buffer == 0)
		return tstring();
	text_length = MultiByteToWideChar(CP_ACP, 0, local8.data(), local8.length(), buffer, text_length);
	if (text_length <= 0)
	{
		delete[] buffer;
		return tstring();
	}
	tstring s(buffer, text_length);
	delete[] buffer;
	return s;
}
#else
std::string text_to_local8(const tstring &text)
{
	return text;
}

tstring local8_to_text(const std::string &local8)
{
	return local8;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC construction:

void WorkspaceGenerator_MSVC::install_mkdir(
	std::ofstream &dsp,
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
		dsp << "if not exist \"" << dest_dir << "\\nul\"" << " mkdir \"" << dest_dir << "\"" << std::endl;
	}
	else
	{
		dsp << "if not exist \"" << dest_dir << "\"" << " mkdir \"" << dest_dir << "\"" << std::endl;
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
			dsp << "if not exist \"" << std::string(path_dest) << "\\nul\"" << " mkdir \"" << std::string(path_dest) << "\"" << std::endl;
		}
		else
		{
			dsp << "if not exist \"" << std::string(path_dest) << "\"" << " mkdir \"" << std::string(path_dest) << "\"" << std::endl;
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

#if _MSC_VER < 1500
		for (int i=0; exclude_from_build[i] != NULL; i++)
			if (stricmp(data.cFileName, exclude_from_build[i]) == 0) skip = true;
#else
		for (int i=0; exclude_from_build[i] != NULL; i++)
			if (_stricmp(data.cFileName, exclude_from_build[i]) == 0) skip = true;
#endif

		if (skip) continue;

		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			std::string subdir = data.cFileName;
			install_mkdir(
				dsp,
				std::string(path + subdir),
				std::string(path_dest + subdir),
				0);
		}
	} while (FindNextFileA(handle, &data));
}

void WorkspaceGenerator_MSVC::install_copydir(
	std::ofstream &dsp,
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

		dsp << "copy \"" << path.c_str() << project->headername.c_str() << "\" \"" << path_dest.c_str() << project->headername.c_str() << "\" > nul" << std::endl; // "\"\t";

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

#if _MSC_VER < 1500
		for (int i=0; exclude_from_build[i] != NULL; i++)
			if (stricmp(data.cFileName, exclude_from_build[i]) == 0) skip = true;
#else
		for (int i=0; exclude_from_build[i] != NULL; i++)
			if (_stricmp(data.cFileName, exclude_from_build[i]) == 0) skip = true;
#endif

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
			dsp << "copy \"" << path.c_str() << file.c_str() << "\" \"" << path_dest.c_str() << file.c_str() << "\" > nul" << std::endl; // "\"\t";
		}

	} while (FindNextFileA(handle, &data));
}

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC6 construction:

WorkspaceGenerator_MSVC6::WorkspaceGenerator_MSVC6()
{
}

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC6 attributes:

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC6 operations:

void WorkspaceGenerator_MSVC6::write(const Workspace &workspace)
{
	write_dsw(workspace);

	std::list<Project>::const_iterator itProject;
	for (itProject = workspace.projects.begin(); itProject != workspace.projects.end(); itProject++)
	{
		const Project &project = *itProject;
		write_dsp(workspace, project);
	}
}

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC6 implementation:

void WorkspaceGenerator_MSVC6::write_dsw(const Workspace &workspace)
{
	write_dsw_clanlib(workspace);
	write_dsw_utilities(workspace);
	write_dsw_examples(workspace);
	write_dsw_tests(workspace);
}

void WorkspaceGenerator_MSVC6::write_dsw_examples(const Workspace &workspace)
{
	std::ofstream dsw("Examples.dsw");

	write_dsw_header(dsw);

	begin_project(dsw, "ExampleAdventureScene", ".\\Examples\\AdventureScene\\AdventureScene.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleBasic2D", ".\\Examples\\Basic2D\\Basic2D.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleBlendFunctions", ".\\Examples\\BlendFunctions\\BlendFunctions.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleCanvas", ".\\Examples\\Canvas\\Canvas.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleCommandLine", ".\\Examples\\CommandLine\\CommandLine.dsp");
	add_project_dependency(dsw, "clanCore");
	end_project(dsw);

	begin_project(dsw, "ExampleCTalk", ".\\Examples\\CTalk\\CTalk.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanNetwork");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleDirectory", ".\\Examples\\Directory\\Directory.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleDisplayModes", ".\\Examples\\DisplayModes\\DisplayModes.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleExePath", ".\\Examples\\ExePath\\ExePath.dsp");
	add_project_dependency(dsw, "clanCore");
	end_project(dsw);

	begin_project(dsw, "ExampleFont", ".\\Examples\\Font\\Font.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

 	begin_project(dsw, "ExampleGLCube", ".\\Examples\\GLCube\\GLCube.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

 	begin_project(dsw, "ExampleGLFog", ".\\Examples\\GLFog\\GLFog.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

 	begin_project(dsw, "ExampleGLGUICube", ".\\Examples\\GLGUICube\\GLGUICube.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

 	begin_project(dsw, "ExampleGlyphBuffer", ".\\Examples\\GlyphBuffer\\GlyphBuffer.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleGUIDeck", ".\\Examples\\GUIDeck\\GUIDeck.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleGUILayout", ".\\Examples\\GUILayout\\GUILayout.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleGUIInputDialog", ".\\Examples\\GUIInputDialog\\GUIInputDialog.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleGUIScript", ".\\Examples\\GUIScript\\GUIScript.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleGUISimple", ".\\Examples\\GUISimple\\GUISimple.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleGUIStyle", ".\\Examples\\GUIStyle\\GUIStyle.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleGUITreeView", ".\\Examples\\GUITreeView\\GUITreeView.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleGUIWindow", ".\\Examples\\GUIWindow\\GUIWindow.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleInitDisplay", ".\\Examples\\InitDisplay\\InitDisplay.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleIRCBot", ".\\Examples\\IRCBot\\IRCBot.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanNetwork");
	end_project(dsw);

	begin_project(dsw, "ExampleJoystick", ".\\Examples\\Joystick\\Joystick.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleMikMod", ".\\Examples\\MikMod\\MikMod.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanMikMod");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleMinimum", ".\\Examples\\Minimum\\Minimum.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleMousePoll", ".\\Examples\\MousePoll\\MousePoll.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleMouseSignal", ".\\Examples\\MouseSignal\\MouseSignal.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleNetChannel", ".\\Examples\\NetChannel\\NetChannel.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanNetwork");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleNetFTP", ".\\Examples\\NetFTP\\NetFTP.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanNetwork");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExamplePacman", ".\\Examples\\Pacman\\Pacman.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanSound");
	end_project(dsw);

	begin_project(dsw, "ExamplePilotsPuzzle", ".\\Examples\\PilotsPuzzle\\PilotsPuzzle.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExamplePlayCD", ".\\Examples\\PlayCD\\PlayCD.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleResources", ".\\Examples\\Resources\\Resources.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleResourcesCustom", ".\\Examples\\ResourcesCustom\\ResourcesCustom.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleScreenShot", ".\\Examples\\ScreenShot\\ScreenShot.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleShadowOfTheBeast", ".\\Examples\\ShadowOfTheBeast\\ShadowOfTheBeast.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleStencil", ".\\Examples\\Stencil\\Stencil.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleSignals", ".\\Examples\\Signals\\Signals.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleSoundBuffer", ".\\Examples\\SoundBuffer\\SoundBuffer.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleSoundFilters", ".\\Examples\\SoundFilters\\SoundFilters.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	end_project(dsw);

	begin_project(dsw, "ExampleSpritesRTS", ".\\Examples\\SpritesRTS\\SpritesRTS.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleSoundProvider", ".\\Examples\\StreamSoundProvider\\StreamSoundProvider.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleSurface", ".\\Examples\\Surface\\Surface.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleThreadSync", ".\\Examples\\ThreadSync\\ThreadSync.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleTime", ".\\Examples\\Time\\Time.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ExampleUDPTunnel", ".\\Examples\\UDPTunnel\\UDPTunnel.dsp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanNetwork");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleVorbis", ".\\Examples\\Vorbis\\Vorbis.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanVorbis");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleXML", ".\\Examples\\XML\\XML.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleZip", ".\\Examples\\Zip\\Zip.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	end_project(dsw);

	write_dsw_tail(dsw);
}

void WorkspaceGenerator_MSVC6::write_dsw_utilities(const Workspace &workspace)
{
	std::ofstream dsw("Utilities.dsw");

	write_dsw_header(dsw);

	begin_project(dsw, "GUIEditor", ".\\Utilities\\GUIEditor\\GUIEditor.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	write_dsw_tail(dsw);
}

void WorkspaceGenerator_MSVC6::write_dsw_tests(const Workspace &workspace)
{
	std::ofstream dsw("Tests.dsw");

	write_dsw_header(dsw);

	begin_project(dsw, "CoreTest", ".\\Tests\\CoreTest\\CoreTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	end_project(dsw);

	begin_project(dsw, "RegExpTest", ".\\Tests\\RegExpTest\\RegExpTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanRegExp");
	end_project(dsw);

	begin_project(dsw, "MySQLTest", ".\\Tests\\MySQLTest\\MySQLTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanMySQL");
	end_project(dsw);

	begin_project(dsw, "SqliteTest", ".\\Tests\\SqliteTest\\SqliteTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSqlite");
	end_project(dsw);

/*
	begin_project(dsw, "Collision", ".\\Tests\\Collision\\Collision.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "Disp2Test", ".\\Tests\\Disp2Test\\Disp2Test.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "GfxBenchmark", ".\\Tests\\GfxBenchmark\\GfxBenchmark.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "TextStylerTest", ".\\Tests\\TextStylerTest\\TextStylerTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "GUITest", ".\\Tests\\GUITest\\GUITest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ListBoxTest", ".\\Tests\\ListBoxTest\\ListBoxTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGUIStyleSilver");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "AlphaTest", ".\\Tests\\AlphaTest\\AlphaTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanJPEG");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "InputButtonTest", ".\\Tests\\InputButtonTest\\InputButtonTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "InputTest", ".\\Tests\\InputTest\\InputTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "GLSpeedTest", ".\\Tests\\GLSpeedTest\\GLSpeedTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "StateTreeTest", ".\\Tests\\StateTreeTest\\StateTreeTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "SpriteTest", ".\\Tests\\SpriteTest\\SpriteTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "FontTest", ".\\Tests\\FontTest\\FontTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "TextureSizeTest", ".\\Tests\\TextureSizeTest\\TextureSizeTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ProviderTest", ".\\Tests\\PrimitivesTest\\PrimitivesTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "ProviderTest", ".\\Tests\\ProviderTest\\ProviderTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	add_project_dependency(dsw, "clanGL");
	end_project(dsw);

	begin_project(dsw, "NetSessionsTest", ".\\Tests\\NetSessionsTest\\NetSessionsTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanNetwork");
	end_project(dsw);

	begin_project(dsw, "SocketTestClient", ".\\Tests\\SocketTest\\SocketTestClient.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanNetwork");
	end_project(dsw);

	begin_project(dsw, "SocketTestServer", ".\\Tests\\SocketTest\\SocketTestServer.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanNetwork");
	end_project(dsw);
*/
	write_dsw_tail(dsw);
}

void WorkspaceGenerator_MSVC6::write_dsw_clanlib(const Workspace &workspace)
{
	std::ofstream dsw("ClanLib.dsw");

	write_dsw_header(dsw);

	std::string dsp_path(".\\Sources\\");

	std::list<Project>::const_iterator itProject;
	for (itProject = workspace.projects.begin(); itProject != workspace.projects.end(); itProject++)
	{
		const Project &project = *itProject;

		begin_project(dsw, project.libname, dsp_path+project.libname+std::string(".dsp"));

		std::list<std::string>::const_iterator it;
		for (it = project.dependencies.begin(); it != project.dependencies.end(); it++)
		{
			const std::string &dep = *it;
			add_project_dependency(dsw, dep);
		}

		end_project(dsw);
	}

	write_dsw_tail(dsw);
}

void WorkspaceGenerator_MSVC6::begin_project(
	std::ofstream &dsw,
	const std::string &name,
	const std::string &filename)
{
	dsw << "Project: \"" << name.c_str() << "\"=\"" << filename.c_str() << "\" - Package Owner=<4>" << std::endl;
	dsw << std::endl;
	dsw << "Package=<5>" << std::endl;
	dsw << "{{{" << std::endl;
	dsw << "}}}" << std::endl;
	dsw << std::endl;
	dsw << "Package=<4>" << std::endl;
	dsw << "{{{" << std::endl;
}

void WorkspaceGenerator_MSVC6::end_project(std::ofstream &dsw)
{
	dsw << "}}}" << std::endl;
	dsw << std::endl;
	dsw << "###############################################################################" << std::endl;
	dsw << std::endl;
}

void WorkspaceGenerator_MSVC6::add_project_dependency(
	std::ofstream &dsw,
	const std::string &dep_name)
{
	dsw << "    Begin Project Dependency" << std::endl;
	dsw << "    Project_Dep_Name " << dep_name.c_str() << std::endl;
	dsw << "    End Project Dependency" << std::endl;
}

void WorkspaceGenerator_MSVC6::write_dsw_header(std::ofstream &dsw)
{
	dsw << "Microsoft Developer Studio Workspace File, Format Version 6.00" << std::endl;
	dsw << "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!" << std::endl;
	dsw << std::endl;
	dsw << "###############################################################################" << std::endl;
	dsw << std::endl;
}

void WorkspaceGenerator_MSVC6::write_dsw_tail(std::ofstream &dsw)
{
	dsw << "Global:" << std::endl;
	dsw << std::endl;
	dsw << "Package=<5>" << std::endl;
	dsw << "{{{" << std::endl;
	dsw << "}}}" << std::endl;
	dsw << std::endl;
	dsw << "Package=<3>" << std::endl;
	dsw << "{{{" << std::endl;
	dsw << "}}}" << std::endl;
	dsw << std::endl;
	dsw << "###############################################################################" << std::endl;
	dsw << std::endl;
}

void WorkspaceGenerator_MSVC6::write_dsp(const Workspace &workspace, const Project &project)
{
	std::string filename = "Sources\\";
	filename += project.libname.c_str();
	filename += ".dsp";

	std::ofstream dsp(filename.c_str());

	dsp << "# Microsoft Developer Studio Project File - Name=\"" << project.libname.c_str() << "\" - Package Owner=<4>" << std::endl;
	dsp << "# Microsoft Developer Studio Generated Build File, Format Version 6.00" << std::endl;
	dsp << "# ** DO NOT EDIT **" << std::endl;
	dsp << std::endl;
	dsp << "# TARGTYPE \"Win32 (x86) Static Library\" 0x0104" << std::endl;
	dsp << std::endl;
	dsp << "CFG=" << project.libname.c_str() << " - Win32 Release" << std::endl;
	dsp << "!MESSAGE This is not a valid makefile. To build this project using NMAKE," << std::endl;
	dsp << "!MESSAGE use the Export Makefile command and run" << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << "!MESSAGE NMAKE /f \"" << project.libname.c_str() << ".mak\"." << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << "!MESSAGE You can specify a configuration when running NMAKE" << std::endl;
	dsp << "!MESSAGE by defining the macro CFG on the command line. For example:" << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << "!MESSAGE NMAKE /f \"" << project.libname.c_str() << ".mak\" CFG=\"" << project.libname.c_str() << " - Win32 Release\"" << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << "!MESSAGE Possible choices for configuration are:" << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << "!MESSAGE \"" << project.libname.c_str() << " - Win32 Release\" (based on \"Win32 (x86) Static Library\")" << std::endl;
	dsp << "!MESSAGE \"" << project.libname.c_str() << " - Win32 Debug\" (based on \"Win32 (x86) Static Library\")" << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << std::endl;
	dsp << "# Begin Project" << std::endl;
	dsp << "# PROP AllowPerConfigDependencies 0" << std::endl;
	dsp << "# PROP Scc_ProjName \"\"" << std::endl;
	dsp << "# PROP Scc_LocalPath \"\"" << std::endl;
	dsp << "CPP=cl.exe" << std::endl;
	dsp << "RSC=rc.exe" << std::endl;
	dsp << std::endl;
	dsp << "!IF  \"$(CFG)\" == \"" << project.libname.c_str() << " - Win32 Release\"" << std::endl;
	dsp << std::endl;
	dsp << "# PROP BASE Use_MFC 0" << std::endl;
	dsp << "# PROP BASE Use_Debug_Libraries 0" << std::endl;
	dsp << "# PROP BASE Output_Dir \"../Release\"" << std::endl;
	dsp << "# PROP BASE Intermediate_Dir \"../Release/" << project.libname.c_str() << "\"" << std::endl;
	dsp << "# PROP BASE Target_Dir \"\"" << std::endl;
	dsp << "# PROP Use_MFC 0" << std::endl;
	dsp << "# PROP Use_Debug_Libraries 0" << std::endl;
	dsp << "# PROP Output_Dir \"../Release\"" << std::endl;
	dsp << "# PROP Intermediate_Dir \"../Release/" << project.libname.c_str() << "\"" << std::endl;
	dsp << "# PROP Target_Dir \"\"" << std::endl;
	dsp << "# ADD BASE CPP /MT /nologo /W3 /GR /GX /Zi /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\"";

	// Precompiled headers
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
				if (project.name == "GUI")
					precomp_header = precomp_header.substr(pos+1);
				else if (pos == std::string::npos)
					precomp_header = project.name + "/" + precomp_header;
				else
					precomp_header = project.name + "/" + precomp_header.substr(pos+1);
				break;
			}
		}
	}

	if (has_precomp)
		dsp << " /Yu\"" << precomp_header << "\" /FD /c" << std::endl;
	else
		dsp << " /YX /FD /c" << std::endl;


	dsp << "# ADD CPP /nologo /MT /W3 /GR /GX /Zi /O2 /I \".\"";
	if (!workspace.input_include_dir.empty()) dsp << " /I \"" << workspace.input_include_dir.c_str() << "\"";
	dsp << " /D \"NDEBUG\" /D \"WIN32\" /D \"_WINDOWS\"";
	{
		for (
			std::list<std::string>::const_iterator it = project.defines_list.begin();
			it != project.defines_list.end();
			it++)
		{
			dsp << " /D \"" << (*it).c_str() << "\"";
		}
	}

	if (has_precomp)
		dsp << " /Yu\"" << precomp_header << "\" /FD /c" << std::endl;
	else
		dsp << " /YX /FD /c" << std::endl;

	dsp << "# ADD BASE RSC /l 0x409 /d \"NDEBUG\"" << std::endl;
	dsp << "# ADD RSC /l 0x409 /d \"NDEBUG\"" << std::endl;
	dsp << "BSC32=bscmake.exe" << std::endl;
	dsp << "# ADD BASE BSC32 /nologo" << std::endl;
	dsp << "# ADD BSC32 /nologo" << std::endl;
	dsp << "LIB32=link.exe -lib" << std::endl;
	dsp << "# ADD BASE LIB32 /nologo";
	if (!workspace.input_lib_dir.empty()) dsp << " /libpath:\"" << workspace.input_lib_dir.c_str() << "\"";
	dsp << std::endl;

	dsp << "# ADD LIB32 ";
	{
		for (
			std::list<std::string>::const_iterator it = project.libs_shared.begin();
			it != project.libs_shared.end();
			it++)
		{
			dsp << " " << (*it).c_str() << "";
		}
	}
	{
		for (
			std::list<std::string>::const_iterator it = project.libs_release.begin();
			it != project.libs_release.end();
			it++)
		{
			dsp << " " << (*it).c_str() << "";
		}
	}
	dsp << " /nologo /out:\"" << workspace.output_lib_dir.c_str() << "\\" << project.libname.c_str() << "-static-mt.lib\"" << std::endl;

	dsp << "# Begin Special Build Tool" << std::endl;
	dsp << "SOURCE=\"$(InputPath)\"" << std::endl;
	dsp << "PostBuild_Desc=Installing library and API headers..." << std::endl;
	dsp << "PostBuild_Cmds=call install_" << project.libname.c_str() << ".bat\t";
//	dsp << "copy ..\\Release\\" << project.libname.c_str() << "-static-mt.lib \"" << output_lib_dir.c_str() << "\"";

	// create the install_libname.bat installation batch file:
	{
		std::string bat_file = "Sources\\install_";
		bat_file += project.libname.c_str();
		bat_file += ".bat";

		std::ofstream bat(bat_file.c_str());

		std::string instdir = workspace.output_include_dir.c_str();
		instdir += "\\ClanLib";
		install_mkdir(bat, "API\\", std::string(instdir), &project);
		install_copydir(bat, "API\\", std::string(instdir), &project);
	}

	dsp << std::endl;
	dsp << "# End Special Build Tool" << std::endl;

	dsp << std::endl;
	dsp << "!ELSEIF  \"$(CFG)\" == \"" << project.libname.c_str() << " - Win32 Debug\"" << std::endl;
	dsp << std::endl;
	dsp << "# PROP BASE Use_MFC 0" << std::endl;
	dsp << "# PROP BASE Use_Debug_Libraries 1" << std::endl;
	dsp << "# PROP BASE Output_Dir \"../Debug\"" << std::endl;
	dsp << "# PROP BASE Intermediate_Dir \"../Debug/" << project.libname.c_str() << "\"" << std::endl;
	dsp << "# PROP BASE Target_Dir \"\"" << std::endl;
	dsp << "# PROP Use_MFC 0" << std::endl;
	dsp << "# PROP Use_Debug_Libraries 1" << std::endl;
	dsp << "# PROP Output_Dir \"../Debug\"" << std::endl;
	dsp << "# PROP Intermediate_Dir \"../Debug/" << project.libname.c_str() << "\"" << std::endl;
	dsp << "# PROP Target_Dir \"\"" << std::endl;
	dsp << "# ADD BASE CPP /MTd /nologo /W3 /Gm /GR /GX /Zi /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\"";

	if (has_precomp)
		dsp << " /Yu\"" << precomp_header << "\" /FD /GZ /c" << std::endl;
	else
		dsp << " /YX /FD /GZ /c" << std::endl;


	dsp << "# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /I \".\"";
	if (!workspace.input_include_dir.empty()) dsp << " /I \"" << workspace.input_include_dir.c_str() << "\"";
	dsp << " /D \"NDEBUG\" /D \"WIN32\" /D \"_WINDOWS\"";

	{
		for (
			std::list<std::string>::const_iterator it = project.defines_list.begin();
			it != project.defines_list.end();
			it++)
		{
			dsp << " /D \"" << (*it).c_str() << "\"";
		}
	}
	if (has_precomp)
		dsp << " /Yu\"" << precomp_header << "\" /FD /GZ /c" << std::endl;
	else
		dsp << " /YX /FD /GZ /c" << std::endl;

	dsp << "# ADD BASE RSC /l 0x409 /d \"_DEBUG\"" << std::endl;
	dsp << "# ADD RSC /l 0x409 /d \"_DEBUG\"" << std::endl;
	dsp << "BSC32=bscmake.exe" << std::endl;
	dsp << "# ADD BASE BSC32 /nologo" << std::endl;
	dsp << "# ADD BSC32 /nologo" << std::endl;
	dsp << "LIB32=link.exe -lib" << std::endl;
	dsp << "# ADD BASE LIB32 /nologo";
	if (!workspace.input_lib_dir.empty()) dsp << " /libpath:\"" << workspace.input_lib_dir.c_str() << "\"";
	dsp << std::endl;

	dsp << "# ADD LIB32 ";
	{
		for (
			std::list<std::string>::const_iterator it = project.libs_shared.begin();
			it != project.libs_shared.end();
			it++)
		{
			dsp << " " << (*it).c_str() << "";
		}
	}
	{
		for (
			std::list<std::string>::const_iterator it = project.libs_debug.begin();
			it != project.libs_debug.end();
			it++)
		{
			dsp << " " << (*it).c_str() << "";
		}
	}
//	dsp << " /nologo /out:\"../Debug/" << project.libname.c_str() << "-static-mt-debug.lib\"" << std::endl;
	dsp << " /nologo /out:\"" << workspace.output_lib_dir.c_str() << "\\" << project.libname.c_str() << "-static-mt-debug.lib\"" << std::endl;

	dsp << "# Begin Special Build Tool" << std::endl;
	dsp << "SOURCE=\"$(InputPath)\"" << std::endl;
	dsp << "PostBuild_Desc=Installing library and API headers..." << std::endl;
	dsp << "PostBuild_Cmds=call install_" << project.libname.c_str() << ".bat\t";

//	dsp << "copy ..\\Debug\\" << project.libname.c_str() << "d.lib \"" << workspace.output_lib_dir.c_str() << "\"";

	dsp << std::endl;
	dsp << "# End Special Build Tool" << std::endl;

	dsp << std::endl;
	dsp << "!ENDIF " << std::endl;
	dsp << std::endl;
	dsp << "# Begin Target" << std::endl;
	dsp << std::endl;
	dsp << "# Name \"" << project.libname.c_str() << " - Win32 Release\"" << std::endl;
	dsp << "# Name \"" << project.libname.c_str() << " - Win32 Debug\"" << std::endl;

	// Generate list of source files:
	generate_source_files(dsp, project);

	// End .dsp file:
	dsp << "# End Target" << std::endl;
	dsp << "# End Project" << std::endl;
}

void WorkspaceGenerator_MSVC6::generate_source_files(std::ofstream &dsp, const Project &project)
{
	std::string chop_str = "Sources\\";
	unsigned int chop_length = chop_str.length();

	std::list<std::string>::iterator itCur, itNew;
	std::list<std::string> cur_path;

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
					end_group(dsp);
					itCur++;
				}
				break;
			}

			itCur++;
			itNew++;
		}
		while (itNew != new_path.end())
		{
			if (file.find("API") == std::string::npos) begin_group(dsp, *itNew + " files");
			else begin_group(dsp, *itNew);
			itNew++;
		}

		cur_path = new_path;

		add_file(dsp, file, project);
	}

	for (itCur = cur_path.begin(); itCur != cur_path.end(); itCur++) end_group(dsp);
}

std::list<std::string> WorkspaceGenerator_MSVC6::extract_path(const std::string &fullname)
{
	std::list<std::string> path;
	int pos = 0;
	int old_pos = 0;

	while (true)
	{
		int pos = fullname.find("\\", old_pos);
		if (pos == std::string::npos) break;

		path.push_back(fullname.substr(old_pos, pos-old_pos));

		old_pos = pos + 1;
	}

	return path;
}

void WorkspaceGenerator_MSVC6::begin_group(std::ofstream &dsp, const std::string &group_name)
{
	dsp << "# Begin Group \"" << group_name.c_str() << "\"" << std::endl;
	dsp << std::endl;
	dsp << "# PROP Default_Filter \"\"" << std::endl;
}

void WorkspaceGenerator_MSVC6::end_group(std::ofstream &dsp)
{
	dsp << "# End Group" << std::endl;
}

void WorkspaceGenerator_MSVC6::add_file(std::ofstream &dsp, const std::string &filename, const Project &project)
{
	dsp << "# Begin Source File" << std::endl;
	dsp << std::endl;
	dsp << "SOURCE=" << filename.c_str() << std::endl;

	if (filename.find("precomp.cpp") != std::string::npos)
	{
		std::string filename_header = filename.substr(0, filename.length()-4) + ".h";
		std::string::size_type pos = filename_header.find_last_of("/\\");
		if (project.name == "GUI")
			filename_header = filename_header.substr(pos+1);

		dsp << "# ADD CPP /Yc\"" << filename_header << "\"" << std::endl;
	}

	dsp << "# End Source File" << std::endl;
}

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC7 construction:

WorkspaceGenerator_MSVC7::WorkspaceGenerator_MSVC7()
: target_version(700)
{
}

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC7 attributes:

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC7 operations:

void WorkspaceGenerator_MSVC7::write(const Workspace &param_workspace)
{
	workspace = param_workspace;

	std::ofstream sln("ClanLib.sln");

	if (target_version == 800)
	{
		sln << "Microsoft Visual Studio Solution File, Format Version 9.00" << std::endl;
		sln << "# Visual Studio 2005" << std::endl;
		// HAHAHA, Microsoft you're cracking me up with that comment :)
	}
	else if (target_version == 710)
		sln << "Microsoft Visual Studio Solution File, Format Version 8.00" << std::endl;
	else
		sln << "Microsoft Visual Studio Solution File, Format Version 7.00" << std::endl;

	// Write sln project:

	std::string vcproj_path(".\\Sources\\");
	std::vector<std::string> projectGUIDs;

	std::list<Project>::const_iterator itProject;
	for (itProject = workspace.projects.begin(); itProject != workspace.projects.end(); itProject++)
	{
		const Project &project = *itProject;
		projectGUIDs.push_back(write_project(sln, project, vcproj_path));
/*
		begin_project(dsw, project.libname, dsp_path+project.libname+std::string(".dsp"));

		std::list<std::string>::const_iterator it;
		for (it = project.dependencies.begin(); it != project.dependencies.end(); it++)
		{
			const std::string &dep = *it;
			add_project_dependency(dsw, dep);
		}

		end_project(dsw);
*/
	}

	sln << "Global" << std::endl;
	sln << "	GlobalSection(SolutionConfiguration) = preSolution" << std::endl;
	sln << "		ConfigName.0 = Static MT Debug" << std::endl;
	sln << "		ConfigName.1 = Static MT Release" << std::endl;
	sln << "		ConfigName.2 = Static MTDLL Debug" << std::endl;
	sln << "		ConfigName.3 = Static MTDLL Release" << std::endl;
	sln << "	EndGlobalSection" << std::endl;
	if (target_version == 700)
	{
		sln << "	GlobalSection(ProjectDependencies) = postSolution" << std::endl;
		sln << "	EndGlobalSection" << std::endl;
	}
	sln << "	GlobalSection(ProjectConfiguration) = postSolution" << std::endl;
	for (std::vector<std::string>::size_type i=0; i<projectGUIDs.size(); i++)
	{
		sln << "		" << projectGUIDs[i].c_str() << ".Static MT Debug.ActiveCfg = Static MT Debug|Win32" << std::endl;
		sln << "		" << projectGUIDs[i].c_str() << ".Static MT Debug.Build.0 = Static MT Debug|Win32" << std::endl;
		sln << "		" << projectGUIDs[i].c_str() << ".Static MT Release.ActiveCfg = Static MT Release|Win32" << std::endl;
		sln << "		" << projectGUIDs[i].c_str() << ".Static MT Release.Build.0 = Static MT Release|Win32" << std::endl;
		sln << "		" << projectGUIDs[i].c_str() << ".Static MTDLL Debug.ActiveCfg = Static MTDLL Debug|Win32" << std::endl;
		sln << "		" << projectGUIDs[i].c_str() << ".Static MTDLL Debug.Build.0 = Static MTDLL Debug|Win32" << std::endl;
		sln << "		" << projectGUIDs[i].c_str() << ".Static MTDLL Release.ActiveCfg = Static MTDLL Release|Win32" << std::endl;
		sln << "		" << projectGUIDs[i].c_str() << ".Static MTDLL Release.Build.0 = Static MTDLL Release|Win32" << std::endl;
	}
	if (target_version == 800)
	{
		sln << "	GlobalSection(SolutionProperties) = preSolution" << std::endl;
		sln << "		HideSolutionNode = FALSE" << std::endl;
		sln << "	EndGlobalSection" << std::endl;
	}
	else
	{
		sln << "	EndGlobalSection" << std::endl;
		sln << "	GlobalSection(ExtensibilityGlobals) = postSolution" << std::endl;
		sln << "	EndGlobalSection" << std::endl;
		sln << "	GlobalSection(ExtensibilityAddIns) = postSolution" << std::endl;
		sln << "	EndGlobalSection" << std::endl;
	}
	sln << "EndGlobal" << std::endl;
}

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC7 implementation:

std::string WorkspaceGenerator_MSVC7::get_project_guid(const std::string &project)
{
	std::string returnGUID;

	// Check if we already made a GUID earlier. Reuse it if we did.
	LONG result;
	HKEY key = 0;
	result = RegCreateKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Clansoft\\ClanLib\\ProjectGUIDs", &key);
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
			returnGUID.length()+1);
		RegCloseKey(key);
	}

	return returnGUID;
}

std::string WorkspaceGenerator_MSVC7::write_project(std::ofstream &sln, const Project &project, const std::string &vcproj_path)
{
	// Write sln project:
	std::string projectName = project.libname;
	std::string projectFilename = vcproj_path + project.libname + ".vcproj";
	std::string returnGUID = get_project_guid(projectName);

	sln << "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"" << projectName.c_str() << "\", \"" << projectFilename.c_str() << "\", \"" << returnGUID.c_str() << "\"" << std::endl;
	if (target_version == 710)
	{
		sln << "\tProjectSection(ProjectDependencies) = postProject" << std::endl;
		sln << "\tEndProjectSection" << std::endl;
	}
	sln << "EndProject" << std::endl;

	// Write vcproj file:
	std::string includePath = ".";
	if (!workspace.input_include_dir.empty()) includePath += "," + workspace.input_include_dir;

	std::string defines = "WIN32,_WINDOWS";
	std::list<std::string>::const_iterator it;
	for (it = project.defines_list.begin(); it != project.defines_list.end(); ++it)
	{
		defines += "," + (*it);
	}

	std::string precompiledHeaderThrough;
	std::string precompiledHeaderFileMTDebug;
	std::string precompiledHeaderFileMTRelease;
	std::string precompiledHeaderFileMTDLLDebug;
	std::string precompiledHeaderFileMTDLLRelease;
	std::string assemblerListingLocationMTDebug = ".\\../Debug/"+projectName+"-static-mt/";
	std::string assemblerListingLocationMTRelease = ".\\../Release/"+projectName+"-static-mt/";
	std::string assemblerListingLocationMTDLLDebug = ".\\../Debug/"+projectName+"-static-mtdll/";
	std::string assemblerListingLocationMTDLLRelease = ".\\../Release/"+projectName+"-static-mtdll/";
	std::string objectFileMTDebug = ".\\../Debug/"+projectName+"-static-mt/";
	std::string objectFileMTRelease = ".\\../Release/"+projectName+"-static-mt/";
	std::string objectFileMTDLLDebug = ".\\../Debug/"+projectName+"-static-mtdll/";
	std::string objectFileMTDLLRelease = ".\\../Release/"+projectName+"-static-mtdll/";
	std::string programDataBaseFileNameMTDebug = workspace.output_lib_dir + "\\" + project.libname + "-static-mt-debug.pdb"; // ".\\../Debug/"+projectName+"-static-mt/";
	std::string programDataBaseFileNameMTRelease = workspace.output_lib_dir + "\\" + project.libname + "-static-mt.pdb"; // ".\\../Release/"+projectName+"-static-mt/";
	std::string programDataBaseFileNameMTDLLDebug = workspace.output_lib_dir + "\\" + project.libname + "-static-mtdll-debug.pdb"; // ".\\../Debug/"+projectName+"-static-mtdll/";
	std::string programDataBaseFileNameMTDLLRelease = workspace.output_lib_dir + "\\" + project.libname + "-static-mtdll.pdb"; // ".\\../Release/"+projectName+"-static-mtdll/";
	std::string additionalOptionsDebug;
	std::string additionalOptionsRelease;
	std::string outputFileMTDebug = workspace.output_lib_dir + "\\" + project.libname + "-static-mt-debug.lib";
	std::string outputFileMTRelease = workspace.output_lib_dir + "\\" + project.libname + "-static-mt.lib";
	std::string outputFileMTDLLDebug = workspace.output_lib_dir + "\\" + project.libname + "-static-mtdll-debug.lib";
	std::string outputFileMTDLLRelease = workspace.output_lib_dir + "\\" + project.libname + "-static-mtdll.lib";
	std::string commandLine = "call install_" + project.libname + ".bat";

	// Precompiled headers
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
				if (project.name == "GUI")
					precomp_header = precomp_header.substr(pos+1);
				else if (pos == std::string::npos)
					precomp_header = project.name + "\\" + precomp_header;
				else
					precomp_header = project.name + "\\" + precomp_header.substr(pos+1);
				break;
			}
		}
	}

	if (has_precomp)
	{
		precompiledHeaderThrough = precomp_header;
		precompiledHeaderFileMTDebug = ".\\../Debug/"+projectName+"-static-mt/"+projectName+".pch";
		precompiledHeaderFileMTRelease = ".\\../Release/"+projectName+"-static-mt/"+projectName+".pch";
		precompiledHeaderFileMTDLLDebug = ".\\../Debug/"+projectName+"-static-mtdll/"+projectName+".pch";
		precompiledHeaderFileMTDLLRelease = ".\\../Release/"+projectName+"-static-mtdll/"+projectName+".pch";
	}

	{
		for (
			std::list<std::string>::const_iterator it = project.libs_shared.begin();
			it != project.libs_shared.end();
			it++)
		{
			additionalOptionsDebug += (*it) + " ";
			additionalOptionsRelease += (*it) + " ";
		}
	}
	{
		for (
			std::list<std::string>::const_iterator it = project.libs_debug.begin();
			it != project.libs_debug.end();
			it++)
		{
			additionalOptionsDebug += (*it) + " ";
		}
	}
	{
		for (
			std::list<std::string>::const_iterator it = project.libs_release.begin();
			it != project.libs_release.end();
			it++)
		{
			additionalOptionsRelease += (*it) + " ";
		}
	}

	std::ofstream vcproj(projectFilename.c_str());
	vcproj << "<?xml version=\"1.0\" encoding = \"Windows-1252\"?>" << std::endl;
	vcproj << "<VisualStudioProject" << std::endl;
	vcproj << "	ProjectType=\"Visual C++\"" << std::endl;
	if (target_version == 800)
		vcproj << "	Version=\"8.00\"" << std::endl;
	else if (target_version == 710)
		vcproj << "	Version=\"7.10\"" << std::endl;
	else
		vcproj << "	Version=\"7.00\"" << std::endl;
	vcproj << "	Name=\"" << project.libname.c_str() << "\"" << std::endl;
	vcproj << "	SccProjectName=\"\"" << std::endl;
	vcproj << "	SccLocalPath=\"\">" << std::endl;
	vcproj << "	<Platforms>" << std::endl;
	vcproj << "		<Platform" << std::endl;
	vcproj << "			Name=\"Win32\"/>" << std::endl;
	vcproj << "	</Platforms>" << std::endl;
	if (target_version == 800)
	{
		vcproj << "	<ToolFiles>" << std::endl;
		vcproj << "	</ToolFiles>" << std::endl;
	}
	vcproj << "	<Configurations>" << std::endl;
	vcproj << "		<Configuration" << std::endl;
	vcproj << "			Name=\"Static MT Debug|Win32\"" << std::endl;
	vcproj << "			OutputDirectory=\".\\../Debug\"" << std::endl;
	vcproj << "			IntermediateDirectory=\".\\../Debug/" << project.libname.c_str() << "-static-mt\"" << std::endl;
	vcproj << "			ConfigurationType=\"4\"" << std::endl;
	if (target_version == 800)
	{
		vcproj << "			InheritedPropertySheets=\"$(VCInstallDir)VCProjectDefaults\\UpgradeFromVC71.vsprops\"" << std::endl;
	}
	vcproj << "			UseOfMFC=\"0\"" << std::endl;
	if (target_version == 800)
		vcproj << "			ATLMinimizesCRunTimeLibraryUsage=\"false\">" << std::endl;
	else
		vcproj << "			ATLMinimizesCRunTimeLibraryUsage=\"FALSE\">" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCCLCompilerTool\"" << std::endl;
	vcproj << "				Optimization=\"0\"" << std::endl;
	vcproj << "				AdditionalIncludeDirectories=\"" << includePath.c_str() << "\"" << std::endl;
	vcproj << "				PreprocessorDefinitions=\"DEBUG," << defines.c_str() << "\"" << std::endl;
	vcproj << "				BasicRuntimeChecks=\"3\"" << std::endl;
	vcproj << "				RuntimeLibrary=\"1\"" << std::endl;
	if (target_version == 800)
		vcproj << "				RuntimeTypeInfo=\"true\"" << std::endl;
	else
		vcproj << "				RuntimeTypeInfo=\"TRUE\"" << std::endl;
	if (precompiledHeaderThrough.empty())
	{
		vcproj << "				UsePrecompiledHeader=\"2\"" << std::endl;
	}
	else
	{
		vcproj << "				UsePrecompiledHeader=\"3\"" << std::endl;
		vcproj << "				PrecompiledHeaderThrough=\"" << precompiledHeaderThrough.c_str() << "\"" << std::endl;
		vcproj << "				PrecompiledHeaderFile=\"" << precompiledHeaderFileMTDebug.c_str() << "\"" << std::endl;
	}
	vcproj << "				AssemblerListingLocation=\"" << assemblerListingLocationMTDebug.c_str() << "\"" << std::endl;
	vcproj << "				ObjectFile=\"" << objectFileMTDebug.c_str() << "\"" << std::endl;
	vcproj << "				ProgramDataBaseFileName=\"" << programDataBaseFileNameMTDebug.c_str() << "\"" << std::endl;
	vcproj << "				WarningLevel=\"3\"" << std::endl;
	if (target_version == 800)
		vcproj << "				SuppressStartupBanner=\"true\"" << std::endl;
	else
		vcproj << "				SuppressStartupBanner=\"TRUE\"" << std::endl;
	vcproj << "				DebugInformationFormat=\"3\"" << std::endl;
	vcproj << "				CompileAs=\"0\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCCustomBuildTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCLibrarianTool\"" << std::endl;
	vcproj << "				AdditionalOptions=\"" << additionalOptionsDebug.c_str() << "\"" << std::endl;
	vcproj << "				OutputFile=\"" << outputFileMTDebug.c_str() << "\"" << std::endl;
	if (target_version == 800)
		vcproj << "				SuppressStartupBanner=\"true\"/>" << std::endl;
	else
		vcproj << "				SuppressStartupBanner=\"TRUE\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCMIDLTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPostBuildEventTool\"" << std::endl;
	vcproj << "				Description=\"Installing library and API headers...\"" << std::endl;
	vcproj << "				CommandLine=\"" << commandLine.c_str() << "\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPreBuildEventTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPreLinkEventTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCResourceCompilerTool\"" << std::endl;
	vcproj << "				PreprocessorDefinitions=\"_DEBUG\"" << std::endl;
	vcproj << "				Culture=\"1033\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCWebServiceProxyGeneratorTool\"/>" << std::endl;
	if (target_version == 710)
	{
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCXMLDataGeneratorTool\"/>" << std::endl;
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCManagedWrapperGeneratorTool\"/>" << std::endl;
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCAuxiliaryManagedWrapperGeneratorTool\"/>" << std::endl;
	}
	vcproj << "		</Configuration>" << std::endl;
	vcproj << "		<Configuration" << std::endl;
	vcproj << "			Name=\"Static MTDLL Debug|Win32\"" << std::endl;
	vcproj << "			OutputDirectory=\".\\../Debug\"" << std::endl;
	vcproj << "			IntermediateDirectory=\".\\../Debug/" << project.libname.c_str() << "-static-mtdll\"" << std::endl;
	vcproj << "			ConfigurationType=\"4\"" << std::endl;
	if (target_version == 800)
	{
		vcproj << "			InheritedPropertySheets=\"$(VCInstallDir)VCProjectDefaults\\UpgradeFromVC71.vsprops\"" << std::endl;
	}
	vcproj << "			UseOfMFC=\"0\"" << std::endl;
	if (target_version == 800)
		vcproj << "			ATLMinimizesCRunTimeLibraryUsage=\"false\">" << std::endl;
	else
		vcproj << "			ATLMinimizesCRunTimeLibraryUsage=\"FALSE\">" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCCLCompilerTool\"" << std::endl;
	vcproj << "				Optimization=\"0\"" << std::endl;
	vcproj << "				AdditionalIncludeDirectories=\"" << includePath.c_str() << "\"" << std::endl;
	vcproj << "				PreprocessorDefinitions=\"DEBUG," << defines.c_str() << "\"" << std::endl;
	vcproj << "				BasicRuntimeChecks=\"3\"" << std::endl;
	vcproj << "				RuntimeLibrary=\"3\"" << std::endl;
	if (target_version == 800)
		vcproj << "				RuntimeTypeInfo=\"true\"" << std::endl;
	else
		vcproj << "				RuntimeTypeInfo=\"TRUE\"" << std::endl;
	if (precompiledHeaderThrough.empty())
	{
		vcproj << "				UsePrecompiledHeader=\"2\"" << std::endl;
	}
	else
	{
		vcproj << "				UsePrecompiledHeader=\"3\"" << std::endl;
		vcproj << "				PrecompiledHeaderThrough=\"" << precompiledHeaderThrough.c_str() << "\"" << std::endl;
		vcproj << "				PrecompiledHeaderFile=\"" << precompiledHeaderFileMTDLLDebug.c_str() << "\"" << std::endl;
	}
	vcproj << "				AssemblerListingLocation=\"" << assemblerListingLocationMTDLLDebug.c_str() << "\"" << std::endl;
	vcproj << "				ObjectFile=\"" << objectFileMTDLLDebug.c_str() << "\"" << std::endl;
	vcproj << "				ProgramDataBaseFileName=\"" << programDataBaseFileNameMTDLLDebug.c_str() << "\"" << std::endl;
	vcproj << "				WarningLevel=\"3\"" << std::endl;
	if (target_version == 800)
		vcproj << "				SuppressStartupBanner=\"true\"" << std::endl;
	else
		vcproj << "				SuppressStartupBanner=\"TRUE\"" << std::endl;
	vcproj << "				DebugInformationFormat=\"3\"" << std::endl;
	vcproj << "				CompileAs=\"0\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCCustomBuildTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCLibrarianTool\"" << std::endl;
	vcproj << "				AdditionalOptions=\"" << additionalOptionsDebug.c_str() << "\"" << std::endl;
	vcproj << "				OutputFile=\"" << outputFileMTDLLDebug.c_str() << "\"" << std::endl;
	if (target_version == 800)
		vcproj << "				SuppressStartupBanner=\"true\"/>" << std::endl;
	else
		vcproj << "				SuppressStartupBanner=\"TRUE\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCMIDLTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPostBuildEventTool\"" << std::endl;
	vcproj << "				Description=\"Installing library and API headers...\"" << std::endl;
	vcproj << "				CommandLine=\"" << commandLine.c_str() << "\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPreBuildEventTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPreLinkEventTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCResourceCompilerTool\"" << std::endl;
	vcproj << "				PreprocessorDefinitions=\"_DEBUG\"" << std::endl;
	vcproj << "				Culture=\"1033\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCWebServiceProxyGeneratorTool\"/>" << std::endl;
	if (target_version == 710)
	{
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCXMLDataGeneratorTool\"/>" << std::endl;
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCManagedWrapperGeneratorTool\"/>" << std::endl;
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCAuxiliaryManagedWrapperGeneratorTool\"/>" << std::endl;
	}
	vcproj << "		</Configuration>" << std::endl;
	vcproj << "		<Configuration" << std::endl;
	vcproj << "			Name=\"Static MT Release|Win32\"" << std::endl;
	vcproj << "			OutputDirectory=\".\\../Release\"" << std::endl;
	vcproj << "			IntermediateDirectory=\".\\../Release/" << project.libname.c_str() << "-static-mt\"" << std::endl;
	vcproj << "			ConfigurationType=\"4\"" << std::endl;
	if (target_version == 800)
	{
		vcproj << "			InheritedPropertySheets=\"$(VCInstallDir)VCProjectDefaults\\UpgradeFromVC71.vsprops\"" << std::endl;
	}
	vcproj << "			UseOfMFC=\"0\"" << std::endl;
	if (target_version == 800)
		vcproj << "			ATLMinimizesCRunTimeLibraryUsage=\"false\">" << std::endl;
	else
		vcproj << "			ATLMinimizesCRunTimeLibraryUsage=\"FALSE\">" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCCLCompilerTool\"" << std::endl;
	vcproj << "				InlineFunctionExpansion=\"1\"" << std::endl;
	vcproj << "				AdditionalIncludeDirectories=\"" << includePath.c_str() << "\"" << std::endl;
	vcproj << "				PreprocessorDefinitions=\"NDEBUG," << defines.c_str() << "\"" << std::endl;
	if (target_version == 800)
	{
		vcproj << "				StringPooling=\"true\"" << std::endl;
		vcproj << "				RuntimeLibrary=\"0\"" << std::endl;
		vcproj << "				EnableFunctionLevelLinking=\"true\"" << std::endl;
		vcproj << "				RuntimeTypeInfo=\"true\"" << std::endl;
	}
	else
	{
		vcproj << "				StringPooling=\"TRUE\"" << std::endl;
		vcproj << "				RuntimeLibrary=\"0\"" << std::endl;
		vcproj << "				EnableFunctionLevelLinking=\"TRUE\"" << std::endl;
		vcproj << "				RuntimeTypeInfo=\"TRUE\"" << std::endl;
	}
	if (precompiledHeaderThrough.empty())
	{
		vcproj << "				UsePrecompiledHeader=\"2\"" << std::endl;
	}
	else
	{
		vcproj << "				UsePrecompiledHeader=\"3\"" << std::endl;
		vcproj << "				PrecompiledHeaderThrough=\"" << precompiledHeaderThrough.c_str() << "\"" << std::endl;
		vcproj << "				PrecompiledHeaderFile=\"" << precompiledHeaderFileMTRelease.c_str() << "\"" << std::endl;
	}
	vcproj << "				AssemblerListingLocation=\"" << assemblerListingLocationMTRelease.c_str() << "\"" << std::endl;
	vcproj << "				ObjectFile=\"" << objectFileMTRelease.c_str() << "\"" << std::endl;
	vcproj << "				ProgramDataBaseFileName=\"" << programDataBaseFileNameMTRelease.c_str() << "\"" << std::endl;
	vcproj << "				WarningLevel=\"3\"" << std::endl;
	if (target_version == 800)
		vcproj << "				SuppressStartupBanner=\"true\"" << std::endl;
	else
		vcproj << "				SuppressStartupBanner=\"TRUE\"" << std::endl;
	vcproj << "				DebugInformationFormat=\"3\"" << std::endl;
	vcproj << "				CompileAs=\"0\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCCustomBuildTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCLibrarianTool\"" << std::endl;
	vcproj << "				AdditionalOptions=\"" << additionalOptionsRelease.c_str() << "\"" << std::endl;
	vcproj << "				OutputFile=\"" << outputFileMTRelease.c_str() << "\"" << std::endl;
	if (target_version == 800)
		vcproj << "				SuppressStartupBanner=\"true\"/>" << std::endl;
	else
		vcproj << "				SuppressStartupBanner=\"TRUE\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCMIDLTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPostBuildEventTool\"" << std::endl;
	vcproj << "				Description=\"Installing library and API headers...\"" << std::endl;
	vcproj << "				CommandLine=\"" << commandLine.c_str() << "\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPreBuildEventTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPreLinkEventTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCResourceCompilerTool\"" << std::endl;
	vcproj << "				PreprocessorDefinitions=\"NDEBUG\"" << std::endl;
	vcproj << "				Culture=\"1033\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCWebServiceProxyGeneratorTool\"/>" << std::endl;
	if (target_version == 710)
	{
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCXMLDataGeneratorTool\"/>" << std::endl;
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCManagedWrapperGeneratorTool\"/>" << std::endl;
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCAuxiliaryManagedWrapperGeneratorTool\"/>" << std::endl;
	}
	vcproj << "		</Configuration>" << std::endl;
	vcproj << "		<Configuration" << std::endl;
	vcproj << "			Name=\"Static MTDLL Release|Win32\"" << std::endl;
	vcproj << "			OutputDirectory=\".\\../Release\"" << std::endl;
	vcproj << "			IntermediateDirectory=\".\\../Release/" << project.libname.c_str() << "-static-mtdll\"" << std::endl;
	vcproj << "			ConfigurationType=\"4\"" << std::endl;
	if (target_version == 800)
	{
		vcproj << "			InheritedPropertySheets=\"$(VCInstallDir)VCProjectDefaults\\UpgradeFromVC71.vsprops\"" << std::endl;
	}
	vcproj << "			UseOfMFC=\"0\"" << std::endl;
	if (target_version == 800)
		vcproj << "			ATLMinimizesCRunTimeLibraryUsage=\"false\">" << std::endl;
	else
		vcproj << "			ATLMinimizesCRunTimeLibraryUsage=\"FALSE\">" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCCLCompilerTool\"" << std::endl;
	vcproj << "				InlineFunctionExpansion=\"1\"" << std::endl;
	vcproj << "				AdditionalIncludeDirectories=\"" << includePath.c_str() << "\"" << std::endl;
	vcproj << "				PreprocessorDefinitions=\"NDEBUG," << defines.c_str() << "\"" << std::endl;
	if (target_version == 800)
	{
		vcproj << "				StringPooling=\"true\"" << std::endl;
		vcproj << "				RuntimeLibrary=\"2\"" << std::endl;
		vcproj << "				EnableFunctionLevelLinking=\"true\"" << std::endl;
		vcproj << "				RuntimeTypeInfo=\"true\"" << std::endl;
	}
	else
	{
		vcproj << "				StringPooling=\"TRUE\"" << std::endl;
		vcproj << "				RuntimeLibrary=\"2\"" << std::endl;
		vcproj << "				EnableFunctionLevelLinking=\"TRUE\"" << std::endl;
		vcproj << "				RuntimeTypeInfo=\"TRUE\"" << std::endl;
	}
	if (precompiledHeaderThrough.empty())
	{
		vcproj << "				UsePrecompiledHeader=\"2\"" << std::endl;
	}
	else
	{
		vcproj << "				UsePrecompiledHeader=\"3\"" << std::endl;
		vcproj << "				PrecompiledHeaderThrough=\"" << precompiledHeaderThrough.c_str() << "\"" << std::endl;
		vcproj << "				PrecompiledHeaderFile=\"" << precompiledHeaderFileMTDLLRelease.c_str() << "\"" << std::endl;
	}
	vcproj << "				AssemblerListingLocation=\"" << assemblerListingLocationMTDLLRelease.c_str() << "\"" << std::endl;
	vcproj << "				ObjectFile=\"" << objectFileMTDLLRelease.c_str() << "\"" << std::endl;
	vcproj << "				ProgramDataBaseFileName=\"" << programDataBaseFileNameMTDLLRelease.c_str() << "\"" << std::endl;
	vcproj << "				WarningLevel=\"3\"" << std::endl;
	if (target_version == 800)
		vcproj << "				SuppressStartupBanner=\"true\"" << std::endl;
	else
		vcproj << "				SuppressStartupBanner=\"TRUE\"" << std::endl;
	vcproj << "				DebugInformationFormat=\"3\"" << std::endl;
	vcproj << "				CompileAs=\"0\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCCustomBuildTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCLibrarianTool\"" << std::endl;
	vcproj << "				AdditionalOptions=\"" << additionalOptionsRelease.c_str() << "\"" << std::endl;
	vcproj << "				OutputFile=\"" << outputFileMTDLLRelease.c_str() << "\"" << std::endl;
	if (target_version == 800)
		vcproj << "				SuppressStartupBanner=\"true\"/>" << std::endl;
	else
		vcproj << "				SuppressStartupBanner=\"TRUE\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCMIDLTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPostBuildEventTool\"" << std::endl;
	vcproj << "				Description=\"Installing library and API headers...\"" << std::endl;
	vcproj << "				CommandLine=\"" << commandLine.c_str() << "\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPreBuildEventTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCPreLinkEventTool\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCResourceCompilerTool\"" << std::endl;
	vcproj << "				PreprocessorDefinitions=\"NDEBUG\"" << std::endl;
	vcproj << "				Culture=\"1033\"/>" << std::endl;
	vcproj << "			<Tool" << std::endl;
	vcproj << "				Name=\"VCWebServiceProxyGeneratorTool\"/>" << std::endl;
	if (target_version == 710)
	{
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCXMLDataGeneratorTool\"/>" << std::endl;
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCManagedWrapperGeneratorTool\"/>" << std::endl;
		vcproj << "			<Tool" << std::endl;
		vcproj << "				Name=\"VCAuxiliaryManagedWrapperGeneratorTool\"/>" << std::endl;
	}
	vcproj << "		</Configuration>" << std::endl;
	vcproj << "	</Configurations>" << std::endl;
	if (target_version == 710)
	{
		vcproj << "\t<References>" << std::endl;
		vcproj << "\t</References>" << std::endl;
	}
	vcproj << "	<Files>" << std::endl;

	generate_source_files(vcproj, project);

	vcproj << "	</Files>" << std::endl;
	vcproj << "	<Globals>" << std::endl;
	vcproj << "	</Globals>" << std::endl;
	vcproj << "</VisualStudioProject>" << std::endl;

	// create the install_libname.bat installation batch file:
	{
		std::string bat_file = "Sources\\install_";
		bat_file += project.libname.c_str();
		bat_file += ".bat";

		std::ofstream bat(bat_file.c_str());

		std::string instdir = workspace.output_include_dir.c_str();
		instdir += "\\ClanLib";
		install_mkdir(bat, "API\\", std::string(instdir), &project);
		install_copydir(bat, "API\\", std::string(instdir), &project);
	}

	return returnGUID;
}

void WorkspaceGenerator_MSVC7::generate_source_files(std::ofstream &vcproj, const Project &project)
{
	std::string chop_str = "Sources\\";
	unsigned int chop_length = chop_str.length();

	std::list<std::string>::iterator itCur, itNew;
	std::list<std::string> cur_path;

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
					end_group(vcproj);
					itCur++;
				}
				break;
			}

			itCur++;
			itNew++;
		}
		while (itNew != new_path.end())
		{
			if (file.find("API") == std::string::npos) begin_group(vcproj, *itNew + " files");
			else begin_group(vcproj, *itNew);
			itNew++;
		}

		cur_path = new_path;

		add_file(vcproj, file);
	}

	for (itCur = cur_path.begin(); itCur != cur_path.end(); itCur++) end_group(vcproj);
}

std::list<std::string> WorkspaceGenerator_MSVC7::extract_path(const std::string &fullname)
{
	std::list<std::string> path;
	int pos = 0;
	int old_pos = 0;

	while (true)
	{
		int pos = fullname.find("\\", old_pos);
		if (pos == std::string::npos) break;

		path.push_back(fullname.substr(old_pos, pos-old_pos));

		old_pos = pos + 1;
	}

	return path;
}

void WorkspaceGenerator_MSVC7::begin_group(std::ofstream &vcproj, const std::string &group_name)
{
	vcproj << "		<Filter" << std::endl;
	vcproj << "			Name=\"" << group_name.c_str() << "\"" << std::endl;
	if (target_version == 800)
		vcproj << "			>" << std::endl;
	else
		vcproj << "			Filter=\"\">" << std::endl;
}

void WorkspaceGenerator_MSVC7::end_group(std::ofstream &vcproj)
{
	vcproj << "		</Filter>" << std::endl;
}

void WorkspaceGenerator_MSVC7::add_file(std::ofstream &vcproj, const std::string &filename)
{
	vcproj << "			<File" << std::endl;
	vcproj << "				RelativePath=\"" << filename.c_str() << "\">" << std::endl;

	if (filename.find("precomp.cpp") != std::string::npos)
	{
		vcproj << "				<FileConfiguration" << std::endl;
		vcproj << "					Name=\"Static MT Debug|Win32\">" << std::endl;
		vcproj << "					<Tool" << std::endl;
		vcproj << "						Name=\"VCCLCompilerTool\"" << std::endl;
		vcproj << "						UsePrecompiledHeader=\"1\"/>" << std::endl;
		vcproj << "				</FileConfiguration>" << std::endl;
		vcproj << "				<FileConfiguration" << std::endl;
		vcproj << "					Name=\"Static MTDLL Debug|Win32\">" << std::endl;
		vcproj << "					<Tool" << std::endl;
		vcproj << "						Name=\"VCCLCompilerTool\"" << std::endl;
		vcproj << "						UsePrecompiledHeader=\"1\"/>" << std::endl;
		vcproj << "				</FileConfiguration>" << std::endl;
		vcproj << "				<FileConfiguration" << std::endl;
		vcproj << "					Name=\"Static MT Release|Win32\">" << std::endl;
		vcproj << "					<Tool" << std::endl;
		vcproj << "						Name=\"VCCLCompilerTool\"" << std::endl;
		vcproj << "						UsePrecompiledHeader=\"1\"/>" << std::endl;
		vcproj << "				</FileConfiguration>" << std::endl;
		vcproj << "				<FileConfiguration" << std::endl;
		vcproj << "					Name=\"Static MTDLL Release|Win32\">" << std::endl;
		vcproj << "					<Tool" << std::endl;
		vcproj << "						Name=\"VCCLCompilerTool\"" << std::endl;
		vcproj << "						UsePrecompiledHeader=\"1\"/>" << std::endl;
		vcproj << "				</FileConfiguration>" << std::endl;
	}

	vcproj << "			</File>" << std::endl;
}
