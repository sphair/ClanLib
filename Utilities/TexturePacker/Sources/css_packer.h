#pragma once

class CssPacker
{
public:
	CssPacker();
	~CssPacker();

	int load(const CL_String &filename);
	void save(const CL_String &filename);

private:
	CL_CSSDocument doc;
};
