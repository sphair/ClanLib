
#pragma once

#include "GLSL/AST/glsl_ast.h"
#include "GLSL/AST/glsl_expression_visitor.h"
#include "glsl_operand.h"
#include "GLSL/llvm_include.h"

class GlslProgram;
class GlslNameScope;

class GlslExpressionCodeGen : GlslExpressionVisitor
{
public:
	GlslExpressionCodeGen(GlslProgram &program, GlslNameScope &name_scope, llvm::IRBuilder<> &builder);
	GlslOperand codegen(GlslAstExpression *expression);

	GlslOperand implicit_to_double(GlslOperand operand);
	GlslOperand implicit_to_float(GlslOperand operand);
	GlslOperand implicit_to_uint(GlslOperand operand);

private:
	void expression(GlslAstIntConstant *node);
	void expression(GlslAstUIntConstant *node);
	void expression(GlslAstFloatConstant *node);
	void expression(GlslAstDoubleConstant *node);
	void expression(GlslAstBoolConstant *node);
	void expression(GlslAstVariableIdentifer *node);
	void expression(GlslAstFieldSelectorOrSwizzle *node);
	void expression(GlslAstArraySubscript *node);
	void expression(GlslAstFunctionCall *node);
	void expression(GlslAstConstructorCall *node);
	void expression(GlslAstUnaryPrefixIncrementExpression *node);
	void expression(GlslAstUnaryPrefixDecrementExpression *node);
	void expression(GlslAstUnaryPostfixIncrementExpression *node);
	void expression(GlslAstUnaryPostfixDecrementExpression *node);
	void expression(GlslAstUnaryPlusExpression *node);
	void expression(GlslAstUnaryMinusExpression *node);
	void expression(GlslAstUnaryBitNotExpression *node);
	void expression(GlslAstUnaryLogicalNotExpression *node);
	void expression(GlslAstAssignmentExpression *node);
	void expression(GlslAstPlusExpression *node);
	void expression(GlslAstMinusExpression *node);
	void expression(GlslAstMultiplyExpression *node);
	void expression(GlslAstDivideExpression *node);
	void expression(GlslAstModulusExpression *node);
	void expression(GlslAstShiftLeftExpression *node);
	void expression(GlslAstShiftRightExpression *node);
	void expression(GlslAstLessExpression *node);
	void expression(GlslAstLessEqualExpression *node);
	void expression(GlslAstGreaterExpression *node);
	void expression(GlslAstGreaterEqualExpression *node);
	void expression(GlslAstEqualExpression *node);
	void expression(GlslAstNotEqualExpression *node);
	void expression(GlslAstBitAndExpression *node);
	void expression(GlslAstBitXorExpression *node);
	void expression(GlslAstBitOrExpression *node);
	void expression(GlslAstLogicalAndExpression *node);
	void expression(GlslAstLogicalXorExpression *node);
	void expression(GlslAstLogicalOrExpression *node);
	void expression(GlslAstSelectExpression *node);
	void expression(GlslAstSequenceExpression *node);
	void construct_void(GlslAstConstructorCall *node);
	void construct_float(GlslAstConstructorCall *node);
	void construct_double(GlslAstConstructorCall *node);
	void construct_int(GlslAstConstructorCall *node);
	void construct_uint(GlslAstConstructorCall *node);
	void construct_bool(GlslAstConstructorCall *node);
	void construct_vec2(GlslAstConstructorCall *node);
	void construct_vec3(GlslAstConstructorCall *node);
	void construct_vec4(GlslAstConstructorCall *node);
	void construct_dvec2(GlslAstConstructorCall *node);
	void construct_dvec3(GlslAstConstructorCall *node);
	void construct_dvec4(GlslAstConstructorCall *node);
	void construct_ivec2(GlslAstConstructorCall *node);
	void construct_ivec3(GlslAstConstructorCall *node);
	void construct_ivec4(GlslAstConstructorCall *node);
	void construct_uvec2(GlslAstConstructorCall *node);
	void construct_uvec3(GlslAstConstructorCall *node);
	void construct_uvec4(GlslAstConstructorCall *node);
	void construct_bvec2(GlslAstConstructorCall *node);
	void construct_bvec3(GlslAstConstructorCall *node);
	void construct_bvec4(GlslAstConstructorCall *node);
	void construct_opaque(GlslAstConstructorCall *node);
	void construct_mat2(GlslAstConstructorCall *node);
	void construct_mat2x3(GlslAstConstructorCall *node);
	void construct_mat2x4(GlslAstConstructorCall *node);
	void construct_mat3(GlslAstConstructorCall *node);
	void construct_mat3x2(GlslAstConstructorCall *node);
	void construct_mat3x4(GlslAstConstructorCall *node);
	void construct_mat4(GlslAstConstructorCall *node);
	void construct_mat4x2(GlslAstConstructorCall *node);
	void construct_mat4x3(GlslAstConstructorCall *node);
	void construct_dmat2(GlslAstConstructorCall *node);
	void construct_dmat2x3(GlslAstConstructorCall *node);
	void construct_dmat2x4(GlslAstConstructorCall *node);
	void construct_dmat3(GlslAstConstructorCall *node);
	void construct_dmat3x2(GlslAstConstructorCall *node);
	void construct_dmat3x4(GlslAstConstructorCall *node);
	void construct_dmat4(GlslAstConstructorCall *node);
	void construct_dmat4x2(GlslAstConstructorCall *node);
	void construct_dmat4x3(GlslAstConstructorCall *node);
	void implicit_convert(GlslOperand &operand1, GlslOperand &operand2);
	void scalar_to_vector(GlslOperand &v1, GlslOperand &v2);
	llvm::Value *matrix_column(llvm::Value *matrix, int index);
	llvm::Value *vector_shuffle(llvm::Value *vector, int index0, int index1, int index2, int index3);
	void transpose4(llvm::Value *&element0, llvm::Value *&element1, llvm::Value *&element2, llvm::Value *&element3);
	llvm::LLVMContext &context();

	GlslProgram &program;
	GlslNameScope &name_scope;
	llvm::IRBuilder<> &builder;
	GlslOperandStack operands;
};
