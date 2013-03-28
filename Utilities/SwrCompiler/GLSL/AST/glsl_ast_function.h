
#pragma once

#include "glsl_ast_node.h"
#include "glsl_ast_statement.h"
#include "glsl_statement_visitor.h"

class GlslAstType;
class GlslAstStatement;
class GlslAstFunctionParameter;

class GlslAstFunction : public GlslAstNode
{
public:
	GlslAstType *return_type;
	std::string name;
	std::vector<GlslAstFunctionParameter *> parameters;
	bool is_prototype;
	std::vector<GlslAstStatement *> statements;

	void codegen(GlslStatementVisitor *visitor)
	{
		for (size_t i = 0; i < statements.size(); i++)
			statements[i]->codegen(visitor);
	}
};
