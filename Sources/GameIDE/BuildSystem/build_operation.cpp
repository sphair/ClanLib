
#include "precomp.h"
#include "build_operation.h"
#include "GameIDE/SolutionModel/solution_model.h"

using namespace clan;

BuildOperation::BuildOperation(SolutionModel *solution)
{
}

void BuildOperation::build()
{
}

void BuildOperation::stop_build()
{
}

bool BuildOperation::is_building() const
{
	return false;
}

std::string BuildOperation::get_info_log()
{
	return info_log;
}

void BuildOperation::write_log_line(const std::string &text)
{
	info_log += text;
	info_log += "\n";
}

void BuildOperation::add_file(const std::string &file)
{
	files[file] = true;
}

std::vector<std::string> BuildOperation::get_files_with_extension(const std::string &extension)
{
	std::vector<std::string> found_files;
	for (auto it = files.begin(); it != files.end(); ++it)
	{
		if (StringHelp::compare(PathHelp::get_extension(it->first), extension, true) == 0)
			found_files.push_back(it->first);
	}
	return found_files;
}
