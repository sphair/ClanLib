
#pragma once

class SolutionModel;

class BuildOperation
{
public:
	BuildOperation(SolutionModel *solution);
	void build();
	void stop_build();
	bool is_building() const;
	std::string get_info_log();

	void write_log_line(const std::string &text);
	void add_file(const std::string &file);
	std::vector<std::string> get_files_with_extension(const std::string &extension);

private:
	std::map<std::string, bool> files;
	std::string info_log;
};
