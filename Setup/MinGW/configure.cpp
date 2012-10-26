#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include <dir.h>

#include "../Generic/workspace.h"
#include "../Generic/project.h"

void make_makefile_conf();
void make_project_makefile(const Project& proj);
void make_main_makefile(const Workspace& work);

int main()
{
	Workspace work;

	std::list<std::string> libs_list_shared;
	std::list<std::string> libs_list_release;
	std::list<std::string> libs_list_debug;
	std::list<std::string> defines_list;

	defines_list.push_back("USE_OPENGL");
	defines_list.push_back("USE_NETWORK");
	defines_list.push_back("USE_CLANSOUND");
	defines_list.push_back("DIRECTINPUT_VERSION=0x700");
	defines_list.push_back("USE_PNG");
	defines_list.push_back("USE_I386_ASSEMBLER");
	
	Project clanCore(
		"Core",
		"clanCore",
		"core.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();

	Project clanSignals(
		"Signals",
		"clanSignals",
		"signals.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanApp(
		"Application",
		"clanApp",
		"application.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.push_back("ws2_32.lib");

	Project clanNetwork(
		"Network",
		"clanNetwork",
		"network.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();

	libs_list_shared.push_back("hermes.lib");
	libs_list_shared.push_back("winmm.lib");
	libs_list_shared.push_back("ddraw.lib");
	libs_list_shared.push_back("dinput.lib");
	libs_list_shared.push_back("dxguid.lib");
	libs_list_shared.push_back("jpeg.lib");
	libs_list_shared.push_back("libpng.lib");

	Project clanDisplay(
		"Display",
		"clanDisplay",
		"display.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();
/*
	Project clanDX7(
		"DX7",
		"clanDX7",
		"dx7.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);
	
	libs_list_shared.clear();
	
	Project clanDX8(
		"DX8",
		"clanDX8",
		"dx8.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);
	
	libs_list_shared.clear();
*/
	libs_list_shared.push_back("dsound.lib");
	libs_list_shared.push_back("winmm.lib");

	Project clanSound(
		"Sound",
		"clanSound",
		"sound.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();

	libs_list_shared.push_back("OpenGL32.lib");
	libs_list_shared.push_back("GLU32.lib");
		
	Project clanGL(
		"GL",
		"clanGL",
		"gl.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();

	Project clanGUI(
		"GUI",
		"clanGUI",
		"gui.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanGUIStyleBoring(
		"GUIStyleBoring",
		"clanGUIStyleBoring",
		"guistyleboring.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanGUIStyleSilver(
		"GUIStyleSilver",
		"clanGUIStyleSilver",
		"guistylesilver.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.push_back("js32.lib");
		
	Project clanJavaScript(
		"JavaScript",
		"clanJavaScript",
		"javascript.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);
/*	
	libs_list_shared.clear();

	libs_list_debug.push_back("luad.lib");
	libs_list_debug.push_back("lualibd.lib");
	libs_list_debug.push_back("tolualibd.lib");
	libs_list_release.push_back("lua.lib");
	libs_list_release.push_back("lualib.lib");
	libs_list_release.push_back("tolualib.lib");
	
	Project clanLua(
		"Lua",
		"clanLua",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);
*/
	libs_list_shared.clear();
	libs_list_debug.clear();
	libs_list_release.clear();

	libs_list_shared.push_back("vorbis_static.lib");
	libs_list_shared.push_back("ogg_static.lib");

	Project clanVorbis(
		"Vorbis",
		"clanVorbis",
		"vorbis.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();

	libs_list_shared.push_back("mikmod.lib");

	Project clanMikMod(
		"MikMod",
		"clanMikMod",
		"mikmod.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();

/*
	libs_list_shared.push_back("jpeg.lib");
	
	Project clanJPEG(
		"JPEG",
		"clanJPEG",
		"jpeg.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);
	
	libs_list_shared.clear();
	
	libs_list_shared.push_back("libpng.lib");
	
	Project clanPNG(
		"PNG",
		"clanPNG",
		"png.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);
	
	libs_list_shared.clear();
	
	libs_list_shared.push_back("freetype206MT.lib");
	
	Project clanTTF(
		"TTF",
		"clanTTF",
		"ttf.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);
	*/

	libs_list_shared.clear();

	Project clanVoice(
		"Voice",
		"clanVoice",
		"voice.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	// Setup project dependencies:

	// Add projects to workspace:
	work.projects.push_back(clanSignals);
	work.projects.push_back(clanCore);
	work.projects.push_back(clanApp);
	work.projects.push_back(clanNetwork);
	work.projects.push_back(clanDisplay);
	work.projects.push_back(clanSound);
	work.projects.push_back(clanGL);
//	work.projects.push_back(clanDX7);
//	work.projects.push_back(clanDX8);
	work.projects.push_back(clanGUI);
	work.projects.push_back(clanGUIStyleBoring);
	work.projects.push_back(clanGUIStyleSilver);
//	work.projects.push_back(clanJavaScript);
//	work.projects.push_back(clanLua);
//	work.projects.push_back(clanVorbis);
//	work.projects.push_back(clanMikMod);
//	work.projects.push_back(clanMPEG);
//	work.projects.push_back(clanJPEG);
//	work.projects.push_back(clanPNG);
//	work.projects.push_back(clanTTF);
//	work.projects.push_back(clanVoice);


	std::cout << "Creating directory MinGW/" << std::endl;
	mkdir("MinGW");

	make_makefile_conf();
	for(std::list<Project>::const_iterator it=work.projects.begin();it!=work.projects.end();++it)
		make_project_makefile(*it);
	make_main_makefile(work);

	return 0;
}

void make_makefile_conf()
{
	std::ifstream in("Setup/MinGW/Makefile.conf.in");
	std::ofstream out("MinGW/Makefile.conf");

	std::cout << "Creating MinGW/Makefile.conf" << std::endl;
	
	char ch;
	while(in.get(ch))
		out.put(ch);
}

void make_project_makefile(const Project& proj)
{
	std::string filename = "MinGW/" + proj.libname + ".mak";
	std::ofstream file(filename.c_str());

	std::cout << "Creating " << filename << std::endl;

	std::string targetname = "MinGW/lib" + proj.libname + ".a";

	file << "# MinGW Makefile for " << proj.name << std::endl;
	file << "TARGET=" << targetname << std::endl;
	file << std::endl;

	file << "CXXFLAGS+=-mthreads -fvtable-thunks" << std::endl;
	file << std::endl;

	file << "CXXFLAGS+=";
	for(std::list<std::string>::const_iterator it=proj.defines_list.begin();it!=proj.defines_list.end();++it)
		file << "-D" << *it << " ";
	file << std::endl;
	file << std::endl;

	file << "SOURCES=";
	
	for(std::list<std::string>::const_iterator it=proj.files.begin();it!=proj.files.end();)
	{
		file << *it++;
		if(it!=proj.files.end())
			file << " \\";
		file << std::endl;
	}

	file << std::endl;
	file << "include MinGW/Makefile.conf" << std::endl;
	file << std::endl;
}

void make_main_makefile(const Workspace& work)
{
	std::string filename = "Makefile.mingw";
	std::ofstream file(filename.c_str());

	std::cout << "Creating " << filename << std::endl;

	file << "# MinGW main ClanLib Makefile" << std::endl;
	file << std::endl;

	file << "all: " << std::endl;
	for(std::list<Project>::const_iterator it=work.projects.begin();it!=work.projects.end();++it)
	{
		std::string filename = "MinGW/" + it->libname + ".mak";
		file << "\tmake -f" << filename << std::endl;
	}
	file << std::endl;
	file << "clean: " << std::endl;
	for(std::list<Project>::const_iterator it=work.projects.begin();it!=work.projects.end();++it)
	{
		std::string filename = "MinGW/" + it->libname + ".mak";
		file << "\tmake -f" << filename << " clean" << std::endl;
	}
	file << std::endl;
	file << ".PHONY: clean" <<std::endl;
	file << std::endl;
}

