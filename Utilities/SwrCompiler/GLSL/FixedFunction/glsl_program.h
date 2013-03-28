
#pragma once

#include "llvm_include.h"

class GlslProgram
{
public:
	GlslProgram();
	~GlslProgram();
	void *compile(std::string name);

	llvm::LLVMContext &context() { return *_context; }
	llvm::Module *module() { return _module; }

private:
	llvm::LLVMContext *_context;
	llvm::Module *_module;
	llvm::ExecutionEngine *_engine;
	llvm::PassManager *pass_manager;
};
