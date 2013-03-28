
#pragma once

#include "GLSL/Lex/glsl_token.h"
#include "glsl_ast_node.h"

class GlslAstType : public GlslAstNode
{
public:
};

class GlslAstStructType : public GlslAstType
{
public:
	std::string name;
};

class GlslAstBuiltInType : public GlslAstType
{
public:
	GlslAstBuiltInType() : type() { }

	GlslToken::Keyword type; // keyword_void, keyword_float, keyword_int, keyword_vec4, etc
};
