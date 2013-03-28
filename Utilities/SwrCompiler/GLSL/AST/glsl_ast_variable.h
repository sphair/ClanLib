
#pragma once

#include "glsl_ast_node.h"

class GlslAstExpression;
typedef GlslAstExpression GlslAstIntegralConstantExpression;
class GlslAstType;
class GlslAstLayoutQualifierId;

enum GlslAstPrecision
{
	highp,
	mediump,
	lowp
};

class GlslAstVariable : public GlslAstNode
{
public:
	GlslAstVariable() : type(), is_array(), array_size() { }

	std::string name;
	GlslAstType *type;
	bool is_array;
	GlslAstIntegralConstantExpression *array_size;
};

class GlslAstFunctionVariable : public GlslAstVariable
{
public:
	GlslAstFunctionVariable() : is_const(), initial_value() { }

	bool is_const;
	GlslAstExpression *initial_value;
};

class GlslAstFunctionParameter : public GlslAstVariable
{
public:
	GlslAstFunctionParameter() : is_in(), is_out(), precision() { }

	bool is_in;
	bool is_out;
	// to do: memory qualifier missing
	GlslAstPrecision precision;
};

class GlslAstGlobalVariable : public GlslAstVariable
{
public:
	GlslAstGlobalVariable() : is_const(), is_in(), is_out(), is_centroid(), is_patch(), is_sample(), is_uniform(), interpolation(), is_invariant(), precision() { }

	bool is_const;
	bool is_in;
	bool is_out;
	bool is_centroid;
	bool is_patch;
	bool is_sample;
	bool is_uniform;
	std::vector<GlslAstLayoutQualifierId *> layout_ids;
	enum Interpolation
	{
		smooth,
		flat,
		perspective
	};
	Interpolation interpolation;
	bool is_invariant;
	GlslAstPrecision precision;
};

class GlslAstLayoutQualifierId : public GlslAstNode
{
public:
	GlslAstLayoutQualifierId() : has_id(), id() { }

	std::string name;
	bool has_id;
	int id;
};
