
#include "precomp.h"
#include "html_page.h"
#include "gif_provider.h"

using namespace clan;

void HTMLPage::load(const std::string &page_url, const std::string &refererer_url)
{
	pagecss.clear();
	css_files.clear();
	pageurl = HTMLUrl(page_url, refererer_url);
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
			std::string media;
			for (size_t i = 0; i < token.attributes.size(); i++)
			{
				if (token.attributes[i].name == "media")
					media = token.attributes[i].value;
			}

			if (media.empty() || HTMLTokenizer::compare(media, "screen") || HTMLTokenizer::compare(media, "all") || media.find("screen") != std::string::npos)
				pagecss += load_css(token.value, page_url);
		}
		if (token.type == HTMLToken::type_tag_begin || token.type == HTMLToken::type_tag_single)
		{
			if (token.name == "link")
			{
				std::string rel, href, media;
				for (size_t i = 0; i < token.attributes.size(); i++)
				{
					if (token.attributes[i].name == "rel")
						rel = token.attributes[i].value;
					if (token.attributes[i].name == "href")
						href = token.attributes[i].value;
					if (token.attributes[i].name == "media")
						media = token.attributes[i].value;
				}

				if (HTMLTokenizer::compare(rel, "stylesheet") && !href.empty() && (media.empty() || HTMLTokenizer::compare(media, "screen") || HTMLTokenizer::compare(media, "all") || media.find("screen") != std::string::npos))
				{
					std::string css = download_url(href, page_url);
					pagecss += load_css(css, HTMLUrl(href, page_url).to_string());
				}
			}
		}
	}

	File fhtml("htmlpage.html", File::create_always, File::access_write);
	fhtml.write(webpage.data(), webpage.length());
	fhtml.close();

	File fcss("htmlpage.css", File::create_always, File::access_write);
	fcss.write(pagecss.data(), pagecss.length());
	fcss.close();
}

std::string HTMLPage::load_css(const std::string &csstext, const std::string &base_url)
{
	CSSFile cssfile;
	cssfile.css = csstext;
	cssfile.base_uri = base_url;
	css_files.push_back(cssfile);

	std::string pagecss;
	CSSTokenizer css_tokenizer(csstext);
	CSSToken css_token;
	while (true)
	{
		css_tokenizer.read(css_token, true);
		if (css_token.type != CSSToken::type_atkeyword || css_token.value != "import")
			break;

		css_tokenizer.read(css_token, true);
		if (css_token.type != CSSToken::type_string)
			break;

		std::string import_url = css_token.value;

		css_tokenizer.read(css_token, true);
		if (css_token.type != CSSToken::type_semi_colon)
			break;

		std::string css = download_url(import_url, base_url);
		pagecss += load_css(css, HTMLUrl(import_url, base_url).to_string());
	}
	pagecss += string_format("@-clan-base-uri \"%1\";\r\n", base_url);
	pagecss += csstext;
	return pagecss;
}

Image HTMLPage::load_image(GraphicContext &gc, const std::string &image_url)
{
	HTMLUrl url(image_url, pageurl);
	std::string initial_url = url.to_string();

	std::string content_type;
	DataBuffer file_data;
	if (url.scheme == "data")
	{
		if (StringHelp::compare(url.encoding, "base64", true) == 0)
		{
			file_data = Base64Decoder::decode(url.data);
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
				WebRequest request = WebRequest::get(image_url);
				request.set_header_field("User-Agent", "CSSTokenize/0.1");
				request.set_header_field("Accept", "text/plain, text/html");

				WebResponse response = request.get_response();
				if (response.get_status_code() == 301 || // Moved Permanently (cache the redirect)
					response.get_status_code() == 302 || // Found (do not cache the redirect)
					response.get_status_code() == 303)   // See Other (do not cache the redirect)
				{
					url = HTMLUrl(response.get_header_field("Location"), url);
				}
				else
				{
					file_data = response.get_body();
					content_type = response.get_content_type();

					file_cache.set_file(initial_url, file_data, content_type);
				}
			}
			if (file_data.get_size() == 0)
				throw Exception("Redirection limit reached");
		}
	}

	IODevice_Memory device(file_data);
	if (content_type == "image/png")
	{
		PixelBuffer pb = PNGProvider::load(device);
		return Image(gc, pb, pb.get_size());
	}
	else if (content_type == "image/jpeg")
	{
		PixelBuffer pb = JPEGProvider::load(device);
		return Image(gc, pb, pb.get_size());
	}
	else if (content_type == "image/gif")
	{
		PixelBuffer pb = GIFProvider::load(device);
		return Image(gc, pb, pb.get_size());
	}
	else
	{
		Console::write_line("Unknown image type");
		return Image();
	}
}

std::string HTMLPage::download_url(const std::string &page_url, const std::string &refererer_url)
{
	HTMLUrl url(page_url, refererer_url);
	std::string initial_url = url.to_string();
	std::string content_type;
	DataBuffer file_data = file_cache.get_file(initial_url, content_type);
	if (file_data.get_size() > 0)
	{
		return std::string(file_data.get_data(), file_data.get_size());
	}
	else
	{
		for (size_t i = 0; i < 5; i++)
		{
			WebRequest request = WebRequest::get(url.to_string());
			request.set_header_field("User-Agent", "CSSTokenize/0.1");
			request.set_header_field("Accept", "text/plain, text/html");
			if (!refererer_url.empty())
				request.set_header_field("Referer", refererer_url);

			WebResponse response = request.get_response();
			if (response.get_status_code() == 301 || // Moved Permanently (cache the redirect)
				response.get_status_code() == 302 || // Found (do not cache the redirect)
				response.get_status_code() == 303)   // See Other (do not cache the redirect)
			{
				url = HTMLUrl(response.get_header_field("Location"), url);
			}
			else
			{
				file_cache.set_file(initial_url, response.get_body(), response.get_content_type());
				return response.get_body_string();
			}
		}
		throw Exception("Redirection limit reached");
	}
}
