
#include "precomp.h"
#include "html_tokenizer.h"
#include "html_token.h"

using namespace clan;

HTMLTokenizer::HTMLTokenizer()
: pos(0), before_first_tag(true)
{
}

void HTMLTokenizer::append(const std::string &new_data)
{
	if (pos == data.size())
	{
		data.clear();
		pos = 0;
	}
	data += new_data;
}

void HTMLTokenizer::tokenize(HTMLToken &out_token)
{
	out_token.type = HTMLToken::type_null;
	out_token.name.clear();
	out_token.value.clear();
	out_token.attributes.clear();
	
	size_t new_pos = pos;
	if (before_first_tag)
		new_pos = read_dtd(pos, out_token);
	if (out_token.type == HTMLToken::type_null)
		new_pos = read_comment(pos, out_token);
	if (out_token.type == HTMLToken::type_null)
		new_pos = read_tag(pos, out_token);
	if (out_token.type == HTMLToken::type_null)
		new_pos = read_text(pos, out_token);
	if (out_token.type == HTMLToken::type_tag_begin && compare(out_token.name, "script"))
		new_pos = read_script_value(pos, new_pos, out_token);
	if (out_token.type == HTMLToken::type_tag_begin && compare(out_token.name, "style"))
		new_pos = read_style_value(pos, new_pos, out_token);
	pos = new_pos;
}

bool HTMLTokenizer::is_tag_begin(size_t p)
{
	return p < data.size() && data[p] == '<';
}

bool HTMLTokenizer::is_tag_end(size_t p)
{
	return p < data.size() && data[p] == '>';
}

bool HTMLTokenizer::is_tag_single_end(size_t p)
{
	return p+1 < data.size() && data[p] == '/' && data[p+1] == '>';
}

bool HTMLTokenizer::is_end_tag_begin(size_t p)
{
	return p+1 < data.size() && data[p] == '<' && data[p+1] == '/';
}

bool HTMLTokenizer::is_tag_name(size_t p)
{
	return p < data.size() && ((data[p] >= 'a' && data[p] <= 'z') || (data[p] >= 'A' && data[p] <= 'Z'));
}

bool HTMLTokenizer::is_tag_name_continued(size_t p)
{
	return p < data.size() && ((data[p] >= 'a' && data[p] <= 'z') || (data[p] >= 'A' && data[p] <= 'Z') || (data[p] >= '0' && data[p] <= '9') || data[p] == ':' || data[p] == '-');
}

bool HTMLTokenizer::is_comment_begin(size_t p)
{
	return p+3 < data.size() && data[p] == '<' && data[p+1] == '!' && data[p+2] == '-' && data[p+3] == '-';
}

bool HTMLTokenizer::is_comment_end(size_t p)
{
	return p+2 < data.size() && data[p] == '-' && data[p+1] == '-' && data[p+2] == '>';
}

bool HTMLTokenizer::is_dtd(size_t p)
{
	return false;
}

bool HTMLTokenizer::is_whitespace(size_t p)
{
	return p < data.size() && (data[p] == ' ' || data[p] == '\r' || data[p] == '\n' || data[p] == '\t');
}

bool HTMLTokenizer::is_operator_equal(size_t p)
{
	return p < data.size() && data[p] == '=';
}

bool HTMLTokenizer::is_string(size_t p)
{
	return is_double_quote(p) || is_single_quote(p) || is_unquoted_string(p);
}

bool HTMLTokenizer::is_single_quote(size_t p)
{
	return p < data.size() && data[p] == '\'';
}

bool HTMLTokenizer::is_double_quote(size_t p)
{
	return p < data.size() && data[p] == '"';
}

bool HTMLTokenizer::is_unquoted_string(size_t p)
{
	return p < data.size() && ((data[p] >= 'a' && data[p] <= 'z') || (data[p] >= 'A' && data[p] <= 'Z') || (data[p] >= '0' && data[p] <= '9'));
}

bool HTMLTokenizer::is_string_escape(size_t p)
{
	return p+1 < data.size() && data[p] == '\\' && (data[p+1] == '\'' || data[p+1] == '"');
}

size_t HTMLTokenizer::read_name(size_t p, std::string &out_string)
{
	if (is_tag_name(p))
	{
		size_t p2 = p+1;
		for (p2 = p+1; p2 < data.size(); p2++)
		{
			if (!is_tag_name_continued(p2))
				break;
		}
		out_string = StringHelp::text_to_lower(data.substr(p, p2-p));
		return p2;
	}
	else
	{
		return p;
	}
}

size_t HTMLTokenizer::read_whitespace(size_t p)
{
	while (is_whitespace(p)) p++;
	return p;
}

size_t HTMLTokenizer::read_string(size_t p, std::string &out_string)
{
	out_string.clear();
	if (is_single_quote(p))
	{
		size_t p2 = p+1;
		for (p2 = p+1; p2 < data.size(); p2++)
		{
			if (is_single_quote(p2))
				break;
			if (is_string_escape(p2))
				p2++;
		}
		if (p2 != data.size())
		{
			out_string = data.substr(p+1, p2-p-1);
			return p2+1;
		}
	}
	else if (is_double_quote(p))
	{
		size_t p2 = p+1;
		for (p2 = p+1; p2 < data.size(); p2++)
		{
			if (is_double_quote(p2))
				break;
			if (is_string_escape(p2))
				p2++;
		}
		if (p2 != data.size())
		{
			out_string = data.substr(p+1, p2-p-1);
			return p2+1;
		}
	}
	else if (is_unquoted_string(p))
	{
		size_t p2 = p+1;
		for (p2 = p+1; p2 < data.size(); p2++)
		{
			if (!is_unquoted_string(p2))
				break;
		}
		if (p2 != data.size())
		{
			out_string = data.substr(p, p2-p);
			return p2;
		}
	}
	return p;
}

size_t HTMLTokenizer::read_tag(size_t p, HTMLToken &out_token)
{
	if (is_tag_begin(p) && is_tag_name(p+1))
	{
		size_t p2 = read_name(p+1, out_token.name);
		size_t p3 = read_whitespace(p2);
		while (is_tag_name(p3))
		{
			HTMLAttribute attribute;
			size_t p4 = read_name(p3, attribute.name);
			p4 = read_whitespace(p4);
			if (!is_operator_equal(p4))
				break;
			p4++;
			p4 = read_whitespace(p4);
			if (!is_string(p4))
				break;
			p4 = read_string(p4, attribute.value);
			p3 = read_whitespace(p4);
			unescape(attribute.value);
			out_token.attributes.push_back(attribute);
		}
		p3 = read_whitespace(p3);
		if (is_tag_end(p3))
		{
			out_token.type = HTMLToken::type_tag_begin;
			return p3+1;
		}
		else if (is_tag_single_end(p3))
		{
			out_token.type = HTMLToken::type_tag_single;
			return p3+2;
		}
	}
	else if (is_end_tag_begin(p) && is_tag_name(p+2))
	{
		size_t p2 = read_name(p+2, out_token.name);
		size_t p3 = read_whitespace(p2);
		if (is_tag_end(p3))
		{
			out_token.type = HTMLToken::type_tag_end;
			return p3+1;
		}
	}
	
	if (is_tag_begin(p))
	{
		for (size_t i = p+1; i < data.size(); i++)
		{
			if (is_tag_end(i))
			{
				out_token.type = HTMLToken::type_invalid;
				return i+1;
			}
		}
	}

	out_token.type = HTMLToken::type_null;
	return p;
}

size_t HTMLTokenizer::read_script_value(size_t p, size_t pvalue, HTMLToken &token)
{
	for (size_t i = pvalue; i < data.size(); i++)
	{
		if (is_end_tag_begin(i))
		{
			std::string name;
			size_t p2 = read_name(i+2, name);
			size_t p3 = read_whitespace(p2);
			if (compare(name, "script") && is_tag_end(p3))
			{
				token.type = HTMLToken::type_script_tag;
				token.value = data.substr(pvalue, i-pvalue);
				return p3+1;
			}
		}
	}

	token.type = HTMLToken::type_null;
	return p;
}

size_t HTMLTokenizer::read_style_value(size_t p, size_t pvalue, HTMLToken &token)
{
	for (size_t i = pvalue; i < data.size(); i++)
	{
		if (is_end_tag_begin(i))
		{
			std::string name;
			size_t p2 = read_name(i+2, name);
			size_t p3 = read_whitespace(p2);
			if (compare(name, "style") && is_tag_end(p3))
			{
				token.type = HTMLToken::type_style_tag;
				token.value = data.substr(pvalue, i-pvalue);
				return p3+1;
			}
		}
	}

	token.type = HTMLToken::type_null;
	return p;
}

bool HTMLTokenizer::compare(const std::string &a, const std::string &b)
{
	return StringHelp::compare(a, b, true) == 0;
}

size_t HTMLTokenizer::read_comment(size_t p, HTMLToken &out_token)
{
	if (is_comment_begin(p))
	{
		for (size_t i = p+4; i < data.size(); i++)
		{
			if (is_comment_end(i))
			{
				out_token.type = HTMLToken::type_comment;
				out_token.value = data.substr(p+4, i-p-4);
				return i+3;
			}
		}
	}

	out_token.type = HTMLToken::type_null;
	return p;
}

size_t HTMLTokenizer::read_text(size_t p, HTMLToken &out_token)
{
	size_t p2;
	for (p2 = p; p2 < data.size(); p2++)
	{
		if (is_comment_begin(p2) || is_tag_begin(p2))
			break;
	}
	
	if (p2 != p)
	{
		out_token.type = HTMLToken::type_text;
		out_token.value = data.substr(p, p2-p);
		unescape(out_token.value);
		return p2;
	}
	else
	{
		out_token.type = HTMLToken::type_null;
		return p;
	}
}

size_t HTMLTokenizer::read_dtd(size_t p, HTMLToken &out_token)
{
	out_token.type = HTMLToken::type_null;
	return p;
}

HTMLTokenizer::HTMLEscape HTMLTokenizer::escapes[] =
{
	// HTML 4.01: 24.2 Character entity references for ISO 8859-1 characters
	"&nbsp;", 160,
	"&iexcl;", 161,
	"&cent;", 162,
	"&pound;", 163,
	"&curren;", 164,
	"&yen;", 165,
	"&brvbar;", 166,
	"&sect;", 167,
	"&uml;", 168,
	"&copy;", 169,
	"&ordf;", 170,
	"&laquo;", 171,
	"&not;", 172,
	"&shy;", 173,
	"&reg;", 174,
	"&macr;", 175,
	"&deg;", 176,
	"&plusmn;", 177,
	"&sup2;", 178,
	"&sup3;", 179,
	"&acute;", 180,
	"&micro;", 181,
	"&para;", 182,
	"&middot;", 183,
	"&cedil;", 184,
	"&sup1;", 185,
	"&ordm;", 186,
	"&raquo;", 187,
	"&frac14;", 188,
	"&frac12;", 189,
	"&frac34;", 190,
	"&iquest;", 191,
	"&Agrave;", 192,
	"&Aacute;", 193,
	"&Acirc;", 194,
	"&Atilde;", 195,
	"&Auml;", 196,
	"&Aring;", 197,
	"&AElig;", 198,
	"&Ccedil;", 199,
	"&Egrave;", 200,
	"&Eacute;", 201,
	"&Ecirc;", 202,
	"&Euml;", 203,
	"&Igrave;", 204,
	"&Iacute;", 205,
	"&Icirc;", 206,
	"&Iuml;", 207,
	"&ETH;", 208,
	"&Ntilde;", 209,
	"&Ograve;", 210,
	"&Oacute;", 211,
	"&Ocirc;", 212,
	"&Otilde;", 213,
	"&Ouml;", 214,
	"&times;", 215,
	"&Oslash;", 216,
	"&Ugrave;", 217,
	"&Uacute;", 218,
	"&Ucirc;", 219,
	"&Uuml;", 220,
	"&Yacute;", 221,
	"&THORN;", 222,
	"&szlig;", 223,
	"&agrave;", 224,
	"&aacute;", 225,
	"&acirc;", 226,
	"&atilde;", 227,
	"&auml;", 228,
	"&aring;", 229,
	"&aelig;", 230,
	"&ccedil;", 231,
	"&egrave;", 232,
	"&eacute;", 233,
	"&ecirc;", 234,
	"&euml;", 235,
	"&igrave;", 236,
	"&iacute;", 237,
	"&icirc;", 238,
	"&iuml;", 239,
	"&eth;", 240,
	"&ntilde;", 241,
	"&ograve;", 242,
	"&oacute;", 243,
	"&ocirc;", 244,
	"&otilde;", 245,
	"&ouml;", 246,
	"&divide;", 247,
	"&oslash;", 248,
	"&ugrave;", 249,
	"&uacute;", 250,
	"&ucirc;", 251,
	"&uuml;", 252,
	"&yacute;", 253,
	"&thorn;", 254,
	"&yuml;", 255,

	// HTML 4.01: 24.3 Character entity references for symbols, mathematical symbols, and Greek letters

	"&fnof;", 402,
	"&Alpha;", 913,
	"&Beta;", 914,
	"&Gamma;", 915,
	"&Delta;", 916,
	"&Epsilon;", 917,
	"&Zeta;", 918,
	"&Eta;", 919,
	"&Theta;", 920,
	"&Iota;", 921,
	"&Kappa;", 922,
	"&Lambda;", 923,
	"&Mu;", 924,
	"&Nu;", 925,
	"&Xi;", 926,
	"&Omicron;", 927,
	"&Pi;", 928,
	"&Rho;", 929,
	"&Sigma;", 931,
	"&Tau;", 932,
	"&Upsilon;", 933,
	"&Phi;", 934,
	"&Chi;", 935,
	"&Psi;", 936,
	"&Omega;", 937,
	"&alpha;", 945,
	"&beta;", 946,
	"&gamma;", 947,
	"&delta;", 948,
	"&epsilon;", 949,
	"&zeta;", 950,
	"&eta;", 951,
	"&theta;", 952,
	"&iota;", 953,
	"&kappa;", 954,
	"&lambda;", 955,
	"&mu;", 956,
	"&nu;", 957,
	"&xi;", 958,
	"&omicron;", 959,
	"&pi;", 960,
	"&rho;", 961,
	"&sigmaf;", 962,
	"&sigma;", 963,
	"&tau;", 964,
	"&upsilon;", 965,
	"&phi;", 966,
	"&chi;", 967,
	"&psi;", 968,
	"&omega;", 969,
	"&thetasym;", 977,
	"&upsih;", 978,
	"&piv;", 982,
	"&bull;", 8226,
	"&hellip;", 8230,
	"&prime;", 8242,
	"&Prime;", 8243,
	"&oline;", 8254,
	"&frasl;", 8260,
	"&weierp;", 8472,
	"&image;", 8465,
	"&real;", 8476,
	"&trade;", 8482,
	"&alefsym;", 8501,
	"&larr;", 8592,
	"&uarr;", 8593,
	"&rarr;", 8594,
	"&darr;", 8595,
	"&harr;", 8596,
	"&crarr;", 8629,
	"&lArr;", 8656,
	"&uArr;", 8657,
	"&rArr;", 8658,
	"&dArr;", 8659,
	"&hArr;", 8660,
	"&forall;", 8704,
	"&part;", 8706,
	"&exist;", 8707,
	"&empty;", 8709,
	"&nabla;", 8711,
	"&isin;", 8712,
	"&notin;", 8713,
	"&ni;", 8715,
	"&prod;", 8719,
	"&sum;", 8721,
	"&minus;", 8722,
	"&lowast;", 8727,
	"&radic;", 8730,
	"&prop;", 8733,
	"&infin;", 8734,
	"&ang;", 8736,
	"&and;", 8743,
	"&or;", 8744,
	"&cap;", 8745,
	"&cup;", 8746,
	"&int;", 8747,
	"&there4;", 8756,
	"&sim;", 8764,
	"&cong;", 8773,
	"&asymp;", 8776,
	"&ne;", 8800,
	"&equiv;", 8801,
	"&le;", 8804,
	"&ge;", 8805,
	"&sub;", 8834,
	"&sup;", 8835,
	"&nsub;", 8836,
	"&sube;", 8838,
	"&supe;", 8839,
	"&oplus;", 8853,
	"&otimes;", 8855,
	"&perp;", 8869,
	"&sdot;", 8901,
	"&lceil;", 8968,
	"&rceil;", 8969,
	"&lfloor;", 8970,
	"&rfloor;", 8971,
	"&lang;", 9001,
	"&rang;", 9002,
	"&loz;", 9674,
	"&spades;", 9824,
	"&clubs;", 9827,
	"&hearts;", 9829,
	"&diams;", 9830,

	// HTML 4.01: 24.4 Character entity references for markup-significant and internationalization characters

	"&quot;", 34,
	"&amp;", 38,
	"&lt;", 60,
	"&gt;", 62,
	"&OElig;", 338,
	"&oelig;", 339,
	"&Scaron;", 352,
	"&scaron;", 353,
	"&Yuml;", 376,
	"&circ;", 710,
	"&tilde;", 732,
	"&ensp;", 8194,
	"&emsp;", 8195,
	"&thinsp;", 8201,
	"&zwnj;", 8204,
	"&zwj;", 8205,
	"&lrm;", 8206,
	"&rlm;", 8207,
	"&ndash;", 8211,
	"&mdash;", 8212,
	"&lsquo;", 8216,
	"&rsquo;", 8217,
	"&sbquo;", 8218,
	"&ldquo;", 8220,
	"&rdquo;", 8221,
	"&bdquo;", 8222,
	"&dagger;", 8224,
	"&Dagger;", 8225,
	"&permil;", 8240,
	"&lsaquo;", 8249,
	"&rsaquo;", 8250,
	"&euro;", 8364,

	0, 0
};

void HTMLTokenizer::unescape(std::string &text)
{
	for (size_t i = 0; i < text.length(); i++)
	{
		if (text[i] == '&')
		{
			size_t j;
			size_t length = min(i+10, text.length());
			for (j = i; j < length && text[j] != ';'; j++);
			if (j < length)
				j++;
			std::string escape = text.substr(i, j-i);
			if (escape.size() > 3 && escape[1] == '#')
			{
				unsigned int v = StringHelp::text_to_uint(escape.substr(2, escape.length()-3));
				text = text.substr(0, i) + StringHelp::wchar_to_utf8(v) + text.substr(j);
			}
			else
			{
				for (size_t k = 0; escapes[k].name != 0; k++)
				{
					if (escape == escapes[k].name)
					{
						text = text.substr(0, i) + StringHelp::wchar_to_utf8(escapes[k].cdata) + text.substr(j);
						break;
					}
				}
			}
		}
	}
}
