
#pragma once

#include "glsl_ast_node.h"
#include "glsl_statement_visitor.h"

class GlslAstExpression;

class GlslAstStatement : public GlslAstNode
{
public:
	virtual void codegen(GlslStatementVisitor *visitor) = 0;
};

class GlslAstCompoundStatement : public GlslAstStatement
{
public:
	std::vector<GlslAstStatement *> statements;

	void codegen(GlslStatementVisitor *visitor)
	{
		for (size_t i = 0; i < statements.size(); i++)
			statements[i]->codegen(visitor);
	}
};

class GlslAstSimpleStatement : public GlslAstStatement
{
public:
};

class GlslAstEmptyStatement : public GlslAstSimpleStatement
{
public:
	void codegen(GlslStatementVisitor *visitor)
	{
	}
};

class GlslAstDeclarationStatement : public GlslAstSimpleStatement
{
public:
	std::vector<GlslAstFunctionVariable *> variables;

	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}
};

class GlslAstExpressionStatement : public GlslAstSimpleStatement
{
public:
	GlslAstExpressionStatement() : expression() { }
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}

	GlslAstExpression *expression;
};

class GlslAstIfStatement : public GlslAstSimpleStatement
{
public:
	GlslAstIfStatement() : condition(), then_statement(), else_statement() { }
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}

	GlslAstExpression *condition;
	GlslAstStatement *then_statement;
	GlslAstStatement *else_statement;
};

class GlslAstSwitchStatement : public GlslAstSimpleStatement
{
public:
	GlslAstSwitchStatement() : expression() { }
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}

	GlslAstExpression *expression;
	std::vector<GlslAstStatement *> statements;
};

class GlslAstCaseLabelStatement : public GlslAstSimpleStatement
{
public:
	GlslAstCaseLabelStatement() : condition(), is_default() { }
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}

	GlslAstExpression *condition;
	bool is_default;
};

class GlslAstIterationStatement : public GlslAstSimpleStatement
{
public:
};

class GlslAstWhileStatement : public GlslAstIterationStatement
{
public:
	GlslAstWhileStatement() : condition(), body() { }
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}

	GlslAstSimpleStatement *condition; // GlslAstExpressionStatement or GlslAstDeclarationStatement only.
	GlslAstStatement *body;
};

class GlslAstDoStatement : public GlslAstIterationStatement
{
public:
	GlslAstDoStatement() : body(), condition() { }
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}

	GlslAstStatement *body;
	GlslAstExpression *condition;
};

class GlslAstForStatement : public GlslAstIterationStatement
{
public:
	GlslAstForStatement() : init(), condition(), loop(), body() { }
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}

	GlslAstSimpleStatement *init; // GlslAstExpressionStatement or GlslAstDeclarationStatement only.
	GlslAstExpression *condition;
	GlslAstExpression *loop;
	GlslAstStatement *body;
};

class GlslAstJumpStatement : public GlslAstSimpleStatement
{
public:
};

class GlslAstContinueStatement : public GlslAstJumpStatement
{
public:
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}
};

class GlslAstBreakStatement : public GlslAstJumpStatement
{
public:
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}
};

class GlslAstReturnStatement : public GlslAstJumpStatement
{
public:
	GlslAstReturnStatement() : expression() { }
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}

	GlslAstExpression *expression;
};

class GlslAstDiscardStatement : public GlslAstJumpStatement
{
public:
	void codegen(GlslStatementVisitor *visitor)
	{
		visitor->statement(this);
	}

	// Fragment shader only.
};
