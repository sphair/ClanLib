
#include <dirent.h>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> scan_dir(const std::string &dir, bool prefix);
void writestr(const std::string &s, FILE *file);
void create_umbrella_project(FILE *file, std::vector<std::string> headers, const std::vector<std::string> &projectdeps);
void create_project_targets(
	FILE *file,
	const std::string &project,
	std::vector<std::string> files,
	std::vector<std::string> headers,
	const std::vector<std::string> &projectdeps,
	const std::string &extraparams);
void write_project_resources(const std::string &project);

int main(int argc, char **argv)
{
	FILE *file = fopen("Makefile", "wb");
	writestr("all: ", file);
	writestr("clanSignals ", file);
	writestr("clanCore ", file);
	writestr("clanApp ", file);
	writestr("clanNetwork ", file);
	writestr("clanDisplay ", file);
	writestr("clanGL ", file);
	writestr("clanSDL ", file);
	writestr("clanSound ", file);
	writestr("clanMikMod ", file);
	writestr("clanVorbis ", file);
	writestr("clanGUI ", file);
	writestr("clanGUIStyleSilver ", file);
	writestr("ClanLib\n", file);
	writestr("\t@echo Build complete!\n\n", file);

	std::vector<std::string> signalsDeps;
	std::string signalsLinkExtra;
	create_project_targets(file, "clanSignals", scan_dir("Sources/Signals", true), scan_dir("Sources/API", true), signalsDeps, signalsLinkExtra);
	
	std::vector<std::string> coreDeps;
	std::string coreLinkExtra = "-lz -lpthread -framework Carbon";
	coreDeps.push_back("clanSignals");
	create_project_targets(file, "clanCore", scan_dir("Sources/Core", true), scan_dir("Sources/API", true), coreDeps, coreLinkExtra);

	std::vector<std::string> appDeps;
	std::string appLinkExtra;
	appDeps.push_back("clanCore");
	appDeps.push_back("clanSignals");
	create_project_targets(file, "clanApp", scan_dir("Sources/Application", true), scan_dir("Sources/API", true), appDeps, appLinkExtra);

	std::vector<std::string> networkDeps;
	std::string networkLinkExtra;
	networkDeps.push_back("clanSignals");
	networkDeps.push_back("clanCore");
	create_project_targets(file, "clanNetwork", scan_dir("Sources/Network", true), scan_dir("Sources/API", true), networkDeps, networkLinkExtra);

	std::vector<std::string> displayDeps;
	std::string displayLinkExtra = "-lpng -ljpeg";
	displayDeps.push_back("clanSignals");
	displayDeps.push_back("clanCore");
	create_project_targets(file, "clanDisplay", scan_dir("Sources/Display", true), scan_dir("Sources/API", true), displayDeps, displayLinkExtra);

	std::vector<std::string> glDeps;
	std::string glLinkExtra = "-framework OpenGL -framework AGL -framework Carbon";
	glDeps.push_back("clanSignals");
	glDeps.push_back("clanCore");
	glDeps.push_back("clanDisplay");
	create_project_targets(file, "clanGL", scan_dir("Sources/GL", true), scan_dir("Sources/API", true), glDeps, glLinkExtra);

	std::vector<std::string> sdlDeps;
	std::string sdlLinkExtra = "-lsdl -lobjc";
	sdlDeps.push_back("clanSignals");
	sdlDeps.push_back("clanCore");
	sdlDeps.push_back("clanDisplay");
	create_project_targets(file, "clanSDL", scan_dir("Sources/SDL", true), scan_dir("Sources/API", true), sdlDeps, sdlLinkExtra);

	std::vector<std::string> soundDeps;
	std::string soundLinkExtra = "-framework CoreAudio";
	soundDeps.push_back("clanSignals");
	soundDeps.push_back("clanCore");
	create_project_targets(file, "clanSound", scan_dir("Sources/Sound", true), scan_dir("Sources/API", true), soundDeps, soundLinkExtra);

	std::vector<std::string> mikmodDeps;
	std::string mikmodLinkExtra = "-lmikmod";
	mikmodDeps.push_back("clanSignals");
	mikmodDeps.push_back("clanCore");
	mikmodDeps.push_back("clanSound");
	create_project_targets(file, "clanMikMod", scan_dir("Sources/MikMod", true), scan_dir("Sources/API", true), mikmodDeps, mikmodLinkExtra);

	std::vector<std::string> vorbisDeps;
	std::string vorbisLinkExtra = "-logg -lvorbis";
	vorbisDeps.push_back("clanSignals");
	vorbisDeps.push_back("clanCore");
	vorbisDeps.push_back("clanSound");
	create_project_targets(file, "clanVorbis", scan_dir("Sources/Vorbis", true), scan_dir("Sources/API", true), vorbisDeps, vorbisLinkExtra);

	std::vector<std::string> GUIDeps;
	std::string GUILinkExtra;
	GUIDeps.push_back("clanSignals");
	GUIDeps.push_back("clanCore");
	GUIDeps.push_back("clanDisplay");
	create_project_targets(file, "clanGUI", scan_dir("Sources/GUI", true), scan_dir("Sources/API", true), GUIDeps, GUILinkExtra);

	std::vector<std::string> GUIStyleSilverDeps;
	std::string GUIStyleSilverLinkExtra;
	GUIStyleSilverDeps.push_back("clanSignals");
	GUIStyleSilverDeps.push_back("clanCore");
	GUIStyleSilverDeps.push_back("clanDisplay");
	GUIStyleSilverDeps.push_back("clanGUI");
	create_project_targets(file, "clanGUIStyleSilver", scan_dir("Sources/GUIStyleSilver", true), scan_dir("Sources/API", true), GUIStyleSilverDeps, GUIStyleSilverLinkExtra);

	std::vector<std::string> umbrellaDeps;
	umbrellaDeps.push_back("clanSignals");
	umbrellaDeps.push_back("clanCore");
	umbrellaDeps.push_back("clanDisplay");
	umbrellaDeps.push_back("clanGUI");
	umbrellaDeps.push_back("clanGUIStyleSilver");
	umbrellaDeps.push_back("clanSound");
	umbrellaDeps.push_back("clanApp");
	umbrellaDeps.push_back("clanNetwork");
	umbrellaDeps.push_back("clanGL");
	umbrellaDeps.push_back("clanSDL");
	umbrellaDeps.push_back("clanVorbis");
	umbrellaDeps.push_back("clanMikMod");
	create_umbrella_project(file, scan_dir("Sources/API", true), umbrellaDeps);

	fclose(file);
	return 0;
}

std::vector<std::string> scan_dir(const std::string &inputdir, bool prefix)
{
	std::vector<std::string> filenames;
	std::vector<std::string> dirs;

	std::string dirprefix;
	if (prefix) dirprefix = inputdir+"/";

	DIR *dir = opendir(inputdir.c_str());
	if (dir)
	{
		while (true)
		{
			dirent *ent = readdir(dir);
			if (ent == 0)
				break;

			if (ent->d_type == DT_DIR)
			{
				if (
					strcmp(".", ent->d_name) != 0 &&
					strcmp("..", ent->d_name) != 0)
				{
					dirs.push_back(ent->d_name);
				}
			}
			else
			{
				std::string f = dirprefix+ent->d_name;
				if (f.find("win32") == std::string::npos)
					filenames.push_back(f);
			}
		}
		closedir(dir);
	}

	std::vector<std::string>::size_type index;
	for (index = 0; index < dirs.size(); index++)
	{
		std::vector<std::string> subfiles = scan_dir(inputdir+std::string("/")+dirs[index], false);
		std::vector<std::string>::size_type index2;
		for (index2 = 0; index2 < subfiles.size(); index2++)
		{
			filenames.push_back(dirprefix+dirs[index]+std::string("/")+subfiles[index2]);
		}
	}
	
	return filenames;
}

void apply_ignore_list(std::vector<std::string> &files)
{
	for (int index = 0; index < (int) files.size(); index++)
	{
		const std::string &file = files[index];
		bool remove = false;

		if (file.find("Sources/GL/GLX") == 0)
			remove = true;
		else if (file.find("Sources/Application/Unix") == 0)
			remove = true;
		else if (file.find("Win32") != std::string::npos)
			remove = true;
		else if (file.find("WGL") != std::string::npos)
			remove = true;
		else if (file.find("Sources/GL/GLX") == 0)
			remove = true;
		else if (file.find("Sources/Sound/SoundProviders/Unix") == 0)
			remove = true;
		else if (file.find("Sources/Sound/Unix") == 0)
			remove = true;

		if (remove)
		{
			files.erase(files.begin()+index);
			index--;
		}
	}
}

void create_umbrella_project(FILE *file, std::vector<std::string> headers, const std::vector<std::string> &projectdeps)
{
	std::vector<std::string>::size_type index;

	write_project_resources("ClanLib");

	writestr("ClanLib : Setup/OSX/clanlib.cpp\n", file);
	
	std::string clversion = "0.8.0";
	std::string bundle = "Frameworks/ClanLib.framework";
	std::string version = bundle+"/Versions/"+clversion;
	writestr("\t@install -d "+version+"\n", file);
	writestr("\t@install -d "+version+"/Headers\n", file);
	writestr("\t@install -d "+version+"/Resources\n", file);
	writestr("\t@ln -s -f "+clversion+" "+bundle+"/Versions/Current\n", file);
	writestr("\t@ln -s -f Versions/Current/Headers "+bundle+"/Headers\n", file);
	writestr("\t@ln -s -f Versions/Current/Resources "+bundle+"/Resources\n", file);
	writestr("\t@install ClanLib-Info.plist "+version+"/Resources/Info.plist\n", file);
	for (int i=0; i<headers.size(); i++)
	{
		if (
			(headers[i].length() > 2 && headers[i].substr(headers[i].length()-2, 2) == ".h") ||
			(headers[i].length() > 4 && headers[i].substr(headers[i].length()-4, 4) == ".inl"))
		{
			std::string dir = headers[i].substr(11, headers[i].find_last_of('/')-10);
			writestr("\t@install -d "+version+"/Headers"+dir+"\n", file);
			writestr("\t@install "+headers[i]+" "+version+"/Headers"+dir+"\n", file);
		}
	}
	writestr(
		"\tg++ -dynamiclib -compatibility_version 1 -current_version 1"
		" -headerpad_max_install_names"
		" -install_name ClanLib.framework/Versions/"+clversion+"/ClanLib"
		" -o ClanLib -FFrameworks ", file);
/*	for (index = 0; index < projectdeps.size(); index++)
	{
		writestr("-framework "+projectdeps[index]+" ", file);
	}
*/
	writestr("Setup/OSX/clanlib.cpp\n", file);
	writestr("\t@install -d "+version+"\n", file);
	writestr("\tinstall ClanLib "+version+"\n", file);
	writestr("\t@ln -s -f Versions/Current/ClanLib "+bundle+"/ClanLib\n", file);
	writestr("\n", file);
}

void create_project_targets(
	FILE *file,
	const std::string &project,
	std::vector<std::string> files,
	std::vector<std::string> headers,
	const std::vector<std::string> &projectdeps,
	const std::string &extraparams)
{
	apply_ignore_list(files);

	std::vector<std::string>::size_type index;

	writestr(project+" : ", file);
	for (index = 0; index < projectdeps.size(); index++)
	{
		writestr(projectdeps[index]+" ", file);
	}
	for (index = 0; index < files.size(); index++)
	{
		if (files[index].length() > 4 && files[index].substr(files[index].length()-4, 4) == ".cpp")
		{
			std::string filename = files[index].substr(0, files[index].find(".cpp"));
			writestr(filename+".o ", file);
		}
	}
	writestr("\n", file);

	write_project_resources(project);
	
	std::string version = "0.8.0";
//	std::string bundle = "$(HOME)/Library/Frameworks/ClanLib.framework";
	std::string subbundle = "Frameworks/"+project+".framework";
	std::string subversion = subbundle+"/Versions/"+version;
	writestr("\t@install -d "+subversion+"\n", file);
	writestr("\t@install -d "+subversion+"/Headers\n", file);
	writestr("\t@install -d "+subversion+"/Resources\n", file);
	writestr("\t@ln -s -f "+version+" "+subbundle+"/Versions/Current\n", file);
	writestr("\t@ln -s -f Versions/Current/Headers "+subbundle+"/Headers\n", file);
	writestr("\t@ln -s -f Versions/Current/Resources "+subbundle+"/Resources\n", file);
	writestr("\t@install "+project+"-Info.plist "+subbundle+"/Resources/Info.plist\n", file);
//	writestr("\t@ln -s -f Versions/Current/"+project+" "+subbundle+"/"+project+"\n", file);
	for (int i=0; i<headers.size(); i++)
	{
		if (
			(headers[i].length() > 2 && headers[i].substr(headers[i].length()-2, 2) == ".h") ||
			(headers[i].length() > 4 && headers[i].substr(headers[i].length()-4, 4) == ".inl"))
		{
			std::string dir = headers[i].substr(11, headers[i].find_last_of('/')-10);
			writestr("\t@install -d "+subbundle+"/Versions/"+version+"/Headers"+dir+"\n", file);
			writestr("\t@install "+headers[i]+" "+subbundle+"/Versions/"+version+"/Headers"+dir+"\n", file);
		}
	}
	writestr(
		"\tg++ -dynamiclib -compatibility_version 1 -current_version 1"
		" -headerpad_max_install_names"
		" -install_name "+project+".framework/Versions/"+version+"/"+project+
		" -o "+project+" -FFrameworks ", file);
	for (index = 0; index < projectdeps.size(); index++)
	{
		writestr("-framework "+projectdeps[index]+" ", file);
	}

	writestr(extraparams+" ", file);

	for (index = 0; index < files.size(); index++)
	{
		if (files[index].length() > 4 && files[index].substr(files[index].length()-4, 4) == ".cpp")
		{
			std::string filename = files[index].substr(0, files[index].find(".cpp"));
			writestr(filename+".o ", file);
		}
	}
	writestr("\n", file);
	writestr("\t@install -d "+subbundle+"/Versions/"+version+"\n", file);
	writestr("\tinstall "+project+" "+subbundle+"/Versions/"+version+"\n", file);
	writestr("\t@ln -s -f Versions/Current/"+project+" "+subbundle+"/"+project+"\n", file);
	writestr("\n", file);

	for (index = 0; index < files.size(); index++)
	{
		if (files[index].length() > 4 && files[index].substr(files[index].length()-4, 4) == ".cpp")
		{
			std::string filename = files[index].substr(0, files[index].find(".cpp"));
			writestr(filename+".o : "+filename+".cpp\n", file);
			writestr("\tg++ -c -I Sources -DUSE_BIG_ENDIAN -I /usr/X11R6/include/ -o "+filename+".o "+filename+".cpp\n\n", file);
		}
	}
}

void write_project_resources(const std::string &project)
{
	std::string bundle_development_region = "English";
	std::string bundle_executable = project;
	std::string bundle_icon_file;
	std::string bundle_identifier = "org.clanlib."+project;
	std::string bundle_info_dictionary_version = "6.0";
	std::string bundle_package_type = "FMWK";
	std::string bundle_signature = "????";
	std::string bundle_version = "0.8.0";
	bool resources_file_mapped = true;

	FILE *file = fopen(std::string(project+"-Info.plist").c_str(), "wb");
	writestr("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", file);
	writestr("<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n", file);
	writestr("<plist version=\"1.0\">\n", file);
	writestr("<dict>\n", file);
	writestr("\t<key>CFBundleDevelopmentRegion</key>\n", file);
	writestr("\t<string>"+bundle_development_region+"</string>\n", file);
	writestr("\t<key>CFBundleExecutable</key>\n", file);
	writestr("\t<string>"+bundle_executable+"</string>\n", file);
	writestr("\t<key>CFBundleIconFile</key>\n", file);
	writestr("\t<string>"+bundle_icon_file+"</string>\n", file);
	writestr("\t<key>CFBundleIdentifier</key>\n", file);
	writestr("\t<string>"+bundle_identifier+"</string>\n", file);
	writestr("\t<key>CFBundleInfoDictionaryVersion</key>\n", file);
	writestr("\t<string>"+bundle_info_dictionary_version+"</string>\n", file);
	writestr("\t<key>CFBundlePackageType</key>\n", file);
	writestr("\t<string>"+bundle_package_type+"</string>\n", file);
	writestr("\t<key>CFBundleSignature</key>\n", file);
	writestr("\t<string>"+bundle_signature+"</string>\n", file);
	writestr("\t<key>CFBundleVersion</key>\n", file);
	writestr("\t<string>"+bundle_version+"</string>\n", file);
	writestr("\t<key>CSResourcesFileMapped</key>\n", file);
	writestr("\t"+std::string(resources_file_mapped ? "<true/>" : "<false/>")+"\n", file);
	writestr("</dict>\n", file);
	writestr("</plist>\n", file);
	fclose(file);
}

void writestr(const std::string &s, FILE *file)
{
	fwrite(s.c_str(), s.length(), 1, file);
}

/*
bahamas:~/Library/Frameworks mbn$ find ClanLib.framework 
ClanLib.framework
ClanLib.framework/Frameworks
ClanLib.framework/Frameworks/ClanApplication.framework
ClanLib.framework/Frameworks/ClanCore.framework
ClanLib.framework/Frameworks/ClanCore.framework/ClanCore
ClanLib.framework/Frameworks/ClanCore.framework/Headers
ClanLib.framework/Frameworks/ClanCore.framework/Resources
ClanLib.framework/Frameworks/ClanCore.framework/Versions
ClanLib.framework/Frameworks/ClanCore.framework/Versions/0.8.0
ClanLib.framework/Frameworks/ClanCore.framework/Versions/0.8.0/ClanCore
ClanLib.framework/Frameworks/ClanCore.framework/Versions/0.8.0/Headers
ClanLib.framework/Frameworks/ClanCore.framework/Versions/0.8.0/Resources
ClanLib.framework/Frameworks/ClanCore.framework/Versions/Current
ClanLib.framework/Frameworks/ClanDisplay.framework
ClanLib.framework/Frameworks/ClanGL.framework
ClanLib.framework/Frameworks/ClanGUI.framework
ClanLib.framework/Frameworks/ClanGUIStyleSilver.framework
ClanLib.framework/Frameworks/ClanMikMod.framework
ClanLib.framework/Frameworks/ClanNetwork.framework
ClanLib.framework/Frameworks/ClanSDL.framework
ClanLib.framework/Frameworks/ClanSignals.framework
ClanLib.framework/Frameworks/ClanSound.framework
ClanLib.framework/Frameworks/ClanVorbis.framework
ClanLib.framework/Headers
ClanLib.framework/Resources
ClanLib.framework/Umbrella
ClanLib.framework/Versions
ClanLib.framework/Versions/0.8.0
ClanLib.framework/Versions/0.8.0/Headers
ClanLib.framework/Versions/0.8.0/Resources
ClanLib.framework/Versions/0.8.0/Umbrella
ClanLib.framework/Versions/Current
*/
