
#pragma once

class CL_IODevice;
class XMLSettings;

class XMLSettingsDocument
{
public:
	XMLSettingsDocument();

	void load(const CL_String &filename);
	void load(CL_IODevice device);
	void save(const CL_String &filename);
	void save(CL_IODevice device);

	XMLSettings get_root();

private:
	CL_DomDocument document;
};
