
#pragma once

#include "llvm_include.h"
#include "GLSL/AST/glsl_ast.h"
#include "glsl_operand.h"

class GlslProgram;
class GlslNameScope;
class GlslOperand;

class GlslStatementCodeGen : GlslStatementVisitor
{
public:
	GlslStatementCodeGen(GlslProgram &program, GlslNameScope &name_scope, llvm::IRBuilder<> &builder);
	void codegen(GlslAstFunction *node);

private:
	void statement(GlslAstDeclarationStatement *node);
	void statement(GlslAstExpressionStatement *node);
	void statement(GlslAstIfStatement *node);
	void statement(GlslAstSwitchStatement *node);
	void statement(GlslAstCaseLabelStatement *node);
	void statement(GlslAstWhileStatement *node);
	void statement(GlslAstDoStatement *node);
	void statement(GlslAstForStatement *node);
	void statement(GlslAstContinueStatement *node);
	void statement(GlslAstBreakStatement *node);
	void statement(GlslAstReturnStatement *node);
	void statement(GlslAstDiscardStatement *node);
	llvm::LLVMContext &context();

	GlslProgram &program;
	GlslNameScope &name_scope;
	llvm::IRBuilder<> &builder;
	bool basic_block_terminated;
	GlslOperand last_expression_result;
};
