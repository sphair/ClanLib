
#pragma once

namespace clan
{

class CSSProperty;
class CSSPropertyParser;
class CSSComputedBox;
class CSSPropertyValue;

class CSSPropertyParsers
{
public:
	CSSPropertyParsers();
	~CSSPropertyParsers();

	void parse(const CSSProperty &property, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values);

private:
	void add(CSSPropertyParser *parser);

	std::vector<CSSPropertyParser *> parsers;
	std::map<std::string, CSSPropertyParser *> name_to_parser;
};

}
