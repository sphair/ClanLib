
#include "precomp.h"
#include "html_page.h"
#include "html_url.h"
#include "html_tokenizer.h"
#include "html_token.h"
#include "gif_provider.h"

void HTMLPage::load(const CL_String &page_url, const CL_String &refererer_url)
{
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

				if (rel == "stylesheet" && !href.empty() && (media.empty() || media == "screen"))
				{
					CL_String css = download_url(href, page_url);
					pagecss += load_css(css, HTMLUrl(href, page_url).to_string());
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
		pagecss += load_css(css, HTMLUrl(import_url, base_url).to_string());
	}
	pagecss += cl_format("@-clan-base-uri \"%1\";\r\n", base_url);
	pagecss += csstext;
	return pagecss;
}

CL_Image HTMLPage::load_image(CL_GraphicContext &gc, const CL_String &image_url)
{
	HTMLUrl url(image_url, pageurl);
	CL_Console::write_line("Downloading image: %1", url.to_string());
	CL_String8 request;
	request = cl_format("GET %1 HTTP/1.1\r\n", url.path+url.query);
	request += cl_format("Host: %1\r\nConnection: close\r\nReferer: %2\r\nAccept: image/png, image/jpeg\r\nUser-Agent: CSSTokenize/0.1\r\n\r\n", url.host, pageurl.to_string());

	CL_TCPConnection connection(CL_SocketName(url.host, url.port));
	connection.set_nodelay(true);
	connection.send(request.data(), request.length(), true);

	CL_String response;
	while (connection.get_read_event().wait(15000))
	{
		char buffer[16*1024];
		int received = connection.read(buffer, 16*1024, false);
		if (received == 0)
			break;
		response.append(buffer, received);
	}
	connection.disconnect_graceful();

	CL_String response_header = response.substr(0, response.find("\r\n\r\n"));
	CL_String content = response.substr(response_header.length() + 4);

	if (response_header.find("Transfer-Encoding: chunked") != CL_String::npos)
	{
		CL_String::size_type start = 0;
		while (true)
		{
			CL_String::size_type end = content.find("\r\n", start);
			if (end == CL_String::npos)
				end = content.length();

			CL_String str_length = content.substr(start, end-start);
			int length = CL_StringHelp::text_to_int(str_length, 16);
			content = content.substr(0, start) + content.substr(end+2);
			start += length;


			end = content.find("\r\n", start);
			if (end == CL_String::npos)
				end = content.length();
			content = content.substr(0, start) + content.substr(end+2);

			if (length == 0)
				break;
		}
	}

	CL_DataBuffer buffer(content.data(), content.length());
	CL_IODevice_Memory device(buffer);
	if (response_header.find("image/png") != CL_String::npos)
	{
		CL_PixelBuffer pb = CL_PNGProvider::load(device);
		return CL_Image(gc, pb, pb.get_size());
	}
	else if (response_header.find("image/jpeg") != CL_String::npos)
	{
		CL_PixelBuffer pb = CL_JPEGProvider::load(device);
		return CL_Image(gc, pb, pb.get_size());
	}
	else if (response_header.find("image/gif") != CL_String::npos)
	{
		CL_PixelBuffer pb = GIFProvider::load(device);
		return CL_Image(gc, pb, pb.get_size());
	}
	else
	{
		CL_Console::write_line("Unknown image type: %1", CL_String8(buffer.get_data(), buffer.get_size()));
		return CL_Image();
	}
}

CL_String HTMLPage::download_url(const CL_String &page_url, const CL_String &refererer_url)
{
	HTMLUrl url(page_url, refererer_url);
	CL_Console::write_line("Downloading URL: %1", url.to_string());
	CL_String8 request;
	request = cl_format("GET %1 HTTP/1.1\r\n", url.path+url.query);
	if (refererer_url.empty())
		request += cl_format("Host: %1\r\nConnection: close\r\nAccept: text/plain, text/html\r\nUser-Agent: CSSTokenize/0.1\r\n\r\n", url.host);
	else
		request += cl_format("Host: %1\r\nConnection: close\r\nReferer: %2\r\nAccept: text/plain, text/html\r\nUser-Agent: CSSTokenize/0.1\r\n\r\n", url.host, refererer_url);
	//MessageBoxW(0, CL_StringHelp::utf8_to_ucs2(cl_format("GET %1 HTTP/1.1\r\n", url.path+url.query)).c_str(), L"Download URL", MB_OK);;

	CL_TCPConnection connection(CL_SocketName(url.host, url.port));
	connection.set_nodelay(true);
	connection.send(request.data(), request.length(), true);

	CL_String response;
	while (connection.get_read_event().wait(15000))
	{
		char buffer[16*1024];
		int received = connection.read(buffer, 16*1024, false);
		if (received == 0)
			break;
		response.append(buffer, received);
	}
	connection.disconnect_graceful();

	CL_String response_header = response.substr(0, response.find("\r\n\r\n"));
	CL_String content = response.substr(response_header.length() + 4);

	if (response_header.find("Transfer-Encoding: chunked") != CL_String::npos)
	{
		CL_String::size_type start = 0;
		while (true)
		{
			CL_String::size_type end = content.find("\r\n", start);
			if (end == CL_String::npos)
				end = content.length();

			CL_String str_length = content.substr(start, end-start);
			int length = CL_StringHelp::text_to_int(str_length, 16);
			content = content.substr(0, start) + content.substr(end+2);
			start += length;


			end = content.find("\r\n", start);
			if (end == CL_String::npos)
				end = content.length();
			content = content.substr(0, start) + content.substr(end+2);

			if (length == 0)
				break;
		}
	}

	return content;
}
