
#pragma once

#include "GLSL/Lex/glsl_tokenizer.h"
#include "llvm_include.h"

class GlslAstType;

class GlslOperandType
{
public:
	GlslOperandType(GlslToken::Keyword type);

	bool is_void() const;
	bool is_opaque() const;
	bool is_float() const;
	bool is_double() const;
	bool is_int() const;
	bool is_uint() const;
	bool is_bool() const;
	int columns() const;
	int rows() const;

	bool is_scalar() const { return columns() == 1 && rows() == 1; }
	bool is_vector() const { return columns() > 1 && rows() == 1; }
	bool is_matrix() const { return rows() > 1; }

	GlslOperandType to_double() const;
	GlslOperandType to_float() const;
	GlslOperandType to_int() const;
	GlslOperandType to_uint() const;
	GlslOperandType to_bool() const;

	GlslToken::Keyword primary_type;
};

class GlslOperand
{
public:
	GlslOperand();
	GlslOperand(llvm::Value *value, bool is_pointer, GlslOperandType type);

	llvm::Value *llvm_value(llvm::IRBuilder<> &builder);
	llvm::Value *llvm_pointer();

	GlslOperandType type;

private:
	llvm::Value *value;
	bool is_pointer;
};

class GlslOperandStack
{
public:
	GlslOperandStack(llvm::IRBuilder<> &builder);

	void clear();
	void push_value(llvm::Value *value, GlslOperandType type);
	void push_pointer(llvm::Value *ptr, GlslOperandType type);
	GlslOperand pop();

	static llvm::Type *get_llvm_type(GlslAstType *type, llvm::LLVMContext &context);
	static GlslOperandType get_operand_type(GlslAstType *type);

private:
	llvm::IRBuilder<> &builder;
	std::vector<GlslOperand> operand_stack;
};
