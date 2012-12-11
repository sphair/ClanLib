
#include "precomp.h"
#include "html_page.h"
#include "html_tokenizer.h"
#include "html_token.h"
#include "gif_provider.h"

void HTMLPage::load(const CL_String &page_url, const CL_String &refererer_url)
{
	pagecss.clear();
	css_files.clear();
	pageurl = CL_HTMLUrl(page_url, refererer_url);
	webpage = download_url(page_url, refererer_url);

	HTMLTokenizer tokenizer;
	tokenizer.append(webpage);
	HTMLToken token;
	while (true)
	{
		tokenizer.tokenize(token);
		if (token.type == HTMLToken::type_null)
			break;
		if (token.type == HTMLToken::type_style_tag)
		{
			CL_String media;
			for (size_t i = 0; i < token.attributes.size(); i++)
			{
				if (token.attributes[i].name == "media")
					media = token.attributes[i].value;
			}

			if (media.empty() || HTMLTokenizer::compare(media, "screen") || HTMLTokenizer::compare(media, "all") || media.find("screen") != CL_String::npos)
				pagecss += load_css(token.value, page_url);
		}
		if (token.type == HTMLToken::type_tag_begin || token.type == HTMLToken::type_tag_single)
		{
			if (token.name == "link")
			{
				CL_String rel, href, media;
				for (size_t i = 0; i < token.attributes.size(); i++)
				{
					if (token.attributes[i].name == "rel")
						rel = token.attributes[i].value;
					if (token.attributes[i].name == "href")
						href = token.attributes[i].value;
					if (token.attributes[i].name == "media")
						media = token.attributes[i].value;
				}

				if (HTMLTokenizer::compare(rel, "stylesheet") && !href.empty() && (media.empty() || HTMLTokenizer::compare(media, "screen") || HTMLTokenizer::compare(media, "all") || media.find("screen") != CL_String::npos))
				{
					CL_String css = download_url(href, page_url);
					pagecss += load_css(css, CL_HTMLUrl(href, page_url).to_string());
				}
			}
		}
	}

	CL_File fhtml("htmlpage.html", CL_File::create_always, CL_File::access_write);
	fhtml.write(webpage.data(), webpage.length());
	fhtml.close();

	CL_File fcss("htmlpage.css", CL_File::create_always, CL_File::access_write);
	fcss.write(pagecss.data(), pagecss.length());
	fcss.close();
}

CL_String HTMLPage::load_css(const CL_String &csstext, const CL_String &base_url)
{
	CSSFile cssfile;
	cssfile.css = csstext;
	cssfile.base_uri = base_url;
	css_files.push_back(cssfile);

	CL_String pagecss;
	CL_CSSTokenizer css_tokenizer(csstext);
	CL_CSSToken css_token;
	while (true)
	{
		css_tokenizer.read(css_token, true);
		if (css_token.type != CL_CSSToken::type_atkeyword || css_token.value != "import")
			break;

		css_tokenizer.read(css_token, true);
		if (css_token.type != CL_CSSToken::type_string)
			break;

		CL_String import_url = css_token.value;

		css_tokenizer.read(css_token, true);
		if (css_token.type != CL_CSSToken::type_semi_colon)
			break;

		CL_String css = download_url(import_url, base_url);
		pagecss += load_css(css, CL_HTMLUrl(import_url, base_url).to_string());
	}
	pagecss += cl_format("@-clan-base-uri \"%1\";\r\n", base_url);
	pagecss += csstext;
	return pagecss;
}

CL_Image HTMLPage::load_image(CL_GraphicContext &gc, const CL_String &image_url)
{
	CL_HTMLUrl url(image_url, pageurl);
	CL_String initial_url = url.to_string();

	CL_String content_type;
	CL_DataBuffer file_data;
	if (url.scheme == "data")
	{
		if (CL_StringHelp::compare(url.encoding, "base64", true) == 0)
		{
			file_data = CL_Base64Decoder::decode(url.data);
			content_type = url.content_type;
		}
	}
	else
	{
		file_data = file_cache.get_file(initial_url, content_type);
		if (file_data.get_size() == 0)
		{
			for (size_t i = 0; i < 5; i++)
			{
				CL_WebRequest request = CL_WebRequest::get(image_url);
				request.set_header_field("User-Agent", "CSSTokenize/0.1");
				request.set_header_field("Accept", "text/plain, text/html");

				CL_WebResponse response = request.get_response();
				if (response.get_status_code() == 301 || // Moved Permanently (cache the redirect)
					response.get_status_code() == 302 || // Found (do not cache the redirect)
					response.get_status_code() == 303)   // See Other (do not cache the redirect)
				{
					url = CL_HTMLUrl(response.get_header_field("Location"), url);
				}
				else
				{
					file_data = response.get_body();
					content_type = response.get_content_type();

					file_cache.set_file(initial_url, file_data, content_type);
				}
			}
			if (file_data.get_size() == 0)
				throw CL_Exception("Redirection limit reached");
		}
	}

	CL_IODevice_Memory device(file_data);
	if (content_type == "image/png")
	{
		CL_PixelBuffer pb = CL_PNGProvider::load(device);
		return CL_Image(gc, pb, pb.get_size());
	}
	else if (content_type == "image/jpeg")
	{
		CL_PixelBuffer pb = CL_JPEGProvider::load(device);
		return CL_Image(gc, pb, pb.get_size());
	}
	else if (content_type == "image/gif")
	{
		CL_PixelBuffer pb = GIFProvider::load(device);
		return CL_Image(gc, pb, pb.get_size());
	}
	else
	{
		CL_Console::write_line("Unknown image type");
		return CL_Image();
	}
}

CL_String HTMLPage::download_url(const CL_String &page_url, const CL_String &refererer_url)
{
	CL_HTMLUrl url(page_url, refererer_url);
	CL_String initial_url = url.to_string();
	CL_String content_type;
	CL_DataBuffer file_data = file_cache.get_file(initial_url, content_type);
	if (file_data.get_size() > 0)
	{
		return CL_String(file_data.get_data(), file_data.get_size());
	}
	else
	{
		for (size_t i = 0; i < 5; i++)
		{
			CL_WebRequest request = CL_WebRequest::get(url.to_string());
			request.set_header_field("User-Agent", "CSSTokenize/0.1");
			request.set_header_field("Accept", "text/plain, text/html");
			if (!refererer_url.empty())
				request.set_header_field("Referer", refererer_url);

			CL_WebResponse response = request.get_response();
			if (response.get_status_code() == 301 || // Moved Permanently (cache the redirect)
				response.get_status_code() == 302 || // Found (do not cache the redirect)
				response.get_status_code() == 303)   // See Other (do not cache the redirect)
			{
				url = CL_HTMLUrl(response.get_header_field("Location"), url);
			}
			else
			{
				file_cache.set_file(initial_url, response.get_body(), response.get_content_type());
				return response.get_body_string();
			}
		}
		throw CL_Exception("Redirection limit reached");
	}
}
