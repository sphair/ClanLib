
#pragma once

class GlslAstDeclarationStatement;
class GlslAstExpressionStatement;
class GlslAstIfStatement;
class GlslAstSwitchStatement;
class GlslAstCaseLabelStatement;
class GlslAstWhileStatement;
class GlslAstDoStatement;
class GlslAstForStatement;
class GlslAstContinueStatement;
class GlslAstBreakStatement;
class GlslAstReturnStatement;
class GlslAstDiscardStatement;

class GlslStatementVisitor
{
public:
	virtual void statement(GlslAstDeclarationStatement *node) = 0;
	virtual void statement(GlslAstExpressionStatement *node) = 0;
	virtual void statement(GlslAstIfStatement *node) = 0;
	virtual void statement(GlslAstSwitchStatement *node) = 0;
	virtual void statement(GlslAstCaseLabelStatement *node) = 0;
	virtual void statement(GlslAstWhileStatement *node) = 0;
	virtual void statement(GlslAstDoStatement *node) = 0;
	virtual void statement(GlslAstForStatement *node) = 0;
	virtual void statement(GlslAstContinueStatement *node) = 0;
	virtual void statement(GlslAstBreakStatement *node) = 0;
	virtual void statement(GlslAstReturnStatement *node) = 0;
	virtual void statement(GlslAstDiscardStatement *node) = 0;
};
