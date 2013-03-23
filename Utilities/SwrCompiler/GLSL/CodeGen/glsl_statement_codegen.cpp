
#include "precomp.h"
#include "glsl_statement_codegen.h"
#include "glsl_expression_codegen.h"
#include "glsl_name_scope.h"
#include "GLSL/FixedFunction/glsl_program.h"

using namespace clan;

GlslStatementCodeGen::GlslStatementCodeGen(GlslProgram &program, GlslNameScope &name_scope, llvm::IRBuilder<> &builder)
: program(program), name_scope(name_scope), builder(builder), basic_block_terminated(false)
{
}

void GlslStatementCodeGen::codegen(GlslAstFunction *node)
{
	basic_block_terminated = false; // To do: this method is broken if there's more than one return statement in the function.
	node->codegen(this);
	if (!basic_block_terminated)
		builder.CreateRetVoid();
}

void GlslStatementCodeGen::statement(GlslAstDeclarationStatement *node)
{
	llvm::BasicBlock &entry_block = builder.GetInsertBlock()->getParent()->getEntryBlock();
	for (size_t i = 0; i < node->variables.size(); i++)
	{
		llvm::Value *array_size = 0;
		llvm::IRBuilder<> alloca_builder(&entry_block, entry_block.begin()); // Allocas must be created at top of entry block for the PromoteMemoryToRegisterPass to work
		llvm::Value *ptr = alloca_builder.CreateAlloca(GlslOperandStack::get_llvm_type(node->variables[i]->type, context()), array_size);
		if (node->variables[i]->initial_value)
		{
			GlslExpressionCodeGen expression(program, name_scope, builder);
			GlslOperand initial_value = expression.codegen(node->variables[i]->initial_value);
			builder.CreateStore(initial_value.llvm_value(builder), ptr, false);
		}
		name_scope.variables[node->variables[i]] = ptr;
		last_expression_result = GlslOperand(ptr, true, GlslOperandStack::get_operand_type(node->variables[i]->type));
	}
}

void GlslStatementCodeGen::statement(GlslAstExpressionStatement *node)
{
	GlslExpressionCodeGen expression(program, name_scope, builder);
	last_expression_result = expression.codegen(node->expression);
}

void GlslStatementCodeGen::statement(GlslAstIfStatement *node)
{
	GlslExpressionCodeGen expression(program, name_scope, builder);
	GlslOperand condition = expression.codegen(node->condition);
	if (node->else_statement == 0)
	{
		llvm::BasicBlock *if_block = llvm::BasicBlock::Create(context(), "if", builder.GetInsertBlock()->getParent());
		llvm::BasicBlock *end_block = llvm::BasicBlock::Create(context(), "endif", builder.GetInsertBlock()->getParent());
		builder.CreateCondBr(condition.llvm_value(builder), if_block, end_block);
		builder.SetInsertPoint(if_block);
		node->then_statement->codegen(this);
		builder.CreateBr(end_block);
		builder.SetInsertPoint(end_block);
	}
	else
	{
		llvm::BasicBlock *if_block = llvm::BasicBlock::Create(context(), "if", builder.GetInsertBlock()->getParent());
		llvm::BasicBlock *else_block = llvm::BasicBlock::Create(context(), "else", builder.GetInsertBlock()->getParent());
		llvm::BasicBlock *end_block = llvm::BasicBlock::Create(context(), "endif", builder.GetInsertBlock()->getParent());
		builder.CreateCondBr(condition.llvm_value(builder), if_block, else_block);
		builder.SetInsertPoint(if_block);
		node->then_statement->codegen(this);
		builder.CreateBr(end_block);
		builder.SetInsertPoint(else_block);
		node->else_statement->codegen(this);
		builder.CreateBr(end_block);
		builder.SetInsertPoint(end_block);
	}
}

void GlslStatementCodeGen::statement(GlslAstSwitchStatement *node)
{
	throw Exception("Codegen for 'switch' not implemented");
	//node->expression->codegen(this);
	//for (size_t i = 0; i < node->statements.size(); i++)
	//{
	//	node->statements[i]->codegen(this);
	//}
}

void GlslStatementCodeGen::statement(GlslAstCaseLabelStatement *node)
{
	throw Exception("Codegen for case labels not implemented");
}

void GlslStatementCodeGen::statement(GlslAstWhileStatement *node)
{
	llvm::BasicBlock *while_block = llvm::BasicBlock::Create(context(), "while", builder.GetInsertBlock()->getParent());
	llvm::BasicBlock *loop_block = llvm::BasicBlock::Create(context(), "loop", builder.GetInsertBlock()->getParent());
	llvm::BasicBlock *end_block = llvm::BasicBlock::Create(context(), "endwhile", builder.GetInsertBlock()->getParent());
	builder.CreateBr(while_block);
	builder.SetInsertPoint(while_block);
	node->condition->codegen(this);
	GlslOperand condition = last_expression_result;
	builder.CreateCondBr(condition.llvm_value(builder), loop_block, end_block);
	builder.SetInsertPoint(loop_block);
	node->body->codegen(this);
	builder.CreateBr(while_block);
	builder.SetInsertPoint(end_block);
}

void GlslStatementCodeGen::statement(GlslAstDoStatement *node)
{
	llvm::BasicBlock *do_block = llvm::BasicBlock::Create(context(), "do", builder.GetInsertBlock()->getParent());
	llvm::BasicBlock *end_block = llvm::BasicBlock::Create(context(), "enddo", builder.GetInsertBlock()->getParent());
	builder.CreateBr(do_block);
	builder.SetInsertPoint(do_block);
	node->body->codegen(this);
	GlslExpressionCodeGen expression(program, name_scope, builder);
	GlslOperand condition = expression.codegen(node->condition);
	builder.CreateCondBr(condition.llvm_value(builder), do_block, end_block);
	builder.SetInsertPoint(end_block);
}

void GlslStatementCodeGen::statement(GlslAstForStatement *node)
{
	node->init->codegen(this);
	llvm::BasicBlock *for_block = llvm::BasicBlock::Create(context(), "for", builder.GetInsertBlock()->getParent());
	llvm::BasicBlock *loop_block = llvm::BasicBlock::Create(context(), "loop", builder.GetInsertBlock()->getParent());
	llvm::BasicBlock *end_block = llvm::BasicBlock::Create(context(), "endfor", builder.GetInsertBlock()->getParent());
	builder.CreateBr(for_block);
	builder.SetInsertPoint(for_block);
	GlslExpressionCodeGen expression(program, name_scope, builder);
	GlslOperand condition = expression.codegen(node->condition);
	builder.CreateCondBr(condition.llvm_value(builder), loop_block, end_block);
	builder.SetInsertPoint(loop_block);
	node->body->codegen(this);
	expression.codegen(node->loop);
	builder.CreateBr(for_block);
	builder.SetInsertPoint(end_block);
}

void GlslStatementCodeGen::statement(GlslAstContinueStatement *node)
{
	throw Exception("Codegen for 'continue' not implemented");
}

void GlslStatementCodeGen::statement(GlslAstBreakStatement *node)
{
	throw Exception("Codegen for 'break' not implemented");
}

void GlslStatementCodeGen::statement(GlslAstReturnStatement *node)
{
	basic_block_terminated = true;
	if (node->expression)
	{
		GlslExpressionCodeGen expression(program, name_scope, builder);
		GlslOperand result = expression.codegen(node->expression);
		builder.CreateRet(result.llvm_value(builder));
	}
	else
	{
		builder.CreateRetVoid();
	}
}

void GlslStatementCodeGen::statement(GlslAstDiscardStatement *node)
{
	throw Exception("Codegen for 'discard' not implemented");
}

llvm::LLVMContext &GlslStatementCodeGen::context()
{
	return program.context();
}
