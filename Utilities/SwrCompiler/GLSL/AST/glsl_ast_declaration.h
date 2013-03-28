
#pragma once

#include "glsl_ast_node.h"

class GlslAstVariable;
class GlslAstAssignmentExpression;

class GlslAstDeclaration : public GlslAstNode
{
public:
	GlslAstDeclaration() : variable(), assignment() { }

	GlslAstVariable *variable;
	GlslAstAssignmentExpression *assignment;
};
