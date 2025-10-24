#include "precomp.h"
#include "css_packer.h"

CssPacker::CssPacker()
{
}

CssPacker::~CssPacker()
{
}

int CssPacker::load(const CL_String &filename)
{
	doc.load(filename);

	std::vector<CL_CSSImport> &imports = doc.get_imports();

	return imports.size();
}

void CssPacker::save(const CL_String &filename)
{
	CL_File output_file(filename, CL_File::create_always, CL_File::access_write);
	doc.save(output_file);
}
