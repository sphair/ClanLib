
#pragma once

class BuildOperation;

class BuildTool
{
public:
	virtual ~BuildTool() { }
	virtual std::string get_id() = 0;
	virtual int get_priority() = 0;
	virtual void run(BuildOperation &build_operation, std::map<std::string, std::string> &properties) = 0;
};
