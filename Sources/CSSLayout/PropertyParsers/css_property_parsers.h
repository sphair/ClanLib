
#pragma once

namespace clan
{

class CSSProperty;
class CSSPropertyParser;
class CSSBoxProperties;
class CSSPropertyValue;

class CSSPropertyParsers
{
public:
	CSSPropertyParsers();
	~CSSPropertyParsers();

	void parse(CSSBoxProperties &properties, const CSSProperty &property, std::map<std::string, CSSPropertyValue *> *out_change_set = 0);

private:
	void add(CSSPropertyParser *parser);

	std::vector<CSSPropertyParser *> parsers;
	std::map<std::string, CSSPropertyParser *> name_to_parser;
};

}
