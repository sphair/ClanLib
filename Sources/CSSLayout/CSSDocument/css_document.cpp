/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/CSSDocument/css_document.h"
#include "API/CSSLayout/CSSDocument/css_select_result.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/iodevice_memory.h"
#include "css_document_impl.h"
#include "css_document_sheet.h"
#include "css_ruleset_match.h"

namespace clan
{

CSSDocument::CSSDocument()
: impl(std::make_shared<CSSDocument_Impl>())
{
}

CSSDocument::~CSSDocument()
{
}

void CSSDocument::add_default_html_sheet()
{
	std::string css_text = get_default_html_sheet();
	DataBuffer buffer(css_text.data(), css_text.length());
	IODevice_Memory iodevice(buffer);
	add_sheet(author_sheet_origin, iodevice, "file:");
}

void CSSDocument::add_sheet(CSSSheetOrigin origin, const std::string &filename, const FileSystem &fs)
{
	// Load the css document:
	IODevice file = fs.open_file(filename);
	DataBuffer file_data(file.get_size());
	file.read(file_data.get_data(), file_data.get_size());
	std::string css_text(file_data.get_data(), file_data.get_size());

	// Find the base URI for this css document:
	std::string base_uri = PathHelp::get_fullpath(filename);

	// Find import directives and load those first:
	std::vector<std::string> import_urls = CSSTokenizer(css_text).read_import_urls();
	for (size_t i = 0; i < import_urls.size(); i++)
	{
		add_sheet(origin, PathHelp::combine(base_uri, import_urls[i]), fs);
	}

	// Add the css sheet:
	CSSTokenizer tokenizer(css_text);
	impl->sheets.push_back(std::shared_ptr<CSSDocumentSheet>(new CSSDocumentSheet(origin, tokenizer, base_uri)));
}

void CSSDocument::add_sheet(CSSSheetOrigin origin, IODevice &iodevice, const std::string &base_uri)
{
	CSSTokenizer tokenizer(iodevice);
	impl->sheets.push_back(std::shared_ptr<CSSDocumentSheet>(new CSSDocumentSheet(origin, tokenizer, base_uri)));
}

CSSSelectResult CSSDocument::select(const DomElement &node, const std::string &pseudo_element)
{
	DomSelectNode select_node(node);
	return select(&select_node, pseudo_element);
}

CSSSelectResult CSSDocument::select(CSSSelectNode *node, const std::string &pseudo_element)
{
	// CSS2.1: 6.4.1 Cascading order

	// Bug: This code does not currently favor user important declarations over author important declarations.
	//      Since I've never heard about a person setting his own user style sheet I don't really care much tbh.
	//      If you're the one guy in the world actually doing this, feel free to refactor the code!

	std::vector<CSSRulesetMatch> rulesets = impl->select_rulesets(node, pseudo_element);
	std::vector<CSSPropertyValue *> properties;
	for (size_t i = rulesets.size(); i > 0; i--)
	{
		for (size_t j = rulesets[i-1].ruleset->important_values.size(); j > 0; j--)
		{
			properties.push_back(rulesets[i-1].ruleset->important_values[j-1].get());
		}
	}
	for (size_t i = rulesets.size(); i > 0; i--)
	{
		for (size_t j = rulesets[i-1].ruleset->values.size(); j > 0; j--)
		{
			properties.push_back(rulesets[i-1].ruleset->values[j-1].get());
		}
	}

	return CSSSelectResult(properties);
}

std::string CSSDocument::get_default_html_sheet()
{
	return
		"html, address,\n"
		"blockquote,\n"
		"body, dd, div,\n"
		"dl, dt, fieldset, form,\n"
		"frame, frameset,\n"
		"h1, h2, h3, h4,\n"
		"h5, h6, noframes,\n"
		"ol, p, ul, center,\n"
		"dir, hr, menu, pre   { display: block }\n"
		"li              { display: list-item }\n"
		"head            { display: none }\n"
		"table           { display: table }\n"
		"tr              { display: table-row }\n"
		"thead           { display: table-header-group }\n"
		"tbody           { display: table-row-group }\n"
		"tfoot           { display: table-footer-group }\n"
		"col             { display: table-column }\n"
		"colgroup        { display: table-column-group }\n"
		"td, th          { display: table-cell }\n"
		"caption         { display: table-caption }\n"
		"th              { font-weight: bolder; text-align: center }\n"
		"caption         { text-align: center }\n"
		"body            { margin: 8px }\n"
		"h1              { font-size: 2em; margin: .67em 0 }\n"
		"h2              { font-size: 1.5em; margin: .75em 0 }\n"
		"h3              { font-size: 1.17em; margin: .83em 0 }\n"
		"h4, p,\n"
		"blockquote, ul,\n"
		"fieldset, form,\n"
		"ol, dl, dir,\n"
		"menu            { margin: 1.12em 0 }\n"
		"h5              { font-size: .83em; margin: 1.5em 0 }\n"
		"h6              { font-size: .75em; margin: 1.67em 0 }\n"
		"h1, h2, h3, h4,\n"
		"h5, h6, b,\n"
		"strong          { font-weight: bolder }\n"
		"blockquote      { margin-left: 40px; margin-right: 40px }\n"
		"i, cite, em,\n"
		"var, address    { font-style: italic }\n"
		"pre, tt, code,\n"
		"kbd, samp       { font-family: monospace }\n"
		"pre             { white-space: pre }\n"
		"button, textarea,\n"
		"input, select   { display: inline-block }\n"
		"big             { font-size: 1.17em }\n"
		"small, sub, sup { font-size: .83em }\n"
		"sub             { vertical-align: sub }\n"
		"sup             { vertical-align: super }\n"
		"table           { border-spacing: 2px; }\n"
		"thead, tbody,\n"
		"tfoot           { vertical-align: middle }\n"
		"td, th, tr      { vertical-align: inherit }\n"
		"s, strike, del  { text-decoration: line-through }\n"
		"hr              { border: 1px inset }\n"
		"ol, ul, dir,\n"
		"menu, dd        { margin-left: 40px }\n"
		"ol              { list-style-type: decimal }\n"
		"ol ul, ul ol,\n"
		"ul ul, ol ol    { margin-top: 0; margin-bottom: 0 }\n"
		"u, ins          { text-decoration: underline }\n"
		"br:before       { content: \"\\A\"; white-space: pre-line }\n"
		"center          { text-align: center }\n"
		":link, :visited { text-decoration: underline }\n"
		":focus          { outline: thin dotted invert }\n"
		"\n"
		"/* Begin bidirectionality settings (do not change) */\n"
		"BDO[DIR=\"ltr\"]  { direction: ltr; unicode-bidi: bidi-override }\n"
		"BDO[DIR=\"rtl\"]  { direction: rtl; unicode-bidi: bidi-override }\n"
		"\n"
		"*[DIR=\"ltr\"]    { direction: ltr; unicode-bidi: embed }\n"
		"*[DIR=\"rtl\"]    { direction: rtl; unicode-bidi: embed }\n"
		"\n"
		"@media print {\n"
		"  h1            { page-break-before: always }\n"
		"  h1, h2, h3,\n"
		"  h4, h5, h6    { page-break-after: avoid }\n"
		"  ul, ol, dl    { page-break-before: avoid }\n"
		"}\n";
}

}
