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

#ifndef header_workspace_generator_msvc8
#define header_workspace_generator_msvc8

#include <vector>
#include "../Generic/workspace.h"
#include <fstream>

class MSVC8_Project;
class MSVC8_Configuration;
class MSVC8_Tool;
class MSVC8_FileItem;
class MSVC8_File;
class MSVC8_Filter;
class MSVC8_FileConfiguration;
class MSVC8_VCCLCompilerTool;
class MSVC8_VCResourceCompilerTool;
class MSVC8_VCLibrarianTool;
class MSVC8_VCLinkerTool;
class MSVC8_VCPostBuildEventTool;

/////////////////////////////////////////////////////////////////////////////
// Workspace generator class:

class WorkspaceGenerator_MSVC8
{
public:
	WorkspaceGenerator_MSVC8();
	void set_platforms(bool include_win32, bool include_x64, bool include_sse2, bool include_intrinsics, bool enable_debug_optimise, bool enable_whole_program_optimize);
	void enable_configurations(bool include_mtdll, bool include_dll);
	void set_target_version(int version);
	void write(const Workspace &workspace);

	enum RuntimeType
	{
		runtime_static_debug,
		runtime_static_release,
		runtime_dll_debug,
		runtime_dll_release
	};

	struct ConfigurationType
	{
		bool included;
		char *name;
		RuntimeType runtime_type;
		bool unicode;
		bool dll;
	};

	static ConfigurationType types[];

private:
	struct SharedConfig
	{
		SharedConfig()
		: config(0), tool_compiler(0), tool_librarian(0), tool_linker(0), tool_post_build(0)
		{
		}
		MSVC8_Configuration *config;
		MSVC8_VCCLCompilerTool *tool_compiler;
		MSVC8_VCLibrarianTool *tool_librarian;
		MSVC8_VCLinkerTool *tool_linker;
		MSVC8_VCPostBuildEventTool *tool_post_build;
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
	MSVC8_Configuration *create_debug_mt_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool is_enable_sse2, bool is_debug_optimize);
	MSVC8_Configuration *create_debug_mtdll_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool is_enable_sse2, bool is_debug_optimize);
	MSVC8_Configuration *create_debug_dll_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool is_enable_sse2, bool is_debug_optimize);
	MSVC8_Configuration *create_release_mt_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool is_enable_sse2, bool is_whole_program_optimize);
	MSVC8_Configuration *create_release_mtdll_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool is_enable_sse2, bool is_whole_program_optimize);
	MSVC8_Configuration *create_release_dll_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header, bool is_enable_sse2, bool is_whole_program_optimize);
	SharedConfig create_shared_config(const std::string &platform, const std::string &project_name, const ConfigurationType &config, bool has_precomp, const std::string &precomp_header);
	std::string make_output_filename(
		const ConfigurationType &config,
		bool make_dll_name,
		const std::string &platform,
		const std::string &project_name);

	std::string make_target_name(
		const ConfigurationType &config,
		const std::string &platform,
		const std::string &project_name);

	std::string make_upper(const std::string &s);

	int target_version;
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
	MSVC8_PropertySheet(int target_version);
	~MSVC8_PropertySheet();

	std::string name;
	std::vector<MSVC8_Tool *> tools;
	std::string input_include_dir_vs100;
	std::string input_lib_dir_vs100;
	int target_version;

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

	std::vector<std::string> platforms;
	std::vector<MSVC8_Configuration *> configurations;
	std::vector<MSVC8_FileItem *> files;

	void write(OutputWriter &output, int indent) const;
	void write_filters(OutputWriter &output, int indent) const;
};

class MSVC8_Setting
{
public:
	MSVC8_Setting();
	MSVC8_Setting(const std::string &s);

	bool is_set() const;
	void set(const std::string &s);
	const std::string &get() const;
	void set_default();

	void write(OutputWriter &output, int indent, const std::string &name) const;

private:
	bool not_set;
	std::string value;
};

class MSVC8_Configuration
{
public:
	MSVC8_Configuration();
	~MSVC8_Configuration();

	std::string name;
	std::string name_without_platform;
	std::string name_without_config;
	MSVC8_Setting output_directory;
	MSVC8_Setting intermediate_directory;
	std::string configuration_type;
	std::string inherited_property_sheets;
	std::vector<std::string> inherited_property_sheets_vs100;
	std::string target_name_vs100;

	std::string use_of_mfc;
	std::string atl_minimizes_c_runtime_library_usage;
	MSVC8_Setting character_set;

	std::vector<MSVC8_Tool *> tools;
	MSVC8_VCCLCompilerTool *tool_compiler_vs100;

	void write(OutputWriter &output, int indent) const;
};

class MSVC8_Tool
{
public:
	virtual ~MSVC8_Tool();

	std::string name;

	void write(OutputWriter &output, int indent) const;

	virtual void write_settings(OutputWriter &output, int indent) const = 0;
};

class MSVC8_VCCLCompilerTool : public MSVC8_Tool
{
public:
	MSVC8_VCCLCompilerTool();

	MSVC8_Setting optimization;
	MSVC8_Setting additional_include_directories;
	MSVC8_Setting preprocessor_definitions;
	MSVC8_Setting basic_runtime_checks;
	MSVC8_Setting runtime_library;
	MSVC8_Setting runtime_type_info;
	MSVC8_Setting use_precompiled_header;
	MSVC8_Setting precompiled_header_through;
	MSVC8_Setting precompiled_header_file;
	MSVC8_Setting assembler_listing_location;
	MSVC8_Setting object_file;
	MSVC8_Setting program_database_filename;
	MSVC8_Setting warning_level;
	MSVC8_Setting suppress_startup_banner;
	MSVC8_Setting debug_information_format;
	MSVC8_Setting compile_as;

	void write_settings(OutputWriter &output, int indent) const;
};

class MSVC8_VCResourceCompilerTool : public MSVC8_Tool
{
public:
	MSVC8_VCResourceCompilerTool();

	MSVC8_Setting preprocessor_definitions;
	MSVC8_Setting culture;

	void write_settings(OutputWriter &output, int indent) const;
};

class MSVC8_VCLibrarianTool : public MSVC8_Tool
{
public:
	MSVC8_VCLibrarianTool();

	MSVC8_Setting additional_library_directories;
	MSVC8_Setting additional_options;
	MSVC8_Setting output_file;
	MSVC8_Setting suppress_startup_banner;

	void write_settings(OutputWriter &output, int indent) const;
};

class MSVC8_VCLinkerTool : public MSVC8_Tool
{
public:
	MSVC8_VCLinkerTool();

	MSVC8_Setting additional_library_directories;
	MSVC8_Setting output_file;

	void write_settings(OutputWriter &output, int indent) const;
};

class MSVC8_VCPostBuildEventTool : public MSVC8_Tool
{
public:
	MSVC8_VCPostBuildEventTool();

	MSVC8_Setting description;
	MSVC8_Setting command_line;

	void write_settings(OutputWriter &output, int indent) const;
};

class MSVC8_FileItem
{
public:
	virtual ~MSVC8_FileItem();

	virtual void write(OutputWriter &output, int indent) const = 0;
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

	void write(OutputWriter &output, int indent) const;
	void write_vs100(OutputWriter &output, int indent, const std::vector<MSVC8_Configuration *> &configurations) const;
	void write_filter_name_vs100(OutputWriter &output, int indent, const std::string &parent) const;
	void write_filter_files_vs100(OutputWriter &output, int indent, const std::string &parent) const;
};

class MSVC8_File : public MSVC8_FileItem
{
public:
	MSVC8_File();
	~MSVC8_File();

	std::string relative_path;
	std::vector<MSVC8_FileConfiguration *> file_configurations;

	void write(OutputWriter &output, int indent) const;
	void write_vs100(OutputWriter &output, int indent, const std::vector<MSVC8_Configuration *> &configurations) const;
	void write_filter_name_vs100(OutputWriter &output, int indent, const std::string &parent) const;
	void write_filter_files_vs100(OutputWriter &output, int indent, const std::string &parent) const;
};

class MSVC8_FileConfiguration
{
public:
	MSVC8_FileConfiguration();
	~MSVC8_FileConfiguration();

	std::string name;
	std::vector<MSVC8_Tool *> tools;

	void write(OutputWriter &output, int indent) const;
	void write_vs100(OutputWriter &output, int indent, const std::vector<MSVC8_Configuration *> &configurations) const;
	void write_filter_name_vs100(OutputWriter &output, int indent, const std::string &parent) const;
	void write_filter_files_vs100(OutputWriter &output, int indent, const std::string &parent) const;
};

#endif
