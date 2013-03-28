
#include "precomp.h"
#include "glsl_expression_codegen.h"
#include "glsl_name_scope.h"
#include "GLSL/FixedFunction/glsl_program.h"
#include "GLSL/FixedFunction/SSA/ssa_vec4f.h"

using namespace clan;

GlslExpressionCodeGen::GlslExpressionCodeGen(GlslProgram &program, GlslNameScope &name_scope, llvm::IRBuilder<> &builder)
: program(program), name_scope(name_scope), builder(builder), operands(builder)
{
}

GlslOperand GlslExpressionCodeGen::codegen(GlslAstExpression *expression)
{
	operands.clear();
	expression->codegen(this);
	return operands.pop();
}

void GlslExpressionCodeGen::expression(GlslAstIntConstant *node)
{
	operands.push_value(llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)node->value, true)), GlslToken::keyword_int);
}

void GlslExpressionCodeGen::expression(GlslAstUIntConstant *node)
{
	operands.push_value(llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)node->value, false)), GlslToken::keyword_uint);
}

void GlslExpressionCodeGen::expression(GlslAstFloatConstant *node)
{
	operands.push_value(llvm::ConstantFP::get(context(), llvm::APFloat(node->value)), GlslToken::keyword_float);
}

void GlslExpressionCodeGen::expression(GlslAstDoubleConstant *node)
{
	operands.push_value(llvm::ConstantFP::get(context(), llvm::APFloat(node->value)), GlslToken::keyword_double);
}

void GlslExpressionCodeGen::expression(GlslAstBoolConstant *node)
{
	operands.push_value(llvm::ConstantInt::get(context(), llvm::APInt(1, (uint64_t)(node->value ? 1 : 0), false)), GlslToken::keyword_bool);
}

void GlslExpressionCodeGen::expression(GlslAstVariableIdentifer *node)
{
	operands.push_pointer(name_scope.variables[node->variable], operands.get_operand_type(node->variable->type));
}

void GlslExpressionCodeGen::expression(GlslAstFieldSelectorOrSwizzle *node)
{
	throw Exception("Codegen for field selector or swizzle not implemented");
}

void GlslExpressionCodeGen::expression(GlslAstArraySubscript *node)
{
	node->operand->codegen(this);
	node->index->codegen(this);
	GlslOperand index = operands.pop();
	GlslOperand ptr = operands.pop();
	operands.push_pointer(builder.CreateGEP(ptr.llvm_pointer(), index.llvm_value(builder)), ptr.type);
}

void GlslExpressionCodeGen::expression(GlslAstFunctionCall *node)
{
	// To do: handle 'out' variables
	// To do: set the correct type of the pushed value
	std::vector<llvm::Value *> args;
	args.push_back(builder.GetInsertBlock()->getParent()->arg_begin()); // globals
	for (size_t i = 0; i < node->parameters.size(); i++)
	{
		node->parameters[i]->codegen(this);
		args.push_back(operands.pop().llvm_value(builder));
	}
	std::string function_name = "fragment_" + node->name; // To do: use GlslNameScope to loop up functions
	GlslOperandType return_type = GlslToken::keyword_float;
	if (node->name == "texture" || node->name == "normalize" || node->name == "reflect" || node->name == "mix")
		return_type = GlslOperandType(GlslToken::keyword_vec4);
	operands.push_value(builder.CreateCall(program.module()->getFunction(function_name.c_str()), args), return_type);
}

void GlslExpressionCodeGen::expression(GlslAstConstructorCall *node)
{
	GlslAstBuiltInType *built_in = dynamic_cast<GlslAstBuiltInType*>(node->type);
	if (built_in)
	{
		switch (built_in->type)
		{
		case GlslToken::keyword_void: construct_void(node); break;
		case GlslToken::keyword_float: construct_float(node); break;
		case GlslToken::keyword_double: construct_double(node); break;
		case GlslToken::keyword_int: construct_int(node); break;
		case GlslToken::keyword_uint: construct_uint(node); break;
		case GlslToken::keyword_vec2: construct_vec2(node); break;
		case GlslToken::keyword_vec3: construct_vec3(node); break;
		case GlslToken::keyword_vec4: construct_vec4(node); break;
		case GlslToken::keyword_dvec2: construct_dvec2(node); break;
		case GlslToken::keyword_dvec3: construct_dvec3(node); break;
		case GlslToken::keyword_dvec4: construct_dvec4(node); break;
		case GlslToken::keyword_ivec2: construct_ivec2(node); break;
		case GlslToken::keyword_ivec3: construct_ivec3(node); break;
		case GlslToken::keyword_ivec4: construct_ivec4(node); break;
		case GlslToken::keyword_uvec2: construct_uvec2(node); break;
		case GlslToken::keyword_uvec3: construct_uvec3(node); break;
		case GlslToken::keyword_uvec4: construct_uvec4(node); break;
		case GlslToken::keyword_bool: construct_bool(node); break;
		case GlslToken::keyword_bvec2: construct_bvec2(node); break;
		case GlslToken::keyword_bvec3: construct_bvec3(node); break;
		case GlslToken::keyword_bvec4: construct_bvec4(node); break;
		case GlslToken::keyword_sampler1D:
		case GlslToken::keyword_sampler2D:
		case GlslToken::keyword_sampler3D:
		case GlslToken::keyword_samplerCube:
		case GlslToken::keyword_sampler1DShadow:
		case GlslToken::keyword_sampler2DShadow:
		case GlslToken::keyword_samplerCubeShadow:
		case GlslToken::keyword_sampler1DArray:
		case GlslToken::keyword_sampler2DArray:
		case GlslToken::keyword_sampler1DArrayShadow:
		case GlslToken::keyword_sampler2DArrayShadow:
		case GlslToken::keyword_samplerCubeArray:
		case GlslToken::keyword_samplerCubeArrayShadow:
		case GlslToken::keyword_isampler1D:
		case GlslToken::keyword_isampler2D:
		case GlslToken::keyword_isampler3D:
		case GlslToken::keyword_isamplerCube:
		case GlslToken::keyword_isampler1DArray:
		case GlslToken::keyword_isampler2DArray:
		case GlslToken::keyword_isamplerCubeArray:
		case GlslToken::keyword_usampler1D:
		case GlslToken::keyword_usampler2D:
		case GlslToken::keyword_usampler3D:
		case GlslToken::keyword_usamplerCube:
		case GlslToken::keyword_usampler1DArray:
		case GlslToken::keyword_usampler2DArray:
		case GlslToken::keyword_usamplerCubeArray:
		case GlslToken::keyword_sampler2DRect:
		case GlslToken::keyword_sampler2DRectShadow:
		case GlslToken::keyword_isampler2DRect:
		case GlslToken::keyword_usampler2DRect:
		case GlslToken::keyword_samplerBuffer:
		case GlslToken::keyword_isamplerBuffer:
		case GlslToken::keyword_usamplerBuffer:
		case GlslToken::keyword_sampler2DMS:
		case GlslToken::keyword_isampler2DMS:
		case GlslToken::keyword_usampler2DMS:
		case GlslToken::keyword_sampler2DMSArray:
		case GlslToken::keyword_isampler2DMSArray:
		case GlslToken::keyword_usampler2DMSArray:
			construct_opaque(node);
			break;
		case GlslToken::keyword_mat2: construct_mat2(node); break;
		case GlslToken::keyword_mat3: construct_mat3(node); break;
		case GlslToken::keyword_mat4: construct_mat4(node); break;
		case GlslToken::keyword_mat2x2: construct_mat2(node); break;
		case GlslToken::keyword_mat2x3: construct_mat2x3(node); break;
		case GlslToken::keyword_mat2x4: construct_mat2x4(node); break;
		case GlslToken::keyword_mat3x2: construct_mat3x2(node); break;
		case GlslToken::keyword_mat3x3: construct_mat3(node); break;
		case GlslToken::keyword_mat3x4: construct_mat3x4(node); break;
		case GlslToken::keyword_mat4x2: construct_mat4x2(node); break;
		case GlslToken::keyword_mat4x3: construct_mat4x3(node); break;
		case GlslToken::keyword_mat4x4: construct_mat4(node); break;
		case GlslToken::keyword_dmat2: construct_dmat2(node); break;
		case GlslToken::keyword_dmat3: construct_dmat3(node); break;
		case GlslToken::keyword_dmat4: construct_dmat4(node); break;
		case GlslToken::keyword_dmat2x2: construct_dmat2(node); break;
		case GlslToken::keyword_dmat2x3: construct_dmat2x3(node); break;
		case GlslToken::keyword_dmat2x4: construct_dmat2x4(node); break;
		case GlslToken::keyword_dmat3x2: construct_dmat3x2(node); break;
		case GlslToken::keyword_dmat3x3: construct_dmat3(node); break;
		case GlslToken::keyword_dmat3x4: construct_dmat3x4(node); break;
		case GlslToken::keyword_dmat4x2: construct_dmat4x2(node); break;
		case GlslToken::keyword_dmat4x3: construct_dmat4x3(node); break;
		case GlslToken::keyword_dmat4x4: construct_dmat4(node); break;
		default:
			throw Exception("Unknown type");
		}
	}
	else
	{
		throw Exception("Codegen for constructor calls not implemented");
	}
}

void GlslExpressionCodeGen::expression(GlslAstUnaryPrefixIncrementExpression *node)
{
	node->operand->codegen(this);
	GlslOperand ptr = operands.pop();
	llvm::Value *before = builder.CreateLoad(ptr.llvm_pointer(), false);
	llvm::Value *after;
	if (ptr.type.is_bool() || ptr.type.is_int() || ptr.type.is_uint())
		after = builder.CreateAdd(before, llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)1, true)));
	else if (ptr.type.is_float() || ptr.type.is_double())
		after = builder.CreateFAdd(before, llvm::ConstantFP::get(context(), llvm::APFloat(1.0f)));
	else
		throw Exception("Unexpected type");
	builder.CreateStore(after, ptr.llvm_pointer());
	operands.push_value(after, ptr.type);
}

void GlslExpressionCodeGen::expression(GlslAstUnaryPrefixDecrementExpression *node)
{
	node->operand->codegen(this);
	GlslOperand ptr = operands.pop();
	llvm::Value *before = builder.CreateLoad(ptr.llvm_pointer(), false);
	llvm::Value *after;
	if (ptr.type.is_bool() || ptr.type.is_int() || ptr.type.is_uint())
		after = builder.CreateSub(before, llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)1, true)));
	else if (ptr.type.is_float() || ptr.type.is_double())
		after = builder.CreateFSub(before, llvm::ConstantFP::get(context(), llvm::APFloat(1.0f)));
	else
		throw Exception("Unexpected type");
	builder.CreateStore(after, ptr.llvm_pointer());
	operands.push_value(after, ptr.type);
}

void GlslExpressionCodeGen::expression(GlslAstUnaryPostfixIncrementExpression *node)
{
	node->operand->codegen(this);
	GlslOperand ptr = operands.pop();
	llvm::Value *before = builder.CreateLoad(ptr.llvm_pointer(), false);
	llvm::Value *after;
	if (ptr.type.is_bool() || ptr.type.is_int() || ptr.type.is_uint())
		after = builder.CreateAdd(before, llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)1, true)));
	else if (ptr.type.is_float() || ptr.type.is_double())
		after = builder.CreateFAdd(before, llvm::ConstantFP::get(context(), llvm::APFloat(1.0f)));
	else
		throw Exception("Unexpected type");
	builder.CreateStore(after, ptr.llvm_pointer());
	operands.push_value(before, ptr.type);
}

void GlslExpressionCodeGen::expression(GlslAstUnaryPostfixDecrementExpression *node)
{
	node->operand->codegen(this);
	GlslOperand ptr = operands.pop();
	llvm::Value *before = builder.CreateLoad(ptr.llvm_pointer(), false);
	llvm::Value *after;
	if (ptr.type.is_bool() || ptr.type.is_int() || ptr.type.is_uint())
		after = builder.CreateSub(before, llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)1, true)));
	else if (ptr.type.is_float() || ptr.type.is_double())
		after = builder.CreateFSub(before, llvm::ConstantFP::get(context(), llvm::APFloat(1.0f)));
	else
		throw Exception("Unexpected type");
	builder.CreateStore(after, ptr.llvm_pointer());
	operands.push_value(before, ptr.type);
}

void GlslExpressionCodeGen::expression(GlslAstUnaryPlusExpression *node)
{
	node->operand->codegen(this);
}

void GlslExpressionCodeGen::expression(GlslAstUnaryMinusExpression *node)
{
	node->operand->codegen(this);
	GlslOperand v = operands.pop();
	if (v.type.is_bool() || v.type.is_int() || v.type.is_uint())
		operands.push_value(builder.CreateNeg(v.llvm_value(builder)), v.type);
	else if (v.type.is_float() || v.type.is_double())
		operands.push_value(builder.CreateFNeg(v.llvm_value(builder)), v.type);
	else
		throw Exception("Unexpected type");
}

void GlslExpressionCodeGen::expression(GlslAstUnaryBitNotExpression *node)
{
	node->operand->codegen(this);
	GlslOperand v = operands.pop();
	operands.push_value(builder.CreateNot(v.llvm_value(builder)), v.type);
}

void GlslExpressionCodeGen::expression(GlslAstUnaryLogicalNotExpression *node)
{
	node->operand->codegen(this);
	GlslOperand v = operands.pop();
	if (v.type.is_bool() || v.type.is_int() || v.type.is_uint())
		operands.push_value(builder.CreateICmpNE(v.llvm_value(builder), llvm::ConstantInt::getFalse(context())), v.type);
	else if (v.type.is_float() || v.type.is_double())
		operands.push_value(builder.CreateFCmpUNE(v.llvm_value(builder), llvm::ConstantInt::getFalse(context())), v.type);
	else
		throw Exception("Unexpected type");
}

void GlslExpressionCodeGen::expression(GlslAstAssignmentExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand rvalue = operands.pop();
	GlslOperand lvalue = operands.pop();

	if (node->assignment_type == GlslToken::operator_assign)
	{
		builder.CreateStore(rvalue.llvm_value(builder), lvalue.llvm_pointer(), false);
		operands.push_pointer(lvalue.llvm_pointer(), lvalue.type);
	}
	else
	{
		llvm::Value *before = builder.CreateLoad(lvalue.llvm_pointer(), false);
		llvm::Value *after = 0;
		switch (node->assignment_type)
		{
		case GlslToken::operator_add_assign:
			if (lvalue.type.is_bool() || lvalue.type.is_int() || lvalue.type.is_uint())
				after = builder.CreateAdd(before, rvalue.llvm_value(builder));
			else
				after = builder.CreateFAdd(before, rvalue.llvm_value(builder));
			break;
		case GlslToken::operator_sub_assign:
			if (lvalue.type.is_bool() || lvalue.type.is_int() || lvalue.type.is_uint())
				after = builder.CreateSub(before, rvalue.llvm_value(builder));
			else
				after = builder.CreateFSub(before, rvalue.llvm_value(builder));
			break;
		case GlslToken::operator_multiply_assign:
			if (lvalue.type.is_bool() || lvalue.type.is_int() || lvalue.type.is_uint())
				after = builder.CreateMul(before, rvalue.llvm_value(builder));
			else
				after = builder.CreateFMul(before, rvalue.llvm_value(builder));
			break;
		case GlslToken::operator_divide_assign:
			if (lvalue.type.is_int())
				after = builder.CreateSDiv(before, rvalue.llvm_value(builder));
			else if (lvalue.type.is_bool() || lvalue.type.is_uint())
				after = builder.CreateUDiv(before, rvalue.llvm_value(builder));
			else
				after = builder.CreateFDiv(before, rvalue.llvm_value(builder));
			break;
		case GlslToken::operator_modulus_assign:
			if (lvalue.type.is_int())
				after = builder.CreateSRem(before, rvalue.llvm_value(builder));
			else if (lvalue.type.is_bool() || lvalue.type.is_uint())
				after = builder.CreateURem(before, rvalue.llvm_value(builder));
			else
				after = builder.CreateFRem(before, rvalue.llvm_value(builder));
			break;
		case GlslToken::operator_shift_left_assign:
			after = builder.CreateShl(before, rvalue.llvm_value(builder));
			break;
		case GlslToken::operator_shift_right_assign:
			after = builder.CreateAShr(before, rvalue.llvm_value(builder));
			break;
		case GlslToken::operator_bit_and_assign:
			after = builder.CreateAnd(before, rvalue.llvm_value(builder));
			break;
		case GlslToken::operator_bit_xor_assign:
			after = builder.CreateXor(before, rvalue.llvm_value(builder));
			break;
		case GlslToken::operator_bit_or_assign:
			after = builder.CreateOr(before, rvalue.llvm_value(builder));
			break;
		default:
			throw Exception("Codegen: unknown assignment operator");
		}
		builder.CreateStore(after, lvalue.llvm_pointer(), false);
		operands.push_pointer(lvalue.llvm_pointer(), lvalue.type);
	}
}

void GlslExpressionCodeGen::expression(GlslAstPlusExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	scalar_to_vector(v1, v2);
	if (v1.type.is_bool() || v1.type.is_int() || v1.type.is_uint())
		operands.push_value(builder.CreateAdd(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else
		operands.push_value(builder.CreateFAdd(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstMinusExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	scalar_to_vector(v1, v2);
	if (v1.type.is_bool() || v1.type.is_int() || v1.type.is_uint())
		operands.push_value(builder.CreateSub(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else
		operands.push_value(builder.CreateFSub(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstMultiplyExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	scalar_to_vector(v1, v2);

	// To do: support other matrix types than mat4x4
	if (v1.type.is_matrix() && v2.type.is_matrix())
	{
		llvm::Type *type = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(program.context()), 4), 4);
		llvm::Value *v1_value = v1.llvm_value(builder);
		llvm::Value *v2_value = v2.llvm_value(builder);

		llvm::Value *m1_col[4] =
		{
			matrix_column(v1_value, 0),
			matrix_column(v1_value, 1),
			matrix_column(v1_value, 2),
			matrix_column(v1_value, 3)
		};

		llvm::Value *m2_row[4] =
		{
			matrix_column(v2_value, 0),
			matrix_column(v2_value, 1),
			matrix_column(v2_value, 2),
			matrix_column(v2_value, 3)
		};
		transpose4(m2_row[0], m2_row[1], m2_row[2], m2_row[3]);

		llvm::Value *result = llvm::UndefValue::get(type);
		for (int i = 0; i < 4; i++)
		{
			llvm::Value *tmp0 = builder.CreateFMul(m1_col[i], m2_row[0]);
			llvm::Value *tmp1 = builder.CreateFMul(m1_col[i], m2_row[1]);
			llvm::Value *tmp2 = builder.CreateFMul(m1_col[i], m2_row[2]);
			llvm::Value *tmp3 = builder.CreateFMul(m1_col[i], m2_row[3]);
			transpose4(tmp0, tmp1, tmp2, tmp3);
			llvm::Value *result_col = builder.CreateFAdd(builder.CreateFAdd(builder.CreateFAdd(tmp0, tmp1), tmp2), tmp3);
			builder.CreateInsertElement(result, result_col, llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)i)));
		}
		operands.push_value(result, v1.type);
	}
	else if (v1.type.is_matrix() && v2.type.is_vector())
	{
		llvm::Value *v1_value = v1.llvm_value(builder);
		llvm::Value *v2_value = v2.llvm_value(builder);
		llvm::Value *element0 = builder.CreateFMul(matrix_column(v1_value, 0), vector_shuffle(v2_value, 0, 0, 0, 0));
		llvm::Value *element1 = builder.CreateFMul(matrix_column(v1_value, 1), vector_shuffle(v2_value, 1, 1, 1, 1));
		llvm::Value *element2 = builder.CreateFMul(matrix_column(v1_value, 2), vector_shuffle(v2_value, 2, 2, 2, 2));
		llvm::Value *element3 = builder.CreateFMul(matrix_column(v1_value, 3), vector_shuffle(v2_value, 3, 3, 3, 3));
		llvm::Value *result = builder.CreateFAdd(builder.CreateFAdd(builder.CreateFAdd(element0, element1), element2), element3);
		operands.push_value(result, v2.type);
	}
	else if (v2.type.is_matrix() && v1.type.is_vector())
	{
		llvm::Value *v1_value = v1.llvm_value(builder);
		llvm::Value *v2_value = v2.llvm_value(builder);
		llvm::Value *element0 = builder.CreateFMul(matrix_column(v2_value, 0), v1_value);
		llvm::Value *element1 = builder.CreateFMul(matrix_column(v2_value, 1), v1_value);
		llvm::Value *element2 = builder.CreateFMul(matrix_column(v2_value, 2), v1_value);
		llvm::Value *element3 = builder.CreateFMul(matrix_column(v2_value, 3), v1_value);
		transpose4(element0, element1, element2, element3);
		llvm::Value *result = builder.CreateFAdd(builder.CreateFAdd(builder.CreateFAdd(element0, element1), element2), element3);
		operands.push_value(result, v1.type);
	}
	else
	{
		if (v1.type.is_bool() || v1.type.is_int() || v1.type.is_uint())
			operands.push_value(builder.CreateMul(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
		else
			operands.push_value(builder.CreateFMul(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	}
}

void GlslExpressionCodeGen::expression(GlslAstDivideExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	scalar_to_vector(v1, v2);
	if (v1.type.is_int())
		operands.push_value(builder.CreateSDiv(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else if (v1.type.is_bool() || v1.type.is_uint())
		operands.push_value(builder.CreateUDiv(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else
		operands.push_value(builder.CreateFDiv(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstModulusExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	scalar_to_vector(v1, v2);
	if (v1.type.is_int())
		operands.push_value(builder.CreateSRem(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else if (v1.type.is_bool() || v1.type.is_uint())
		operands.push_value(builder.CreateURem(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else
		operands.push_value(builder.CreateFRem(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstShiftLeftExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	operands.push_value(builder.CreateShl(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstShiftRightExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	operands.push_value(builder.CreateAShr(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstLessExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	if (v1.type.is_int())
		operands.push_value(builder.CreateICmpSLT(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else if (v1.type.is_bool() || v1.type.is_uint())
		operands.push_value(builder.CreateICmpULT(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else
		operands.push_value(builder.CreateFCmpULT(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstLessEqualExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	if (v1.type.is_int())
		operands.push_value(builder.CreateICmpSLE(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else if (v1.type.is_bool() || v1.type.is_uint())
		operands.push_value(builder.CreateICmpULE(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else
		operands.push_value(builder.CreateFCmpULE(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstGreaterExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	if (v1.type.is_int())
		operands.push_value(builder.CreateICmpSGT(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else if (v1.type.is_bool() || v1.type.is_uint())
		operands.push_value(builder.CreateICmpUGT(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else
		operands.push_value(builder.CreateFCmpUGT(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstGreaterEqualExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	if (v1.type.is_int())
		operands.push_value(builder.CreateICmpSGE(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else if (v1.type.is_bool() || v1.type.is_uint())
		operands.push_value(builder.CreateICmpUGE(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else
		operands.push_value(builder.CreateFCmpUGE(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstEqualExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	if (v1.type.is_bool() || v1.type.is_int() || v1.type.is_uint())
		operands.push_value(builder.CreateICmpEQ(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else
		operands.push_value(builder.CreateFCmpUEQ(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstNotEqualExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	if (v1.type.is_bool() || v1.type.is_int() || v1.type.is_uint())
		operands.push_value(builder.CreateICmpNE(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
	else
		operands.push_value(builder.CreateFCmpUNE(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstBitAndExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	scalar_to_vector(v1, v2);
	operands.push_value(builder.CreateAnd(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstBitXorExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	scalar_to_vector(v1, v2);
	operands.push_value(builder.CreateXor(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstBitOrExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	implicit_convert(v1, v2);
	scalar_to_vector(v1, v2);
	operands.push_value(builder.CreateOr(v1.llvm_value(builder), v2.llvm_value(builder)), v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstLogicalAndExpression *node)
{
	// To do: This is wrong.  According to the spec we are only allowed to evaluate the second operand if the first is true.
	// Logical expressions are also only allowed on booleans.
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	if (v1.type.is_bool() || v1.type.is_int() || v1.type.is_uint())
		operands.push_value(
			builder.CreateAnd(
				builder.CreateICmpNE(v1.llvm_value(builder), llvm::ConstantInt::getFalse(context())),
				builder.CreateICmpNE(v2.llvm_value(builder), llvm::ConstantInt::getFalse(context()))),
			v1.type);
	else
		operands.push_value(
			builder.CreateAnd(
				builder.CreateFCmpONE(v1.llvm_value(builder), llvm::ConstantInt::getFalse(context())),
				builder.CreateFCmpONE(v2.llvm_value(builder), llvm::ConstantInt::getFalse(context()))),
			v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstLogicalXorExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	if (v1.type.is_bool() || v1.type.is_int() || v1.type.is_uint())
		operands.push_value(
			builder.CreateXor(
				builder.CreateICmpNE(v1.llvm_value(builder), llvm::ConstantInt::getFalse(context())),
				builder.CreateICmpNE(v2.llvm_value(builder), llvm::ConstantInt::getFalse(context()))),
			v1.type);
	else
		operands.push_value(
			builder.CreateXor(
				builder.CreateFCmpONE(v1.llvm_value(builder), llvm::ConstantInt::getFalse(context())),
				builder.CreateFCmpONE(v2.llvm_value(builder), llvm::ConstantInt::getFalse(context()))),
			v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstLogicalOrExpression *node)
{
	node->operand1->codegen(this);
	node->operand2->codegen(this);
	GlslOperand v2 = operands.pop();
	GlslOperand v1 = operands.pop();
	if (v1.type.is_bool() || v1.type.is_int() || v1.type.is_uint())
		operands.push_value(
			builder.CreateOr(
				builder.CreateICmpNE(v1.llvm_value(builder), llvm::ConstantInt::getFalse(context())),
				builder.CreateICmpNE(v2.llvm_value(builder), llvm::ConstantInt::getFalse(context()))),
			v1.type);
	else
		operands.push_value(
			builder.CreateOr(
				builder.CreateFCmpONE(v1.llvm_value(builder), llvm::ConstantInt::getFalse(context())),
				builder.CreateFCmpONE(v2.llvm_value(builder), llvm::ConstantInt::getFalse(context()))),
			v1.type);
}

void GlslExpressionCodeGen::expression(GlslAstSelectExpression *node)
{
	throw Exception("Codegen for select operator not implemented");
}

void GlslExpressionCodeGen::expression(GlslAstSequenceExpression *node)
{
	// The left operand of the sequential-evaluation operator is evaluated as a void expression.
	// The result of the operation has the same value and type as the right operand.
	node->operand1->codegen(this);
	operands.pop();
	node->operand2->codegen(this);
}

llvm::LLVMContext &GlslExpressionCodeGen::context()
{
	return program.context();
}

void GlslExpressionCodeGen::construct_void(GlslAstConstructorCall *node)
{
	throw Exception("Constructor call not allowed on void type");
}

void GlslExpressionCodeGen::construct_float(GlslAstConstructorCall *node)
{
	if (node->parameters.size() != 1)
		throw Exception(string_format("no overloaded function takes %1 arguments", (int)node->parameters.size()));

	GlslOperand operand1 = operands.pop();
	llvm::Value *value = operand1.llvm_value(builder);
	if (operand1.type.columns() != 1)
		value = builder.CreateExtractElement(value, llvm::ConstantInt::get(llvm::Type::getInt32Ty(program.context()), llvm::APInt(32, (uint64_t)0)));

	if (operand1.type.is_double())
		value = builder.CreateTrunc(value, llvm::Type::getFloatTy(program.context()));
	else if (operand1.type.is_int())
		value = builder.CreateSIToFP(value, llvm::Type::getFloatTy(program.context()));
	else if (operand1.type.is_uint() || operand1.type.is_bool())
		value = builder.CreateUIToFP(value, llvm::Type::getFloatTy(program.context()));
	else if (!operand1.type.is_float())
		throw Exception("No overloaded function matches");

	operands.push_value(value, GlslToken::keyword_float);
}

void GlslExpressionCodeGen::construct_double(GlslAstConstructorCall *node)
{
	if (node->parameters.size() != 1)
		throw Exception(string_format("no overloaded function takes %1 arguments", (int)node->parameters.size()));

	GlslOperand operand1 = operands.pop();
	llvm::Value *value = operand1.llvm_value(builder);
	if (operand1.type.columns() != 1)
		value = builder.CreateExtractElement(value, llvm::ConstantInt::get(llvm::Type::getInt32Ty(program.context()), llvm::APInt(32, (uint64_t)0)));

	if (operand1.type.is_float())
		value = builder.CreateZExt(value, llvm::Type::getDoubleTy(program.context()));
	else if (operand1.type.is_int())
		value = builder.CreateSIToFP(value, llvm::Type::getDoubleTy(program.context()));
	else if (operand1.type.is_uint() || operand1.type.is_bool())
		value = builder.CreateUIToFP(value, llvm::Type::getDoubleTy(program.context()));
	else if (!operand1.type.is_double())
		throw Exception("No overloaded function matches");

	operands.push_value(value, GlslToken::keyword_double);
}

void GlslExpressionCodeGen::construct_int(GlslAstConstructorCall *node)
{
	if (node->parameters.size() != 1)
		throw Exception(string_format("no overloaded function takes %1 arguments", (int)node->parameters.size()));

	GlslOperand operand1 = operands.pop();
	llvm::Value *value = operand1.llvm_value(builder);
	if (operand1.type.columns() != 1)
		value = builder.CreateExtractElement(value, llvm::ConstantInt::get(llvm::Type::getInt32Ty(program.context()), llvm::APInt(32, (uint64_t)0)));

	if (operand1.type.is_float() || operand1.type.is_double())
		value = builder.CreateFPToSI(value, llvm::Type::getInt32Ty(program.context()));
	else if (!operand1.type.is_int() && !operand1.type.is_uint())
		throw Exception("No overloaded function matches");

	operands.push_value(value, GlslToken::keyword_int);
}

void GlslExpressionCodeGen::construct_uint(GlslAstConstructorCall *node)
{
	if (node->parameters.size() != 1)
		throw Exception(string_format("no overloaded function takes %1 arguments", (int)node->parameters.size()));

	GlslOperand operand1 = operands.pop();
	llvm::Value *value = operand1.llvm_value(builder);
	if (operand1.type.columns() != 1)
		value = builder.CreateExtractElement(value, llvm::ConstantInt::get(llvm::Type::getInt32Ty(program.context()), llvm::APInt(32, (uint64_t)0)));

	if (operand1.type.is_float() || operand1.type.is_double())
		value = builder.CreateFPToUI(value, llvm::Type::getInt32Ty(program.context()));
	else if (!operand1.type.is_int() && !operand1.type.is_uint())
		throw Exception("No overloaded function matches");

	operands.push_value(value, GlslToken::keyword_uint);
}

void GlslExpressionCodeGen::construct_bool(GlslAstConstructorCall *node)
{
	if (node->parameters.size() != 1)
		throw Exception(string_format("no overloaded function takes %1 arguments", (int)node->parameters.size()));

	GlslOperand operand1 = operands.pop();
	llvm::Value *value = operand1.llvm_value(builder);
	if (operand1.type.columns() != 1)
		value = builder.CreateExtractElement(value, llvm::ConstantInt::get(llvm::Type::getInt32Ty(program.context()), llvm::APInt(32, (uint64_t)0)));

	if (operand1.type.is_float() || operand1.type.is_double())
		value = builder.CreateFPToUI(value, llvm::Type::getInt1Ty(program.context()));
	else if (operand1.type.is_int() || operand1.type.is_uint())
		value = builder.CreateTrunc(value, llvm::Type::getInt1Ty(program.context()));
	else if (!operand1.type.is_bool())
		throw Exception("No overloaded function matches");

	operands.push_value(value, GlslToken::keyword_bool);
}

void GlslExpressionCodeGen::construct_vec2(GlslAstConstructorCall *node)
{
	if (node->parameters.size() != 2)
		throw Exception(string_format("no overloaded function takes %1 arguments", (int)node->parameters.size()));
	node->parameters[0]->codegen(this);
	node->parameters[1]->codegen(this);
	llvm::Type *type = llvm::VectorType::get(llvm::Type::getFloatTy(context()), 2);
	GlslOperand operand2 = operands.pop();
	GlslOperand operand1 = operands.pop();
	llvm::Value *v = builder.CreateInsertElement(llvm::UndefValue::get(type), operand1.llvm_value(builder), llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)0)));
	v = builder.CreateInsertElement(v, operand2.llvm_value(builder), llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)1)));
	operands.push_value(v, GlslToken::keyword_vec2);
}

void GlslExpressionCodeGen::construct_vec3(GlslAstConstructorCall *node)
{
	if (node->parameters.size() != 3)
		throw Exception(string_format("no overloaded function takes %1 arguments", (int)node->parameters.size()));
	node->parameters[0]->codegen(this);
	node->parameters[1]->codegen(this);
	node->parameters[2]->codegen(this);
	llvm::Type *type = llvm::VectorType::get(llvm::Type::getFloatTy(context()), 3);
	GlslOperand operand3 = operands.pop();
	GlslOperand operand2 = operands.pop();
	GlslOperand operand1 = operands.pop();
	llvm::Value *v = builder.CreateInsertElement(llvm::UndefValue::get(type), operand1.llvm_value(builder), llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)0)));
	v = builder.CreateInsertElement(v, operand2.llvm_value(builder), llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)1)));
	v = builder.CreateInsertElement(v, operand3.llvm_value(builder), llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)2)));
	operands.push_value(v, GlslToken::keyword_vec3);
}

void GlslExpressionCodeGen::construct_vec4(GlslAstConstructorCall *node)
{
	if (node->parameters.size() != 4)
		throw Exception(string_format("no overloaded function takes %1 arguments", (int)node->parameters.size()));
	node->parameters[0]->codegen(this);
	node->parameters[1]->codegen(this);
	node->parameters[2]->codegen(this);
	node->parameters[3]->codegen(this);
	llvm::Type *type = llvm::VectorType::get(llvm::Type::getFloatTy(context()), 4);
	GlslOperand operand4 = operands.pop();
	GlslOperand operand3 = operands.pop();
	GlslOperand operand2 = operands.pop();
	GlslOperand operand1 = operands.pop();
	llvm::Value *v = builder.CreateInsertElement(llvm::UndefValue::get(type), operand1.llvm_value(builder), llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)0)));
	v = builder.CreateInsertElement(v, operand2.llvm_value(builder), llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)1)));
	v = builder.CreateInsertElement(v, operand3.llvm_value(builder), llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)2)));
	v = builder.CreateInsertElement(v, operand4.llvm_value(builder), llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)3)));
	operands.push_value(v, GlslToken::keyword_vec4);
}

void GlslExpressionCodeGen::construct_dvec2(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dvec3(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dvec4(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_ivec2(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_ivec3(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_ivec4(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_uvec2(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_uvec3(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_uvec4(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_bvec2(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_bvec3(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_bvec4(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_opaque(GlslAstConstructorCall *node)
{
	// To do: check in spec if this is allowed or not
	throw Exception("Constructor call not allowed on opaque types");
}

void GlslExpressionCodeGen::construct_mat2(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_mat2x3(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_mat2x4(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_mat3(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_mat3x2(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_mat3x4(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_mat4(GlslAstConstructorCall *node)
{
	if (node->parameters.size() != 16)
		throw Exception(string_format("no overloaded function takes %1 arguments", (int)node->parameters.size()));
	std::vector<GlslOperand> elements;
	for (int i = 0; i < 16; i++)
	{
		node->parameters[i]->codegen(this);
		elements.push_back(operands.pop());
	}

	llvm::Type *type = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(program.context()), 4), 4);
	llvm::Value *m = llvm::UndefValue::get(type);
	for (int col = 0; col < 4; col++)
	{
		llvm::Type *col_type = llvm::VectorType::get(llvm::Type::getFloatTy(context()), 4);
		llvm::Value *v = llvm::UndefValue::get(col_type);
		for (int row = 0; row < 4; row++)
			v = builder.CreateInsertElement(v, elements[col * 4 + row].llvm_value(builder), llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)row)));
		std::vector<unsigned int> indexes;
		indexes.push_back(col);
		m = builder.CreateInsertValue(m, v, indexes);
	}
	operands.push_value(m, GlslToken::keyword_mat4);
}

void GlslExpressionCodeGen::construct_mat4x2(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_mat4x3(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dmat2(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dmat2x3(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dmat2x4(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dmat3(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dmat3x2(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dmat3x4(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dmat4(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dmat4x2(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::construct_dmat4x3(GlslAstConstructorCall *node)
{
	throw Exception("Type support not implemented");
}

void GlslExpressionCodeGen::implicit_convert(GlslOperand &operand1, GlslOperand &operand2)
{
	if (operand1.type.is_double() || operand2.type.is_double())
	{
		if (!operand1.type.is_double())
			operand1 = implicit_to_double(operand1);
		else if (!operand2.type.is_double())
			operand2 = implicit_to_double(operand2);
	}
	else if (operand1.type.is_float() || operand2.type.is_float())
	{
		if (!operand1.type.is_float())
			operand1 = implicit_to_float(operand1);
		else if (!operand2.type.is_float())
			operand2 = implicit_to_float(operand2);
	}
	else if (operand1.type.is_int() || operand2.type.is_uint())
	{
		if (!operand1.type.is_uint())
			operand1 = implicit_to_uint(operand1);
		else if (!operand2.type.is_uint())
			operand2 = implicit_to_uint(operand2);
	}
}

GlslOperand GlslExpressionCodeGen::implicit_to_double(GlslOperand operand)
{
	if (operand.type.is_float())
	{
		llvm::Value *value = operand.llvm_value(builder);
		value = builder.CreateZExt(value, llvm::Type::getDoubleTy(program.context()));
		return GlslOperand(value, false, operand.type.to_double());
	}
	else if (operand.type.is_int())
	{
		llvm::Value *value = operand.llvm_value(builder);
		value = builder.CreateSIToFP(value, llvm::Type::getDoubleTy(program.context()));
		return GlslOperand(value, false, operand.type.to_double());
	}
	else if (operand.type.is_uint())
	{
		llvm::Value *value = operand.llvm_value(builder);
		value = builder.CreateUIToFP(value, llvm::Type::getDoubleTy(program.context()));
		return GlslOperand(value, false, operand.type.to_uint());
	}
	else if (operand.type.is_double())
	{
		return operand;
	}
	else
	{
		throw Exception("Cannot implicitly convert type to double");
	}
}

GlslOperand GlslExpressionCodeGen::implicit_to_float(GlslOperand operand)
{
	if (operand.type.is_double())
	{
		llvm::Value *value = operand.llvm_value(builder);
		value = builder.CreateTrunc(value, llvm::Type::getFloatTy(program.context()));
		return GlslOperand(value, false, operand.type.to_float());
	}
	else if (operand.type.is_int())
	{
		llvm::Value *value = operand.llvm_value(builder);
		value = builder.CreateSIToFP(value, llvm::Type::getFloatTy(program.context()));
		return GlslOperand(value, false, operand.type.to_float());
	}
	else if (operand.type.is_uint())
	{
		llvm::Value *value = operand.llvm_value(builder);
		value = builder.CreateUIToFP(value, llvm::Type::getFloatTy(program.context()));
		return GlslOperand(value, false, operand.type.to_float());
	}
	else if (operand.type.is_float())
	{
		return operand;
	}
	else
	{
		throw Exception("Cannot implicitly convert type to float");
	}
}

GlslOperand GlslExpressionCodeGen::implicit_to_uint(GlslOperand operand)
{
	if (operand.type.is_double() || operand.type.is_float())
	{
		llvm::Value *value = operand.llvm_value(builder);
		value = builder.CreateFPToUI(value, llvm::Type::getInt32Ty(program.context()));
		return GlslOperand(value, false, operand.type.to_uint());
	}
	else if (operand.type.is_int())
	{
		llvm::Value *value = operand.llvm_value(builder);
		return GlslOperand(value, false, operand.type.to_uint());
	}
	else if (operand.type.is_uint())
	{
		return operand;
	}
	else
	{
		throw Exception("Cannot implicitly convert type to uint");
	}
}

void GlslExpressionCodeGen::scalar_to_vector(GlslOperand &v1, GlslOperand &v2)
{
	if (v1.type.is_scalar() && v2.type.is_vector()) // scalar * vector
	{
		if (v1.type.is_double())
		{
			int columns = v2.type.columns();
			llvm::Type *type = llvm::VectorType::get(llvm::Type::getDoubleTy(program.context()), columns);
			llvm::Value *scalar = v1.llvm_value(builder);
			llvm::Value *v = llvm::UndefValue::get(type);
			for (int i = 0; i < columns; i++)
				v = builder.CreateInsertElement(v, scalar, llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)i)));
			v1 = GlslOperand(v, false, v2.type);
		}
		else if (v1.type.is_float())
		{
			int columns = v2.type.columns();
			llvm::Type *type = llvm::VectorType::get(llvm::Type::getFloatTy(program.context()), columns);
			llvm::Value *scalar = v1.llvm_value(builder);
			llvm::Value *v = llvm::UndefValue::get(type);
			for (int i = 0; i < columns; i++)
				v = builder.CreateInsertElement(v, scalar, llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)i)));
			v1 = GlslOperand(v, false, v2.type);
		}
		else if (v1.type.is_int() || v1.type.is_uint())
		{
			int columns = v2.type.columns();
			llvm::Type *type = llvm::VectorType::get(llvm::Type::getInt32Ty(program.context()), columns);
			llvm::Value *scalar = v1.llvm_value(builder);
			llvm::Value *v = llvm::UndefValue::get(type);
			for (int i = 0; i < columns; i++)
				v = builder.CreateInsertElement(v, scalar, llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)i)));
			v1 = GlslOperand(v, false, v2.type);
		}
		else if (v1.type.is_bool())
		{
			int columns = v2.type.columns();
			llvm::Type *type = llvm::VectorType::get(llvm::Type::getInt1Ty(program.context()), columns);
			llvm::Value *scalar = v1.llvm_value(builder);
			llvm::Value *v = llvm::UndefValue::get(type);
			for (int i = 0; i < columns; i++)
				v = builder.CreateInsertElement(v, scalar, llvm::ConstantInt::get(context(), llvm::APInt(32, (uint64_t)i)));
			v1 = GlslOperand(v, false, v2.type);
		}
	}
	else if (v1.type.is_vector() && v2.type.is_scalar())
	{
		scalar_to_vector(v2, v1);
	}
}

llvm::Value *GlslExpressionCodeGen::matrix_column(llvm::Value *matrix, int index)
{
	std::vector<unsigned int> indexes;
	indexes.push_back(index);
	return builder.CreateExtractValue(matrix, indexes);
}

llvm::Value *GlslExpressionCodeGen::vector_shuffle(llvm::Value *vector, int index0, int index1, int index2, int index3)
{
	std::vector<llvm::Constant*> constants;
	constants.push_back(llvm::ConstantInt::get(program.context(), llvm::APInt(32, index0)));
	constants.push_back(llvm::ConstantInt::get(program.context(), llvm::APInt(32, index1)));
	constants.push_back(llvm::ConstantInt::get(program.context(), llvm::APInt(32, index2)));
	constants.push_back(llvm::ConstantInt::get(program.context(), llvm::APInt(32, index3)));
	llvm::Value *mask = llvm::ConstantVector::get(constants);
	return builder.CreateShuffleVector(vector, vector, mask);
}

void GlslExpressionCodeGen::transpose4(llvm::Value *&element0, llvm::Value *&element1, llvm::Value *&element2, llvm::Value *&element3)
{
	SSAVec4f v0 = SSAVec4f::from_llvm(element0);
	SSAVec4f v1 = SSAVec4f::from_llvm(element1);
	SSAVec4f v2 = SSAVec4f::from_llvm(element2);
	SSAVec4f v3 = SSAVec4f::from_llvm(element3);
	SSAVec4f::transpose(v0, v1, v2, v3);
	element0 = v0.v;
	element1 = v1.v;
	element2 = v2.v;
	element3 = v3.v;
}
