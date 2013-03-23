
#include "precomp.h"
#include "glsl_codegen.h"
#include "GLSL/AST/glsl_ast.h"
#include "GLSL/FixedFunction/glsl_program.h"
#include "GLSL/FixedFunction/glsl_fixed_function.h"
#include "glsl_operand.h"
#include "glsl_statement_codegen.h"

using namespace clan;

GlslCodeGen::GlslCodeGen(GlslProgram &program, ShaderType shader_type)
: program(program), shader_type(shader_type), context(program.context()), module(program.module()), builder(program.context()), global_struct_type(0)
{
}

void GlslCodeGen::codegen(GlslAstTranslationUnit *ast)
{
	for (size_t i = 0; i < ast->globals.size(); i++)
	{
		global(ast->globals[i]);
	}

	std::vector<llvm::Type *> global_struct_elements;
	for (size_t i = 0; i < global_struct_variables.size(); i++)
		global_struct_elements.push_back(GlslOperandStack::get_llvm_type(global_struct_variables[i]->type, context));
	global_struct_type = llvm::StructType::get(context, global_struct_elements, false);

	for (size_t i = 0; i < ast->functions.size(); i++)
	{
		if (ast->functions[i]->is_prototype)
			prototype(ast->functions[i]);
	}

	for (size_t i = 0; i < ast->functions.size(); i++)
	{
		if (!ast->functions[i]->is_prototype)
			function(ast->functions[i]);
	}
}

void GlslCodeGen::function(GlslAstFunction *node)
{
	llvm::Function *func = create_function(node);
	if (!func->empty())
		throw Exception("Redefinition of function");
	llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", func);
	builder.SetInsertPoint(entry);

	llvm::Function::arg_iterator arg_it = func->arg_begin();

	llvm::Value *globals = arg_it++;
	for (size_t i = 0; i < global_struct_variables.size(); i++)
	{
		std::vector<llvm::Value *> indexes;
		indexes.push_back(llvm::ConstantInt::get(context, llvm::APInt(32, (uint64_t)0)));
		indexes.push_back(llvm::ConstantInt::get(context, llvm::APInt(32, i)));
		global_scope.variables[global_struct_variables[i]] = builder.CreateGEP(globals, indexes);
	}

	for (size_t i = 0; i < node->parameters.size(); i++)
	{
		if (node->parameters[i]->is_out)
		{
			global_scope.variables[node->parameters[i]] = arg_it;
		}
		else
		{
			llvm::Value *array_size = 0;
			llvm::IRBuilder<> alloca_builder(entry, entry->begin()); // Allocas must be created at top of entry block for the PromoteMemoryToRegisterPass to work
			llvm::Value *ptr = alloca_builder.CreateAlloca(GlslOperandStack::get_llvm_type(node->parameters[i]->type, context), array_size);
			global_scope.variables[node->parameters[i]] = ptr;
			builder.CreateStore(arg_it, ptr, false);
		}
		++arg_it;
	}

	GlslStatementCodeGen statements(program, global_scope, builder);
	statements.codegen(node);

	llvm::verifyFunction(*func);
}

llvm::Function *GlslCodeGen::create_function(GlslAstFunction *node)
{
	std::string function_name = shader_prefix() + node->name;
	llvm::Type *return_type = GlslOperandStack::get_llvm_type(node->return_type, context);
	std::vector<llvm::Type *> parameters;
	parameters.reserve(node->parameters.size() + 1);
	parameters.push_back(global_struct_type->getPointerTo());
	for (size_t i = 0; i < node->parameters.size(); i++)
	{
		llvm::Type *type = GlslOperandStack::get_llvm_type(node->parameters[i]->type, context);
		if (node->parameters[i]->is_out)
			type = type->getPointerTo();
		parameters.push_back(type);
	}
	llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, parameters, false);
	llvm::Function *func = module->getFunction(function_name.c_str());
	if (func == 0)
	{
		func = llvm::Function::Create(func_type, llvm::Function::PrivateLinkage, function_name.c_str(), module);
		func->addFnAttr(llvm::Attribute::AlwaysInline);
	}
	return func;
}

void GlslCodeGen::prototype(GlslAstFunction *node)
{
	llvm::Function *func = create_function(node);
}

void GlslCodeGen::global(GlslAstGlobalVariable *node)
{
	global_struct_variables.push_back(node);
}

std::string GlslCodeGen::shader_prefix() const
{
	switch (shader_type)
	{
	case vertex_shader:
		return "vertex_";
	case fragment_shader:
		return "fragment_";
	default:
		throw Exception("Unknown shader type");
	}
}
