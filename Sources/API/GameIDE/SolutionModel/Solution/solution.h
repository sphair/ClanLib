
#pragma once

#include "project_reference.h"

class Solution
{
public:
	void create(std::string folder, std::string name);
	void load(std::string filename);
	void save();
	void close();

	std::string name;
	std::string filename;
	std::vector<ProjectReference> projects;
};
