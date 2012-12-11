
#pragma once

#include "file_cache.h"

class HTMLPage
{
public:
	void load(const CL_String &page_url, const CL_String &refererer_url = CL_String());
	CL_Image load_image(CL_GraphicContext &gc, const CL_String &image_url);

	CL_HTMLUrl pageurl;
	CL_String webpage;
	CL_String pagecss;

	struct CSSFile
	{
		CL_String css;
		CL_String base_uri;
	};
	std::vector<CSSFile> css_files;

private:
	CL_String load_css(const CL_String &csstext, const CL_String &base_url);
	CL_String download_url(const CL_String &page_url, const CL_String &refererer_url);

	FileCache file_cache;
};
