
#pragma once

class ProjectReference
{
public:
	ProjectReference(std::string name, std::string filename) : name(name), filename(filename) { }

	std::string name;
	std::string filename;
};
