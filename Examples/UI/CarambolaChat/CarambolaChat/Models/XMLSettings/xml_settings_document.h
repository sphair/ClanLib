
#pragma once

class XMLSettings;

class XMLSettingsAppModel
{
public:
	XMLSettingsAppModel();

	void load(const std::string &filename);
	void load(clan::IODevice device);
	void save(const std::string &filename);
	void save(clan::IODevice device);

	XMLSettings get_root();

private:
	clan::DomDocument document;
};

