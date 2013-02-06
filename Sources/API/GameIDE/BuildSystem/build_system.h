
#pragma once

class SolutionModel;
class BuildOperation;

class BuildSystem
{
public:
	void build(SolutionModel *model);
	void stop_build();

	clan::Signal_v1<std::string> sig_log_text_added;

private:
	std::shared_ptr<BuildOperation> build_operation;
};
