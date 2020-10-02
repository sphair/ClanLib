/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#ifndef header_workspace_generator_msvc8
#define header_workspace_generator_msvc8

#include <vector>
#include "workspace.h"
#include <fstream>

class MSVC8_Project;
class MSVC8_Configuration;
class MSVC8_FileItem;
class MSVC8_File;
class MSVC8_Filter;
class MSVC8_FileConfiguration;

/////////////////////////////////////////////////////////////////////////////
// Workspace generator class:

class WorkspaceGenerator_MSVC8
{
public:
	WorkspaceGenerator_MSVC8();
	void set_platforms(bool include_win32, bool include_x64, bool include_sse2, bool include_intrinsics, bool enable_debug_optimise, bool enable_whole_program_optimize);
	void set_android(bool enable);
	void enable_configurations(bool include_mtdll);
	void set_target_version(int version);
	void write(const Workspace &workspace);

	enum RuntimeType
	{
		runtime_static_debug,
		runtime_static_release,
		runtime_dll_debug,
		runtime_dll_release
	};

	// Warning - "WorkspaceGenerator_MSVC8::types" uses this directly
	struct ConfigurationType
	{
		bool included;
		const char *name;
		RuntimeType runtime_type;
	};

	static ConfigurationType types[];

private:
	struct SharedConfig
	{
		SharedConfig()
		: config(0)
		{
		}
		MSVC8_Configuration *config;
	};

	void write_solution(const Workspace &workspace);
	void write_property_sheet(const Workspace &workspace);
	void write_project(const Workspace &workspace, const Project &project);
	void write_install_batch_file(const Workspace &workspace, const Project &project);
	void install_mkdir(
		std::ofstream &bat,
		const std::string &dest_dir);
	void install_mkdir(
		std::ofstream &bat,
		const std::string &src_dir,
		const std::string &dest_dir,
		const Project *project);
	void install_copydir(
		std::ofstream &bat,
		const std::string &src_dir,
		const std::string &dest_dir,
		const Project *project);
	void generate_source_files(MSVC8_Project &vcproj, const Project &project);
	std::list<std::string> extract_path(const std::string &fullname);
	std::string get_project_guid(const std::string &project);
	MSVC8_Configuration *create_debug_mt_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool write_sse2_props);
	MSVC8_Configuration *create_debug_mtdll_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool write_sse2_props);
	MSVC8_Configuration *create_debug_dll_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool write_sse2_props);
	MSVC8_Configuration *create_release_mt_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool write_sse2_props);
	MSVC8_Configuration *create_release_mtdll_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool write_sse2_props);
	MSVC8_Configuration *create_release_dll_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool write_sse2_props);

	MSVC8_Configuration *create_android_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header);

	SharedConfig create_shared_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header);

	std::string make_target_name(
		const ConfigurationType &config,
		const std::string &platform,
		const std::string &project_name);

	std::string make_upper(const std::string &s);

	int target_version;
	bool target_android = false;
	bool include_platform_win32;
	bool include_platform_x64;
	bool is_enable_sse2;
	bool is_enable_intrinsics;
	bool is_debug_optimize;
	bool is_whole_program_optimize;
};

/////////////////////////////////////////////////////////////////////////////
// OutputWriter class:

class OutputWriter
{
public:
	OutputWriter(const std::string &filename);

	void write_line(int indent, const std::string &line);

	std::ofstream output;
};

/////////////////////////////////////////////////////////////////////////////
// Visual Studio VC Project classes:

class MSVC8_PropertySheet
{
public:
	MSVC8_PropertySheet(int target_version, bool target_android);
	~MSVC8_PropertySheet();

	std::string name;
	std::string input_include_dir_vs100;
	std::string input_lib_dir_vs100;
	int target_version;
	bool target_android = false;

	void write(OutputWriter &output, int indent);
};

class MSVC8_Project
{
public:
	MSVC8_Project();
	~MSVC8_Project();

	std::string name;
	std::string project_guid;

	int target_version;
	bool target_android;

	std::vector<std::string> platforms;
	std::vector<MSVC8_Configuration *> configurations;
	std::vector<MSVC8_FileItem *> files;

	void write(OutputWriter &output, int indent) const;
	void write_filters(OutputWriter &output, int indent) const;

};

class MSVC8_Configuration
{
public:
	MSVC8_Configuration();
	~MSVC8_Configuration();

	std::string name;
	std::string name_without_platform;
	std::string name_without_config;
	std::vector<std::string> inherited_property_sheets_vs100;
	std::string target_name_vs100;

	std::string use_precompiled_header;
	std::string precompiled_header_through;

	bool is_this_android = false;
	std::string android_debug_libraries;
};

class MSVC8_FileItem
{
public:
	virtual ~MSVC8_FileItem();

	virtual void write_vs100(OutputWriter &output, int indent, const std::vector<MSVC8_Configuration *> &configurations) const = 0;
	virtual void write_filter_name_vs100(OutputWriter &output, int indent, const std::string &parent) const = 0;
	virtual void write_filter_files_vs100(OutputWriter &output, int indent, const std::string &parent) const = 0;
};

class MSVC8_Filter : public MSVC8_FileItem
{
public:
	MSVC8_Filter();
	~MSVC8_Filter();

	std::string name;
	std::vector<MSVC8_FileItem *> files;

	void write_vs100(OutputWriter &output, int indent, const std::vector<MSVC8_Configuration *> &configurations) const;
	void write_filter_name_vs100(OutputWriter &output, int indent, const std::string &parent) const;
	void write_filter_files_vs100(OutputWriter &output, int indent, const std::string &parent) const;
};

class MSVC8_File : public MSVC8_FileItem
{
public:
	MSVC8_File(bool is_exclude_from_build, const std::string& name) : exclude_from_build(is_exclude_from_build), relative_path(name) {}

	bool exclude_from_build;
	std::string relative_path;

	void write_vs100(OutputWriter &output, int indent, const std::vector<MSVC8_Configuration *> &configurations) const;
	void write_filter_name_vs100(OutputWriter &output, int indent, const std::string &parent) const;
	void write_filter_files_vs100(OutputWriter &output, int indent, const std::string &parent) const;
};


#endif
