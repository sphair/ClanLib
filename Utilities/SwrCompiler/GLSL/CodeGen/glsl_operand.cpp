
#include "precomp.h"
#include "glsl_operand.h"
#include "GLSL/AST/glsl_ast.h"
#include "GLSL/FixedFunction/glsl_fixed_function.h"

using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// GlslOperandType

GlslOperandType::GlslOperandType(GlslToken::Keyword type)
: primary_type(type)
{
}

bool GlslOperandType::is_void() const
{
	return primary_type == GlslToken::keyword_void;
}

bool GlslOperandType::is_opaque() const
{
	switch (primary_type)
	{
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
		return true;
	default:
		return false;
	}
}

bool GlslOperandType::is_float() const
{
	switch (primary_type)
	{
	case GlslToken::keyword_float:
	case GlslToken::keyword_vec2:
	case GlslToken::keyword_vec3:
	case GlslToken::keyword_vec4:
	case GlslToken::keyword_mat2:
	case GlslToken::keyword_mat2x2:
	case GlslToken::keyword_mat2x3:
	case GlslToken::keyword_mat2x4:
	case GlslToken::keyword_mat3x2:
	case GlslToken::keyword_mat3:
	case GlslToken::keyword_mat3x3:
	case GlslToken::keyword_mat3x4:
	case GlslToken::keyword_mat4x2:
	case GlslToken::keyword_mat4x3:
	case GlslToken::keyword_mat4:
	case GlslToken::keyword_mat4x4:
		return true;
	default:
		return false;
	}
}

bool GlslOperandType::is_double() const
{
	switch (primary_type)
	{
	case GlslToken::keyword_double:
	case GlslToken::keyword_dvec2:
	case GlslToken::keyword_dvec3:
	case GlslToken::keyword_dvec4:
	case GlslToken::keyword_dmat2:
	case GlslToken::keyword_dmat2x2:
	case GlslToken::keyword_dmat2x3:
	case GlslToken::keyword_dmat2x4:
	case GlslToken::keyword_dmat3x2:
	case GlslToken::keyword_dmat3:
	case GlslToken::keyword_dmat3x3:
	case GlslToken::keyword_dmat3x4:
	case GlslToken::keyword_dmat4x2:
	case GlslToken::keyword_dmat4x3:
	case GlslToken::keyword_dmat4:
	case GlslToken::keyword_dmat4x4:
		return true;
	default:
		return false;
	}
}

bool GlslOperandType::is_int() const
{
	switch (primary_type)
	{
	case GlslToken::keyword_int:
	case GlslToken::keyword_ivec2:
	case GlslToken::keyword_ivec3:
	case GlslToken::keyword_ivec4:
		return true;
	default:
		return false;
	}
}

bool GlslOperandType::is_uint() const
{
	switch (primary_type)
	{
	case GlslToken::keyword_uint:
	case GlslToken::keyword_uvec2:
	case GlslToken::keyword_uvec3:
	case GlslToken::keyword_uvec4:
		return true;
	default:
		return false;
	}
}

bool GlslOperandType::is_bool() const
{
	switch (primary_type)
	{
	case GlslToken::keyword_bool:
	case GlslToken::keyword_bvec2:
	case GlslToken::keyword_bvec3:
	case GlslToken::keyword_bvec4:
		return true;
	default:
		return false;
	}
}

int GlslOperandType::columns() const
{
	switch (primary_type)
	{
	default:
		return 1;
	case GlslToken::keyword_vec2:
	case GlslToken::keyword_dvec2:
	case GlslToken::keyword_bvec2:
	case GlslToken::keyword_ivec2:
	case GlslToken::keyword_uvec2:
	case GlslToken::keyword_mat2:
	case GlslToken::keyword_mat2x2:
	case GlslToken::keyword_mat2x3:
	case GlslToken::keyword_mat2x4:
	case GlslToken::keyword_dmat2:
	case GlslToken::keyword_dmat2x2:
	case GlslToken::keyword_dmat2x3:
	case GlslToken::keyword_dmat2x4:
		return 2;
	case GlslToken::keyword_vec3:
	case GlslToken::keyword_dvec3:
	case GlslToken::keyword_ivec3:
	case GlslToken::keyword_bvec3:
	case GlslToken::keyword_uvec3:
	case GlslToken::keyword_mat3x2:
	case GlslToken::keyword_mat3:
	case GlslToken::keyword_mat3x3:
	case GlslToken::keyword_mat3x4:
	case GlslToken::keyword_dmat3x2:
	case GlslToken::keyword_dmat3:
	case GlslToken::keyword_dmat3x3:
	case GlslToken::keyword_dmat3x4:
		return 3;
	case GlslToken::keyword_vec4:
	case GlslToken::keyword_dvec4:
	case GlslToken::keyword_bvec4:
	case GlslToken::keyword_ivec4:
	case GlslToken::keyword_uvec4:
	case GlslToken::keyword_mat4x2:
	case GlslToken::keyword_mat4x3:
	case GlslToken::keyword_mat4:
	case GlslToken::keyword_mat4x4:
	case GlslToken::keyword_dmat4x2:
	case GlslToken::keyword_dmat4x3:
	case GlslToken::keyword_dmat4:
	case GlslToken::keyword_dmat4x4:
		return 4;
	}
}

int GlslOperandType::rows() const
{
	switch (primary_type)
	{
	default:
		return 1;
	case GlslToken::keyword_mat2:
	case GlslToken::keyword_mat2x2:
	case GlslToken::keyword_mat3x2:
	case GlslToken::keyword_mat4x2:
	case GlslToken::keyword_dmat2:
	case GlslToken::keyword_dmat2x2:
	case GlslToken::keyword_dmat3x2:
	case GlslToken::keyword_dmat4x2:
		return 2;
	case GlslToken::keyword_mat2x3:
	case GlslToken::keyword_mat3:
	case GlslToken::keyword_mat3x3:
	case GlslToken::keyword_mat4x3:
	case GlslToken::keyword_dmat2x3:
	case GlslToken::keyword_dmat3:
	case GlslToken::keyword_dmat3x3:
	case GlslToken::keyword_dmat4x3:
		return 3;
	case GlslToken::keyword_mat2x4:
	case GlslToken::keyword_mat3x4:
	case GlslToken::keyword_mat4:
	case GlslToken::keyword_mat4x4:
	case GlslToken::keyword_dmat2x4:
	case GlslToken::keyword_dmat3x4:
	case GlslToken::keyword_dmat4:
	case GlslToken::keyword_dmat4x4:
		return 4;
	}
}

GlslOperandType GlslOperandType::to_double() const
{
	switch (primary_type)
	{
	case GlslToken::keyword_int:
	case GlslToken::keyword_uint:
	case GlslToken::keyword_bool:
	case GlslToken::keyword_float:
	case GlslToken::keyword_double:
		return GlslToken::keyword_double;
	case GlslToken::keyword_vec2:
	case GlslToken::keyword_ivec2:
	case GlslToken::keyword_uvec2:
	case GlslToken::keyword_bvec2:
	case GlslToken::keyword_dvec2:
		return GlslToken::keyword_dvec2;
	case GlslToken::keyword_vec3:
	case GlslToken::keyword_ivec3:
	case GlslToken::keyword_uvec3:
	case GlslToken::keyword_bvec3:
	case GlslToken::keyword_dvec3:
		return GlslToken::keyword_dvec3;
	case GlslToken::keyword_vec4:
	case GlslToken::keyword_ivec4:
	case GlslToken::keyword_uvec4:
	case GlslToken::keyword_bvec4:
	case GlslToken::keyword_dvec4:
		return GlslToken::keyword_dvec4;
	case GlslToken::keyword_mat2:
	case GlslToken::keyword_dmat2:
		return GlslToken::keyword_dmat2;
	case GlslToken::keyword_mat2x2:
	case GlslToken::keyword_dmat2x2:
		return GlslToken::keyword_dmat2x2;
	case GlslToken::keyword_mat2x3:
	case GlslToken::keyword_dmat2x3:
		return GlslToken::keyword_dmat2x3;
	case GlslToken::keyword_mat2x4:
	case GlslToken::keyword_dmat2x4:
		return GlslToken::keyword_dmat2x4;
	case GlslToken::keyword_mat3x2:
	case GlslToken::keyword_dmat3x2:
		return GlslToken::keyword_dmat3x2;
	case GlslToken::keyword_mat3:
	case GlslToken::keyword_dmat3:
		return GlslToken::keyword_dmat3;
	case GlslToken::keyword_mat3x3:
	case GlslToken::keyword_dmat3x3:
		return GlslToken::keyword_dmat3x3;
	case GlslToken::keyword_mat3x4:
	case GlslToken::keyword_dmat3x4:
		return GlslToken::keyword_dmat3x4;
	case GlslToken::keyword_mat4x2:
	case GlslToken::keyword_dmat4x2:
		return GlslToken::keyword_dmat4x2;
	case GlslToken::keyword_mat4x3:
	case GlslToken::keyword_dmat4x3:
		return GlslToken::keyword_dmat4x3;
	case GlslToken::keyword_mat4:
	case GlslToken::keyword_dmat4:
		return GlslToken::keyword_dmat4;
	case GlslToken::keyword_mat4x4:
	case GlslToken::keyword_dmat4x4:
		return GlslToken::keyword_dmat4x4;
	default:
		throw Exception("Type cannot be converted to double");
	}
}

GlslOperandType GlslOperandType::to_float() const
{
	switch (primary_type)
	{
	case GlslToken::keyword_int:
	case GlslToken::keyword_uint:
	case GlslToken::keyword_bool:
	case GlslToken::keyword_float:
	case GlslToken::keyword_double:
		return GlslToken::keyword_float;
	case GlslToken::keyword_vec2:
	case GlslToken::keyword_ivec2:
	case GlslToken::keyword_uvec2:
	case GlslToken::keyword_bvec2:
	case GlslToken::keyword_dvec2:
		return GlslToken::keyword_vec2;
	case GlslToken::keyword_vec3:
	case GlslToken::keyword_ivec3:
	case GlslToken::keyword_uvec3:
	case GlslToken::keyword_bvec3:
	case GlslToken::keyword_dvec3:
		return GlslToken::keyword_vec3;
	case GlslToken::keyword_vec4:
	case GlslToken::keyword_ivec4:
	case GlslToken::keyword_uvec4:
	case GlslToken::keyword_bvec4:
	case GlslToken::keyword_dvec4:
		return GlslToken::keyword_vec4;
	case GlslToken::keyword_mat2:
	case GlslToken::keyword_dmat2:
		return GlslToken::keyword_mat2;
	case GlslToken::keyword_mat2x2:
	case GlslToken::keyword_dmat2x2:
		return GlslToken::keyword_mat2x2;
	case GlslToken::keyword_mat2x3:
	case GlslToken::keyword_dmat2x3:
		return GlslToken::keyword_mat2x3;
	case GlslToken::keyword_mat2x4:
	case GlslToken::keyword_dmat2x4:
		return GlslToken::keyword_mat2x4;
	case GlslToken::keyword_mat3x2:
	case GlslToken::keyword_dmat3x2:
		return GlslToken::keyword_mat3x2;
	case GlslToken::keyword_mat3:
	case GlslToken::keyword_dmat3:
		return GlslToken::keyword_mat3;
	case GlslToken::keyword_mat3x3:
	case GlslToken::keyword_dmat3x3:
		return GlslToken::keyword_mat3x3;
	case GlslToken::keyword_mat3x4:
	case GlslToken::keyword_dmat3x4:
		return GlslToken::keyword_mat3x4;
	case GlslToken::keyword_mat4x2:
	case GlslToken::keyword_dmat4x2:
		return GlslToken::keyword_mat4x2;
	case GlslToken::keyword_mat4x3:
	case GlslToken::keyword_dmat4x3:
		return GlslToken::keyword_mat4x3;
	case GlslToken::keyword_mat4:
	case GlslToken::keyword_dmat4:
		return GlslToken::keyword_mat4;
	case GlslToken::keyword_mat4x4:
	case GlslToken::keyword_dmat4x4:
		return GlslToken::keyword_mat4x4;
	default:
		throw Exception("Type cannot be converted to float");
	}
}

GlslOperandType GlslOperandType::to_int() const
{
	switch (primary_type)
	{
	case GlslToken::keyword_int:
	case GlslToken::keyword_uint:
	case GlslToken::keyword_bool:
	case GlslToken::keyword_float:
	case GlslToken::keyword_double:
		return GlslToken::keyword_int;
	case GlslToken::keyword_vec2:
	case GlslToken::keyword_ivec2:
	case GlslToken::keyword_uvec2:
	case GlslToken::keyword_bvec2:
	case GlslToken::keyword_dvec2:
		return GlslToken::keyword_ivec2;
	case GlslToken::keyword_vec3:
	case GlslToken::keyword_ivec3:
	case GlslToken::keyword_uvec3:
	case GlslToken::keyword_bvec3:
	case GlslToken::keyword_dvec3:
		return GlslToken::keyword_ivec3;
	case GlslToken::keyword_vec4:
	case GlslToken::keyword_ivec4:
	case GlslToken::keyword_uvec4:
	case GlslToken::keyword_bvec4:
	case GlslToken::keyword_dvec4:
		return GlslToken::keyword_ivec4;
	default:
		throw Exception("Type cannot be converted to int");
	}
}

GlslOperandType GlslOperandType::to_uint() const
{
	switch (primary_type)
	{
	case GlslToken::keyword_int:
	case GlslToken::keyword_uint:
	case GlslToken::keyword_bool:
	case GlslToken::keyword_float:
	case GlslToken::keyword_double:
		return GlslToken::keyword_uint;
	case GlslToken::keyword_vec2:
	case GlslToken::keyword_ivec2:
	case GlslToken::keyword_uvec2:
	case GlslToken::keyword_bvec2:
	case GlslToken::keyword_dvec2:
		return GlslToken::keyword_uvec2;
	case GlslToken::keyword_vec3:
	case GlslToken::keyword_ivec3:
	case GlslToken::keyword_uvec3:
	case GlslToken::keyword_bvec3:
	case GlslToken::keyword_dvec3:
		return GlslToken::keyword_uvec3;
	case GlslToken::keyword_vec4:
	case GlslToken::keyword_ivec4:
	case GlslToken::keyword_uvec4:
	case GlslToken::keyword_bvec4:
	case GlslToken::keyword_dvec4:
		return GlslToken::keyword_uvec4;
	default:
		throw Exception("Type cannot be converted to uint");
	}
}

GlslOperandType GlslOperandType::to_bool() const
{
	switch (primary_type)
	{
	case GlslToken::keyword_int:
	case GlslToken::keyword_uint:
	case GlslToken::keyword_bool:
	case GlslToken::keyword_float:
	case GlslToken::keyword_double:
		return GlslToken::keyword_bool;
	case GlslToken::keyword_vec2:
	case GlslToken::keyword_ivec2:
	case GlslToken::keyword_uvec2:
	case GlslToken::keyword_bvec2:
	case GlslToken::keyword_dvec2:
		return GlslToken::keyword_bvec2;
	case GlslToken::keyword_vec3:
	case GlslToken::keyword_ivec3:
	case GlslToken::keyword_uvec3:
	case GlslToken::keyword_bvec3:
	case GlslToken::keyword_dvec3:
		return GlslToken::keyword_bvec3;
	case GlslToken::keyword_vec4:
	case GlslToken::keyword_ivec4:
	case GlslToken::keyword_uvec4:
	case GlslToken::keyword_bvec4:
	case GlslToken::keyword_dvec4:
		return GlslToken::keyword_bvec4;
	default:
		throw Exception("Type cannot be converted to bool");
	}
}

/////////////////////////////////////////////////////////////////////////////
// GlslOperand

GlslOperand::GlslOperand()
: value(0), is_pointer(false), type(GlslToken::keyword_void)
{
}

GlslOperand::GlslOperand(llvm::Value *value, bool is_pointer, GlslOperandType type)
: value(value), is_pointer(is_pointer), type(type)
{
}

llvm::Value *GlslOperand::llvm_value(llvm::IRBuilder<> &builder)
{
	if (is_pointer)
		return builder.CreateLoad(value, false);
	else
		return value;
}

llvm::Value *GlslOperand::llvm_pointer()
{
	if (!is_pointer)
		throw Exception("GlslCodeGen operand is not a pointer");
	else
		return value;
}

/////////////////////////////////////////////////////////////////////////////
// GlslOperandStack

GlslOperandStack::GlslOperandStack(llvm::IRBuilder<> &builder)
: builder(builder)
{
}

void GlslOperandStack::clear()
{
	operand_stack.clear();
}

GlslOperand GlslOperandStack::pop()
{
	GlslOperand operand = operand_stack.back();
	operand_stack.pop_back();
	return operand;
}

void GlslOperandStack::push_value(llvm::Value *value, GlslOperandType type)
{
	operand_stack.push_back(GlslOperand(value, false, type));
}

void GlslOperandStack::push_pointer(llvm::Value *ptr, GlslOperandType type)
{
	operand_stack.push_back(GlslOperand(ptr, true, type));
}

GlslOperandType GlslOperandStack::get_operand_type(GlslAstType *type)
{
	GlslAstBuiltInType *built_in = dynamic_cast<GlslAstBuiltInType*>(type);
	GlslAstStructType *glsl_struct = dynamic_cast<GlslAstStructType*>(type);
	if (built_in)
	{
		return built_in->type;
	}
	else if (glsl_struct)
	{
		return GlslToken::keyword_struct;
	}
	else
	{
		throw Exception("Unknown type");
	}
}

llvm::Type *GlslOperandStack::get_llvm_type(GlslAstType *type, llvm::LLVMContext &context)
{
	GlslAstBuiltInType *built_in = dynamic_cast<GlslAstBuiltInType*>(type);
	GlslAstStructType *glsl_struct = dynamic_cast<GlslAstStructType*>(type);
	if (built_in)
	{
		switch (built_in->type)
		{
		case GlslToken::keyword_void:
			return llvm::Type::getVoidTy(context);
		case GlslToken::keyword_float:
			return llvm::Type::getFloatTy(context);
		case GlslToken::keyword_double:
			return llvm::Type::getDoubleTy(context);
		case GlslToken::keyword_int:
		case GlslToken::keyword_uint:
			return llvm::Type::getInt32Ty(context);
		case GlslToken::keyword_bool:
			return llvm::Type::getInt1Ty(context);
		case GlslToken::keyword_vec2:
			return llvm::VectorType::get(llvm::Type::getFloatTy(context), 2);
		case GlslToken::keyword_vec3:
			return llvm::VectorType::get(llvm::Type::getFloatTy(context), 3);
		case GlslToken::keyword_vec4:
			return llvm::VectorType::get(llvm::Type::getFloatTy(context), 4);
		case GlslToken::keyword_dvec2:
			return llvm::VectorType::get(llvm::Type::getDoubleTy(context), 2);
		case GlslToken::keyword_dvec3:
			return llvm::VectorType::get(llvm::Type::getDoubleTy(context), 3);
		case GlslToken::keyword_dvec4:
			return llvm::VectorType::get(llvm::Type::getDoubleTy(context), 4);
		case GlslToken::keyword_bvec2:
			return llvm::VectorType::get(llvm::Type::getInt1Ty(context), 2);
		case GlslToken::keyword_bvec3:
			return llvm::VectorType::get(llvm::Type::getInt1Ty(context), 3);
		case GlslToken::keyword_bvec4:
			return llvm::VectorType::get(llvm::Type::getInt1Ty(context), 4);
		case GlslToken::keyword_ivec2:
			return llvm::VectorType::get(llvm::Type::getInt32Ty(context), 2);
		case GlslToken::keyword_ivec3:
			return llvm::VectorType::get(llvm::Type::getInt32Ty(context), 3);
		case GlslToken::keyword_ivec4:
			return llvm::VectorType::get(llvm::Type::getInt32Ty(context), 4);
		case GlslToken::keyword_uvec2:
			return llvm::VectorType::get(llvm::Type::getInt32Ty(context), 2);
		case GlslToken::keyword_uvec3:
			return llvm::VectorType::get(llvm::Type::getInt32Ty(context), 3);
		case GlslToken::keyword_uvec4:
			return llvm::VectorType::get(llvm::Type::getInt32Ty(context), 4);
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
			return GlslFixedFunction::get_sampler_struct(context)->getPointerTo();
		case GlslToken::keyword_mat2:
		case GlslToken::keyword_mat2x2:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(context), 2), 2);
		case GlslToken::keyword_mat2x3:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(context), 2), 3);
		case GlslToken::keyword_mat2x4:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(context), 2), 4);
		case GlslToken::keyword_mat3x2:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(context), 3), 2);
		case GlslToken::keyword_mat3:
		case GlslToken::keyword_mat3x3:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(context), 3), 3);
		case GlslToken::keyword_mat3x4:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(context), 3), 4);
		case GlslToken::keyword_mat4x2:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(context), 4), 2);
		case GlslToken::keyword_mat4x3:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(context), 4), 3);
		case GlslToken::keyword_mat4:
		case GlslToken::keyword_mat4x4:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(context), 4), 4);
		case GlslToken::keyword_dmat2:
		case GlslToken::keyword_dmat2x2:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getDoubleTy(context), 2), 2);
		case GlslToken::keyword_dmat2x3:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getDoubleTy(context), 2), 3);
		case GlslToken::keyword_dmat2x4:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getDoubleTy(context), 2), 4);
		case GlslToken::keyword_dmat3x2:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getDoubleTy(context), 3), 2);
		case GlslToken::keyword_dmat3:
		case GlslToken::keyword_dmat3x3:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getDoubleTy(context), 3), 3);
		case GlslToken::keyword_dmat3x4:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getDoubleTy(context), 3), 4);
		case GlslToken::keyword_dmat4x2:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getDoubleTy(context), 4), 2);
		case GlslToken::keyword_dmat4x3:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getDoubleTy(context), 4), 3);
		case GlslToken::keyword_dmat4:
		case GlslToken::keyword_dmat4x4:
			return llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getDoubleTy(context), 4), 4);
		default:
			throw Exception("Unknown type");
		}
	}
	else if (glsl_struct)
	{
		throw Exception("Struct types not implemented");
	}
	else
	{
		throw Exception("Unknown type");
	}
}
