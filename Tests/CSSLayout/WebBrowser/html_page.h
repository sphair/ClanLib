
#pragma once

#include "file_cache.h"

class HTMLPage
{
public:
	void load(const std::string &page_url, const std::string &refererer_url = std::string());
	clan::Image load_image(clan::GraphicContext &gc, const std::string &image_url);

	clan::HTMLUrl pageurl;
	std::string webpage;
	std::string pagecss;

	struct CSSFile
	{
		std::string css;
		std::string base_uri;
	};
	std::vector<CSSFile> css_files;

private:
	std::string load_css(std::string csstext, const std::string &base_url);
	std::string download_url(const std::string &page_url, const std::string &refererer_url);

	FileCache file_cache;
};
