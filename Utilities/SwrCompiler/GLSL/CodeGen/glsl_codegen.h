
#pragma once

#include "GLSL/llvm_include.h"
#include "GLSL/AST/glsl_ast.h"
#include "glsl_name_scope.h"

class GlslProgram;

class GlslCodeGen
{
public:
	enum ShaderType
	{
		vertex_shader,
		fragment_shader
	};

	GlslCodeGen(GlslProgram &program, ShaderType shader_type);
	void codegen(GlslAstTranslationUnit *ast);

	const std::vector<GlslAstGlobalVariable *> &get_global_struct_variables() const { return global_struct_variables; }
	llvm::StructType *get_global_struct_type() const { return global_struct_type; }

	std::string shader_prefix() const;

private:
	void function(GlslAstFunction *node);
	void prototype(GlslAstFunction *node);
	void global(GlslAstGlobalVariable *node);

	llvm::Function *create_function(GlslAstFunction *node);

	GlslProgram &program;
	ShaderType shader_type;
	llvm::LLVMContext &context;
	llvm::Module *module;
	llvm::IRBuilder<> builder;
	std::vector<GlslAstGlobalVariable *> global_struct_variables;
	llvm::StructType *global_struct_type;
	GlslNameScope global_scope;
};
