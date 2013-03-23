
#include "precomp.h"
#include "glsl_parser.h"

using namespace clan;

GlslParser::GlslParser(std::string source)
: tokenizer(source)
{
}

std::unique_ptr<GlslAstTranslationUnit> GlslParser::parse()
{
	ast.reset(new GlslAstTranslationUnit());
	variable_scopes.push_back(VariableScope());

	while (true)
	{
		tokenizer.read_token(token, true);
		if (token.type == GlslToken::type_eof)
			break;

		FunctionOrGlobalPreparse preparse = preparse_function_or_global();

		if (is_type(GlslToken::type_semicolon))
		{
			GlslAstGlobalVariable *global = new(gc()) GlslAstGlobalVariable();
			global->is_const = preparse.is_const;
			global->is_in = preparse.is_in;
			global->is_out = preparse.is_out;
			global->is_centroid = preparse.is_centroid;
			global->is_patch = preparse.is_patch;
			global->is_sample = preparse.is_sample;
			global->is_uniform = preparse.is_uniform;
			global->layout_ids = preparse.layout_ids;
			global->interpolation = preparse.interpolation;
			global->is_invariant = preparse.is_invariant;
			global->precision = preparse.precision;
			global->type = preparse.type;
			global->name = preparse.name;
			global->is_array = preparse.is_array;
			global->array_size = preparse.array_size;
			ast->globals.push_back(global);
			variable_scopes.back().push_back(global);
		}
		else if (is_operator(GlslToken::operator_paranthesis_begin))
		{
			ast->functions.push_back(parse_function(preparse));
		}
		else
		{
			throw Exception("Syntax error");
		}
	}

	return cl_move(ast);
}

GlslParser::FunctionOrGlobalPreparse GlslParser::preparse_function_or_global()
{
	FunctionOrGlobalPreparse preparse;
	if (is_keyword(GlslToken::keyword_const))
	{
		preparse.is_const = true;
		next();
	}
	if (is_keyword(GlslToken::keyword_in))
	{
		preparse.is_in = true;
		next();
	}
	if (is_keyword(GlslToken::keyword_out))
	{
		preparse.is_out = true;
		next();
	}
	if (is_keyword(GlslToken::keyword_inout))
	{
		preparse.is_in = true;
		preparse.is_out = true;
		next();
	}
	if (is_keyword(GlslToken::keyword_centroid))
	{
		preparse.is_centroid = true;
		next();
	}
	if (is_keyword(GlslToken::keyword_patch))
	{
		preparse.is_patch = true;
		next();
	}
	if (is_keyword(GlslToken::keyword_sample))
	{
		preparse.is_sample = true;
		next();
	}
	if (is_keyword(GlslToken::keyword_uniform))
	{
		preparse.is_in = true;
		next();
	}
	if (is_keyword(GlslToken::keyword_layout))
	{
		parse_layout(preparse.layout_ids);
		next();
	}
	if (is_keyword(GlslToken::keyword_flat) || is_keyword(GlslToken::keyword_smooth))
	{
		preparse.interpolation = is_keyword(GlslToken::keyword_flat) ? GlslAstGlobalVariable::flat : GlslAstGlobalVariable::smooth;
		next();
	}
	if (is_keyword(GlslToken::keyword_invariant))
	{
		preparse.is_invariant = true;
		next();
	}
	if (is_keyword(GlslToken::keyword_lowp) || is_keyword(GlslToken::keyword_mediump) || is_keyword(GlslToken::keyword_highp))
	{
		if (is_keyword(GlslToken::keyword_lowp))
			preparse.precision = lowp;
		else if (is_keyword(GlslToken::keyword_mediump))
			preparse.precision = mediump;
		else
			preparse.precision = highp;
		next();
	}

	if (is_keyword(GlslToken::keyword_struct))
	{
		preparse.type = parse_struct();
		next();
	}
	else if (is_type(GlslToken::type_identifier))
	{
		preparse.type = find_type(token.identifier);
		next();
	}
	else
	{
		preparse.type = parse_built_in_type();
		next();
	}

	if (is_type(GlslToken::type_identifier))
	{
		preparse.name = token.identifier;
		next();
	}
	if (is_operator(GlslToken::operator_bracket_begin))
	{
		preparse.is_array = true;
		preparse.array_size = parse_array_size();
		next();
	}
	return preparse;
}

void GlslParser::parse_layout(std::vector<GlslAstLayoutQualifierId *> &layout_ids)
{
	throw Exception("layout keyword not yet supported");
}

GlslAstType *GlslParser::find_type(const std::string &identifier)
{
	// To do: implement custom types (structs)
	return 0;
}

GlslAstVariable *GlslParser::find_variable(const std::string &identifier)
{
	for (size_t scope_index = variable_scopes.size(); scope_index > 0; scope_index--)
	{
		VariableScope &scope = variable_scopes[scope_index - 1];
		for (size_t variable_index = 0; variable_index < scope.size(); variable_index++)
		{
			if (scope[variable_index]->name == identifier)
			{
				return scope[variable_index];
			}
		}
	}
	return 0;
}

GlslAstBuiltInType *GlslParser::parse_built_in_type()
{
	if (token.type != GlslToken::type_keyword)
		throw Exception("Syntax error");
	switch (token.keyword)
	{
	case GlslToken::keyword_void:
	case GlslToken::keyword_float:
	case GlslToken::keyword_double:
	case GlslToken::keyword_int:
	case GlslToken::keyword_uint:
	case GlslToken::keyword_bool:
	case GlslToken::keyword_vec2:
	case GlslToken::keyword_vec3:
	case GlslToken::keyword_vec4:
	case GlslToken::keyword_dvec2:
	case GlslToken::keyword_dvec3:
	case GlslToken::keyword_dvec4:
	case GlslToken::keyword_bvec2:
	case GlslToken::keyword_bvec3:
	case GlslToken::keyword_bvec4:
	case GlslToken::keyword_ivec2:
	case GlslToken::keyword_ivec3:
	case GlslToken::keyword_ivec4:
	case GlslToken::keyword_uvec2:
	case GlslToken::keyword_uvec3:
	case GlslToken::keyword_uvec4:
	case GlslToken::keyword_mat2:
	case GlslToken::keyword_mat3:
	case GlslToken::keyword_mat4:
	case GlslToken::keyword_mat2x2:
	case GlslToken::keyword_mat2x3:
	case GlslToken::keyword_mat2x4:
	case GlslToken::keyword_mat3x2:
	case GlslToken::keyword_mat3x3:
	case GlslToken::keyword_mat3x4:
	case GlslToken::keyword_mat4x2:
	case GlslToken::keyword_mat4x3:
	case GlslToken::keyword_mat4x4:
	case GlslToken::keyword_dmat2:
	case GlslToken::keyword_dmat3:
	case GlslToken::keyword_dmat4:
	case GlslToken::keyword_dmat2x2:
	case GlslToken::keyword_dmat2x3:
	case GlslToken::keyword_dmat2x4:
	case GlslToken::keyword_dmat3x2:
	case GlslToken::keyword_dmat3x3:
	case GlslToken::keyword_dmat3x4:
	case GlslToken::keyword_dmat4x2:
	case GlslToken::keyword_dmat4x3:
	case GlslToken::keyword_dmat4x4:
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
		if (built_in_types.find(token.keyword) == built_in_types.end())
		{
			built_in_types[token.keyword] = new(gc()) GlslAstBuiltInType();
			built_in_types[token.keyword]->type = token.keyword;
		}
		return built_in_types[token.keyword];
	default:
		throw Exception("Syntax error");
	}
}

GlslAstIntegralConstantExpression *GlslParser::parse_array_size()
{
	next();
	return parse_expression(bracket_end);
}

GlslAstStructType *GlslParser::parse_struct()
{
	throw Exception("Structs not yet supported");
}

GlslAstFunction *GlslParser::parse_function(const FunctionOrGlobalPreparse &preparse)
{
	GlslAstFunction *function = new(gc()) GlslAstFunction();
	function->return_type = preparse.type;
	function->name = preparse.name;

	next(); // paranthesis begin
	while (!is_operator(GlslToken::operator_paranthesis_end))
	{
		GlslAstFunctionParameter *parameter = new(gc()) GlslAstFunctionParameter();
		parameter->is_in = false;
		parameter->is_out = false;
		// to do: memory qualifier missing
		parameter->precision = highp;
		parameter->type = 0;
		parameter->is_array = false;
		parameter->array_size = 0;
		while (!is_operator(GlslToken::operator_comma) && !is_operator(GlslToken::operator_paranthesis_end))
		{
			if (is_keyword(GlslToken::keyword_in))
			{
				parameter->is_in = true;
			}
			else if (is_keyword(GlslToken::keyword_out))
			{
				parameter->is_out = true;
			}
			else if (is_keyword(GlslToken::keyword_inout))
			{
				parameter->is_in = true;
				parameter->is_out = true;
			}
			else if (is_keyword(GlslToken::keyword_highp))
			{
				parameter->precision = highp;
			}
			else if (is_keyword(GlslToken::keyword_mediump))
			{
				parameter->precision = mediump;
			}
			else if (is_keyword(GlslToken::keyword_lowp))
			{
				parameter->precision = lowp;
			}
			else if (is_type(GlslToken::type_identifier))
			{
				GlslAstType *found_type = find_type(token.identifier);
				if (found_type)
					parameter->type = found_type;
				else
					parameter->name = token.identifier;
			}
			else if (is_operator(GlslToken::operator_bracket_begin))
			{
				parameter->array_size = parse_array_size();
			}
			else
			{
				parameter->type = parse_built_in_type();
			}
			next();
		}
		if (parameter->type == 0)
			throw Exception("Syntax error");

		function->parameters.push_back(parameter);
		if (is_operator(GlslToken::operator_comma))
			next();
	}
	next();
	if (is_type(GlslToken::type_scope_begin))
	{
		function->is_prototype = false;
		next();

		variable_scopes.push_back(VariableScope());
		for (size_t i = 0; i < function->parameters.size(); i++)
			variable_scopes.back().push_back(function->parameters[i]);

		while (!is_type(GlslToken::type_scope_end))
		{
			function->statements.push_back(parse_statement());
			next();
		}

		variable_scopes.pop_back();
	}
	else if (is_type(GlslToken::type_semicolon))
	{
		function->is_prototype = true;
	}
	else
	{
		throw Exception("Syntax error");
	}
	return function;
}

GlslAstStatement *GlslParser::parse_statement()
{
	if (is_type(GlslToken::type_scope_begin))
	{
		return parse_compound_statement();
	}
	else if (is_keyword(GlslToken::keyword_if))
	{
		return parse_if_statement();
	}
	else if (is_keyword(GlslToken::keyword_switch))
	{
		return parse_switch_statement();
	}
	else if (is_keyword(GlslToken::keyword_case) || is_keyword(GlslToken::keyword_default))
	{
		return parse_case_label_statement();
	}
	else if (is_keyword(GlslToken::keyword_for))
	{
		return parse_for_statement();
	}
	else if (is_keyword(GlslToken::keyword_do))
	{
		return parse_do_statement();
	}
	else if (is_keyword(GlslToken::keyword_while))
	{
		return parse_while_statement();
	}
	else if (is_keyword(GlslToken::keyword_continue))
	{
		return parse_continue_statement();
	}
	else if (is_keyword(GlslToken::keyword_break))
	{
		return parse_break_statement();
	}
	else if (is_keyword(GlslToken::keyword_discard))
	{
		return parse_discard_statement();
	}
	else if (is_keyword(GlslToken::keyword_return))
	{
		return parse_return_statement();
	}
	else if (is_type(GlslToken::type_semicolon))
	{
		return new(gc()) GlslAstEmptyStatement();
	}
	else
	{
		return parse_declaration_or_expression_statement(semicolon_end);
	}
}

GlslAstCompoundStatement *GlslParser::parse_compound_statement()
{
	GlslAstCompoundStatement *statement = new(gc()) GlslAstCompoundStatement();
	next(); // scope begin
	while (!is_type(GlslToken::type_scope_end))
	{
		statement->statements.push_back(parse_statement());
		next();
	}
	return statement;
}

GlslAstIfStatement *GlslParser::parse_if_statement()
{
	GlslAstIfStatement *statement = new(gc()) GlslAstIfStatement();
	next(); // if keyword
	if (!is_operator(GlslToken::operator_paranthesis_begin))
		throw Exception("Syntax error");
	next();
	statement->condition = parse_expression(paranthesis_end);
	next();
	statement->then_statement = parse_statement();
	GlslToken next_token = tokenizer.peek_token(true);
	if (next_token.type == GlslToken::type_keyword && next_token.keyword == GlslToken::keyword_else)
	{
		next(); // semicolon or scope end
		next(); // else
		statement->else_statement = parse_statement();
	}
	return statement;
}

GlslAstSwitchStatement *GlslParser::parse_switch_statement()
{
	GlslAstSwitchStatement *statement = new(gc()) GlslAstSwitchStatement();
	next(); // switch keyword
	if (!is_operator(GlslToken::operator_paranthesis_begin))
		throw Exception("Syntax error");
	next();
	statement->expression = parse_expression(paranthesis_end);
	next();
	if (!is_type(GlslToken::type_scope_begin))
		throw Exception("Syntax error");
	next();
	while (!is_type(GlslToken::type_scope_end))
	{
		statement->statements.push_back(parse_statement());
		next();
	}
	// To do: verify that the scope is valid
	return statement;
}

GlslAstCaseLabelStatement *GlslParser::parse_case_label_statement()
{
	GlslAstCaseLabelStatement *statement = new(gc()) GlslAstCaseLabelStatement();
	if (is_keyword(GlslToken::keyword_default))
	{
		statement->is_default = false;
		next();
		if (!is_operator(GlslToken::operator_colon))
			throw Exception("Syntax error");
	}
	else
	{
		next(); // case keyword
		statement->condition = parse_expression(colon_end);
		// To do: verify condition is constant
	}
	return statement;
}

GlslAstForStatement *GlslParser::parse_for_statement()
{
	GlslAstForStatement *statement = new(gc()) GlslAstForStatement();
	next(); // for keyword
	if (!is_operator(GlslToken::operator_paranthesis_begin))
		throw Exception("Syntax error");
	next();
	if (!is_type(GlslToken::type_semicolon))
		statement->init = parse_declaration_or_expression_statement(semicolon_end);
	next();
	if (!is_type(GlslToken::type_semicolon))
		statement->condition = parse_expression(semicolon_end);
	next();
	if (!is_operator(GlslToken::operator_paranthesis_end))
		statement->loop = parse_expression(paranthesis_end);
	next();
	statement->body = parse_statement();
	return statement;
}

GlslAstDoStatement *GlslParser::parse_do_statement()
{
	GlslAstDoStatement *statement = new(gc()) GlslAstDoStatement();
	next(); // do keyword
	statement->body = parse_statement();
	next();
	if (!is_keyword(GlslToken::keyword_while))
		throw Exception("Syntax error");
	next();
	if (!is_operator(GlslToken::operator_paranthesis_begin))
		throw Exception("Syntax error");
	next();
	statement->condition = parse_expression(paranthesis_end);
	next();
	return statement;
}

GlslAstWhileStatement *GlslParser::parse_while_statement()
{
	GlslAstWhileStatement *statement = new(gc()) GlslAstWhileStatement();
	next(); // while keyword
	if (!is_operator(GlslToken::operator_paranthesis_begin))
		throw Exception("Syntax error");
	next();
	statement->condition = parse_declaration_or_expression_statement(paranthesis_end);
	next();
	statement->body = parse_statement();
	return statement;
}

GlslAstContinueStatement *GlslParser::parse_continue_statement()
{
	GlslAstContinueStatement *statement = new(gc()) GlslAstContinueStatement();
	next(); // continue keyword
	return statement;
}

GlslAstBreakStatement *GlslParser::parse_break_statement()
{
	GlslAstBreakStatement *statement = new(gc()) GlslAstBreakStatement();
	next(); // break keyword
	return statement;
}

GlslAstDiscardStatement *GlslParser::parse_discard_statement()
{
	GlslAstDiscardStatement *statement = new(gc()) GlslAstDiscardStatement();
	next(); // discard keyword
	return statement;
}

GlslAstReturnStatement *GlslParser::parse_return_statement()
{
	GlslAstReturnStatement *statement = new(gc()) GlslAstReturnStatement();
	next(); // return keyword
	if (!is_type(GlslToken::type_semicolon))
		statement->expression = parse_expression(semicolon_end);
	return statement;
}

GlslAstSimpleStatement *GlslParser::parse_declaration_or_expression_statement(ExpressionEndCondition end_condition)
{
	GlslAstSimpleStatement *declaration = try_parse_declaration_statement(end_condition);
	if (declaration)
		return declaration;
	else
		return parse_expression_statement(end_condition);
}

GlslAstDeclarationStatement *GlslParser::try_parse_declaration_statement(ExpressionEndCondition end_condition)
{
	tokenizer.save_position();

	bool is_const = false;
	GlslAstType *type = 0;

	if (is_keyword(GlslToken::keyword_const))
	{
		is_const = true;
		next();
	}

	if (is_built_in_type_keyword())
		type = parse_built_in_type();
	else if (is_type(GlslToken::type_identifier))
		type = find_type(token.identifier);

	if (type == 0)
	{
		tokenizer.restore_position();
		return 0;
	}

	next();

	GlslAstDeclarationStatement *statement = new(gc()) GlslAstDeclarationStatement();

	while (true)
	{
		int paranthesis_count = 0;
		while (is_operator(GlslToken::operator_paranthesis_begin))
		{
			paranthesis_count++;
			next();
		}

		if (!is_type(GlslToken::type_identifier))
		{
			tokenizer.restore_position();
			return 0;
		}

		std::string name = token.identifier;
		next();

		for (int i = 0; i < paranthesis_count; i++)
		{
			if (!is_operator(GlslToken::operator_paranthesis_end))
			{
				tokenizer.restore_position();
				return 0;
			}
			next();
		}

		if (statement->variables.empty() &&
			!is_operator(GlslToken::operator_assign) &&
			!is_operator(GlslToken::operator_comma) &&
			!is_end_condition(end_condition))
		{
			tokenizer.restore_position();
			return 0;
		}

		GlslAstExpression *initial_value = 0;
		if (is_operator(GlslToken::operator_assign))
		{
			next();
			initial_value = parse_expression(comma_end | end_condition);
		}

		GlslAstFunctionVariable *variable = new(gc()) GlslAstFunctionVariable();
		variable->is_const = is_const;
		variable->type = type;
		variable->name = name;
		variable->initial_value = initial_value;
		variable->is_array = false; // to do: add support for arrays
		variable->array_size = 0;
		statement->variables.push_back(variable);
		variable_scopes.back().push_back(variable);

		if (is_end_condition(end_condition))
		{
			break;
		}
		else if (is_operator(GlslToken::operator_comma))
		{
			next();
		}
		else
		{
			throw Exception("Syntax error");
		}
	}

	return statement;
}

GlslAstExpressionStatement *GlslParser::parse_expression_statement(ExpressionEndCondition end_condition)
{
	GlslAstExpressionStatement *statement = new(gc()) GlslAstExpressionStatement();
	statement->expression = parse_expression(end_condition);
	return statement;
}

GlslAstExpression *GlslParser::parse_expression(ExpressionEndCondition end_condition)
{
	// To do: add support for the trinary select (?:) operator
	GlslAstExpression *lhs = parse_unary(end_condition);
	next();
	return parse_binary(0, lhs, end_condition);
}

GlslAstExpression *GlslParser::parse_unary(ExpressionEndCondition end_condition)
{
	GlslAstExpression *operand = parse_unary_prefix(end_condition);
	while (true)
	{
		GlslToken next_token = tokenizer.peek_token(true);
		if (next_token.type == GlslToken::type_operator && next_token.oper == GlslToken::operator_dot)
		{
			next();
			next();
			if (!is_type(GlslToken::type_identifier))
				throw Exception("Syntax error");
			GlslAstFieldSelectorOrSwizzle *expression = new(gc()) GlslAstFieldSelectorOrSwizzle();
			expression->operand = operand;
			expression->name = token.identifier;
			operand = expression;
		}
		else if (next_token.type == GlslToken::type_operator && next_token.oper == GlslToken::operator_increment)
		{
			next();
			GlslAstUnaryPostfixIncrementExpression *expression = new(gc()) GlslAstUnaryPostfixIncrementExpression();
			expression->operand = operand;
			operand = expression;
		}
		else if (next_token.type == GlslToken::type_operator && next_token.oper == GlslToken::operator_decrement)
		{
			next();
			GlslAstUnaryPostfixDecrementExpression *expression = new(gc()) GlslAstUnaryPostfixDecrementExpression();
			expression->operand = operand;
			operand = expression;
		}
		else if (next_token.type == GlslToken::type_operator && next_token.oper == GlslToken::operator_bracket_begin)
		{
			next();
			next();
			GlslAstArraySubscript *expression = new(gc()) GlslAstArraySubscript();
			expression->operand = operand;
			expression->index = parse_expression(bracket_end);
			operand = expression;
		}
		else
		{
			break;
		}
	}
	return operand;
}

GlslAstExpression *GlslParser::parse_unary_prefix(ExpressionEndCondition end_condition)
{
	if (is_operator(GlslToken::operator_paranthesis_begin))
	{
		next();
		GlslAstExpression *expression = parse_expression(paranthesis_end);
		return expression;
	}
	else if (is_operator(GlslToken::operator_logical_not))
	{
		next();
		GlslAstUnaryLogicalNotExpression *expression = new(gc()) GlslAstUnaryLogicalNotExpression();
		expression->operand = parse_unary(end_condition);
		return expression;
	}
	else if (is_operator(GlslToken::operator_bit_not))
	{
		next();
		GlslAstUnaryBitNotExpression *expression = new(gc()) GlslAstUnaryBitNotExpression();
		expression->operand = parse_unary(end_condition);
		return expression;
	}
	else if (is_operator(GlslToken::operator_plus))
	{
		next();
		GlslAstUnaryPlusExpression *expression = new(gc()) GlslAstUnaryPlusExpression();
		expression->operand = parse_unary(end_condition);
		return expression;
	}
	else if (is_operator(GlslToken::operator_minus))
	{
		next();
		GlslAstUnaryMinusExpression *expression = new(gc()) GlslAstUnaryMinusExpression();
		expression->operand = parse_unary(end_condition);
		return expression;
	}
	else if (is_operator(GlslToken::operator_increment))
	{
		next();
		GlslAstUnaryPrefixIncrementExpression *expression = new(gc()) GlslAstUnaryPrefixIncrementExpression();
		expression->operand = parse_unary(end_condition);
		return expression;
	}
	else if (is_operator(GlslToken::operator_decrement))
	{
		next();
		GlslAstUnaryPrefixDecrementExpression *expression = new(gc()) GlslAstUnaryPrefixDecrementExpression();
		expression->operand = parse_unary(end_condition);
		return expression;
	}
	else if (is_type(GlslToken::type_identifier))
	{
		GlslToken next_token = tokenizer.peek_token(true);
		if (next_token.type == GlslToken::type_operator && next_token.oper == GlslToken::operator_paranthesis_begin)
		{
			GlslAstType *type = find_type(token.identifier);
			if (type)
				return parse_constructor_call();
			else
				return parse_function_call();
		}
		else
		{
			GlslAstVariableIdentifer *expression = new(gc()) GlslAstVariableIdentifer();
			expression->variable = find_variable(token.identifier);
			return expression;
		}
	}
	else if (is_built_in_type_keyword())
	{
		return parse_constructor_call();
	}
	else if (is_keyword(GlslToken::keyword_true))
	{
		GlslAstBoolConstant *expression = new(gc()) GlslAstBoolConstant();
		expression->value = true;
		return expression;
	}
	else if (is_keyword(GlslToken::keyword_false))
	{
		GlslAstBoolConstant *expression = new(gc()) GlslAstBoolConstant();
		expression->value = false;
		return expression;
	}
	else if (is_type(GlslToken::type_int_constant))
	{
		GlslAstIntConstant *expression = new(gc()) GlslAstIntConstant();
		expression->value = token.int_constant;
		return expression;
	}
	else if (is_type(GlslToken::type_uint_constant))
	{
		GlslAstUIntConstant *expression = new(gc()) GlslAstUIntConstant();
		expression->value = token.int_constant;
		return expression;
	}
	else if (is_type(GlslToken::type_float_constant))
	{
		GlslAstFloatConstant *expression = new(gc()) GlslAstFloatConstant();
		expression->value = token.float_constant;
		return expression;
	}
	else if (is_type(GlslToken::type_double_constant))
	{
		GlslAstDoubleConstant *expression = new(gc()) GlslAstDoubleConstant();
		expression->value = token.double_constant;
		return expression;
	}
	else
	{
		throw Exception("Syntax error");
	}
}

GlslAstExpression *GlslParser::parse_binary(int lhs_precendence, GlslAstExpression *lhs, ExpressionEndCondition end_condition)
{
	while (!is_end_condition(end_condition))
	{
		int binary_precendence = get_token_precendence();
		if (binary_precendence < lhs_precendence)
			break;

		GlslAstBinaryExpression *expression = create_token_expression();
		next();

		GlslAstExpression *rhs = parse_unary(end_condition);
		next();
		int rhs_precendence = get_token_precendence();
		if (binary_precendence < rhs_precendence)
			rhs = parse_binary(binary_precendence + 1, rhs, end_condition);

		expression->operand1 = lhs;
		expression->operand2 = rhs;
		lhs = expression;
	}
	return lhs;
}

int GlslParser::get_token_precendence()
{
	if (token.type == GlslToken::type_operator)
	{
		switch (token.oper)
		{
		case GlslToken::operator_paranthesis_begin:
		case GlslToken::operator_paranthesis_end:
			return 17; // highest
		case GlslToken::operator_bracket_begin:
		case GlslToken::operator_bracket_end:
		case GlslToken::operator_dot:
			return 16;
		case GlslToken::operator_increment:
		case GlslToken::operator_decrement:
			// Note: 16 when used postfix
		case GlslToken::operator_bit_not:
		case GlslToken::operator_logical_not:
			return 15;
		case GlslToken::operator_multiply:
		case GlslToken::operator_divide:
		case GlslToken::operator_modulus:
			return 14;
		case GlslToken::operator_plus:
		case GlslToken::operator_minus:
			// Note: 15 when unary
			return 13;
		case GlslToken::operator_shift_left:
		case GlslToken::operator_shift_right:
			return 12;
		case GlslToken::operator_less:
		case GlslToken::operator_greater:
		case GlslToken::operator_less_equal:
		case GlslToken::operator_greater_equal:
			return 11;
		case GlslToken::operator_equal:
		case GlslToken::operator_not_equal:
			return 10;
		case GlslToken::operator_bit_and:
			return 9;
		case GlslToken::operator_bit_xor:
			return 8;
		case GlslToken::operator_bit_or:
			return 7;
		case GlslToken::operator_logical_and:
			return 6;
		case GlslToken::operator_logical_xor:
			return 5;
		case GlslToken::operator_logical_or:
			return 4;
		case GlslToken::operator_questionmark:
		case GlslToken::operator_colon:
			return 3;
		case GlslToken::operator_assign:
		case GlslToken::operator_add_assign:
		case GlslToken::operator_sub_assign:
		case GlslToken::operator_multiply_assign:
		case GlslToken::operator_divide_assign:
		case GlslToken::operator_modulus_assign:
		case GlslToken::operator_shift_left_assign:
		case GlslToken::operator_shift_right_assign:
		case GlslToken::operator_bit_and_assign:
		case GlslToken::operator_bit_xor_assign:
		case GlslToken::operator_bit_or_assign:
			return 2;
		case GlslToken::operator_comma:
			return 1; // lowest
		}
	}
	return -1;
}

GlslAstBinaryExpression *GlslParser::create_token_expression()
{
	if (token.type != GlslToken::type_operator)
		throw Exception("Not a binary operator token");

	switch (token.oper)
	{
	case GlslToken::operator_multiply:
		return new(gc()) GlslAstMultiplyExpression();
	case GlslToken::operator_divide:
		return new(gc()) GlslAstDivideExpression();
	case GlslToken::operator_modulus:
		return new(gc()) GlslAstModulusExpression();
	case GlslToken::operator_plus:
		return new(gc()) GlslAstPlusExpression();
	case GlslToken::operator_minus:
		return new(gc()) GlslAstMinusExpression();
	case GlslToken::operator_shift_left:
		return new(gc()) GlslAstShiftLeftExpression();
	case GlslToken::operator_shift_right:
		return new(gc()) GlslAstShiftRightExpression();
	case GlslToken::operator_less:
		return new(gc()) GlslAstLessExpression();
	case GlslToken::operator_greater:
		return new(gc()) GlslAstGreaterExpression();
	case GlslToken::operator_less_equal:
		return new(gc()) GlslAstLessEqualExpression();
	case GlslToken::operator_greater_equal:
		return new(gc()) GlslAstGreaterEqualExpression();
	case GlslToken::operator_equal:
		return new(gc()) GlslAstEqualExpression();
	case GlslToken::operator_not_equal:
		return new(gc()) GlslAstNotEqualExpression();
	case GlslToken::operator_bit_and:
		return new(gc()) GlslAstBitAndExpression();
	case GlslToken::operator_bit_xor:
		return new(gc()) GlslAstBitXorExpression();
	case GlslToken::operator_bit_or:
		return new(gc()) GlslAstBitOrExpression();
	case GlslToken::operator_logical_and:
		return new(gc()) GlslAstLogicalAndExpression();
	case GlslToken::operator_logical_xor:
		return new(gc()) GlslAstLogicalXorExpression();
	case GlslToken::operator_logical_or:
		return new(gc()) GlslAstLogicalOrExpression();
	case GlslToken::operator_assign:
	case GlslToken::operator_add_assign:
	case GlslToken::operator_sub_assign:
	case GlslToken::operator_multiply_assign:
	case GlslToken::operator_divide_assign:
	case GlslToken::operator_modulus_assign:
	case GlslToken::operator_shift_left_assign:
	case GlslToken::operator_shift_right_assign:
	case GlslToken::operator_bit_and_assign:
	case GlslToken::operator_bit_xor_assign:
	case GlslToken::operator_bit_or_assign:
		{
			GlslAstAssignmentExpression *expression = new(gc()) GlslAstAssignmentExpression();
			expression->assignment_type = token.oper;
			return expression;
		}
	case GlslToken::operator_comma:
		return new(gc()) GlslAstSequenceExpression();
	default:
		throw Exception("Not a binary operator token");
	}
}

GlslAstFunctionCall *GlslParser::parse_function_call()
{
	GlslAstFunctionCall *expression = new(gc()) GlslAstFunctionCall();
	expression->name = token.identifier;
	next();
	if (!is_operator(GlslToken::operator_paranthesis_begin))
		throw Exception("Syntax error");
	next();
	while (!is_operator(GlslToken::operator_paranthesis_end))
	{
		GlslAstExpression *parameter = parse_expression(comma_end|paranthesis_end);
		expression->parameters.push_back(parameter);
		if (is_operator(GlslToken::operator_comma))
			next();
	}
	return expression;
}

GlslAstConstructorCall *GlslParser::parse_constructor_call()
{
	GlslAstConstructorCall *expression = new(gc()) GlslAstConstructorCall();
	expression->type = parse_built_in_type();
	next();
	if (!is_operator(GlslToken::operator_paranthesis_begin))
		throw Exception("Syntax error");
	next();
	while (!is_operator(GlslToken::operator_paranthesis_end))
	{
		GlslAstExpression *parameter = parse_expression(comma_end|paranthesis_end);
		expression->parameters.push_back(parameter);
		if (is_operator(GlslToken::operator_comma))
			next();
	}
	return expression;
}

void GlslParser::next()
{
	tokenizer.read_token(token, true);
	if (token.type == GlslToken::type_eof)
		throw Exception("Premature end of file while parsing shader program");
}

bool GlslParser::is_end_condition(ExpressionEndCondition end_condition) const
{
	return
		((end_condition & semicolon_end) && is_type(GlslToken::type_semicolon)) ||
		((end_condition & paranthesis_end) && is_operator(GlslToken::operator_paranthesis_end)) ||
		((end_condition & bracket_end) && is_operator(GlslToken::operator_bracket_end)) ||
		((end_condition & colon_end) && is_operator(GlslToken::operator_colon)) ||
		((end_condition & comma_end) && is_operator(GlslToken::operator_comma));
}

bool GlslParser::is_type(GlslToken::Type type) const
{
	return token.type == type;
}

bool GlslParser::is_keyword(GlslToken::Keyword keyword) const
{
	return token.type == GlslToken::type_keyword && token.keyword == keyword;
}

bool GlslParser::is_operator(GlslToken::Operator oper) const
{
	return token.type == GlslToken::type_operator && token.oper == oper;
}

bool GlslParser::is_built_in_type_keyword() const
{
	if (token.type != GlslToken::type_keyword)
		return false;
	switch (token.keyword)
	{
	case GlslToken::keyword_void:
	case GlslToken::keyword_float:
	case GlslToken::keyword_double:
	case GlslToken::keyword_int:
	case GlslToken::keyword_uint:
	case GlslToken::keyword_bool:
	case GlslToken::keyword_vec2:
	case GlslToken::keyword_vec3:
	case GlslToken::keyword_vec4:
	case GlslToken::keyword_dvec2:
	case GlslToken::keyword_dvec3:
	case GlslToken::keyword_dvec4:
	case GlslToken::keyword_bvec2:
	case GlslToken::keyword_bvec3:
	case GlslToken::keyword_bvec4:
	case GlslToken::keyword_ivec2:
	case GlslToken::keyword_ivec3:
	case GlslToken::keyword_ivec4:
	case GlslToken::keyword_uvec2:
	case GlslToken::keyword_uvec3:
	case GlslToken::keyword_uvec4:
	case GlslToken::keyword_mat2:
	case GlslToken::keyword_mat3:
	case GlslToken::keyword_mat4:
	case GlslToken::keyword_mat2x2:
	case GlslToken::keyword_mat2x3:
	case GlslToken::keyword_mat2x4:
	case GlslToken::keyword_mat3x2:
	case GlslToken::keyword_mat3x3:
	case GlslToken::keyword_mat3x4:
	case GlslToken::keyword_mat4x2:
	case GlslToken::keyword_mat4x3:
	case GlslToken::keyword_mat4x4:
	case GlslToken::keyword_dmat2:
	case GlslToken::keyword_dmat3:
	case GlslToken::keyword_dmat4:
	case GlslToken::keyword_dmat2x2:
	case GlslToken::keyword_dmat2x3:
	case GlslToken::keyword_dmat2x4:
	case GlslToken::keyword_dmat3x2:
	case GlslToken::keyword_dmat3x3:
	case GlslToken::keyword_dmat3x4:
	case GlslToken::keyword_dmat4x2:
	case GlslToken::keyword_dmat4x3:
	case GlslToken::keyword_dmat4x4:
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

#ifdef NOT_USED

void GlslParser::parse()
{
	while (external_declaration());
}

bool GlslParser::external_declaration()
{
	return function_definition() || declaration();
}

bool GlslParser::function_definition()
{
	return function_prototype() && compound_statement_no_new_scope();
}

bool GlslParser::function_prototype()
{
	return function_declarator() && token(GlslToken::operator_paranthesis_end);
}

bool GlslParser::function_declarator()
{
	return function_header() || function_header_with_parameters();
}

bool GlslParser::function_header()
{
	return fully_specified_type() && token(GlslToken::type_identifier) && token(GlslToken::operator_paranthesis_begin);
}

bool GlslParser::function_header_with_parameters()
{
	return
		(function_header() && parameter_declaration()) ||
		(fully_specified_type() && token(GlslToken::operator_comma) && parameter_declaration());
}

bool GlslParser::parameter_declaration()
{
	return
		(parameter_type_qualifier() && parameter_qualifier() && parameter_declarator()) ||
		(parameter_qualifier() && parameter_declarator()) ||
		(parameter_type_qualifier() && parameter_qualifier() && parameter_type_specifier()) ||
		(parameter_qualifier() && parameter_type_specifier());
}

bool GlslParser::parameter_type_qualifier()
{
	return token(GlslToken::keyword_const);
}

bool GlslParser::parameter_qualifier()
{
	return empty() || token(GlslToken::keyword_in) || token(GlslToken::keyword_out) || token(GlslToken::keyword_inout);
}

bool GlslParser::parameter_declarator()
{
	return
		(type_specifier() && token(GlslToken::type_identifier)) ||
		(type_specifier() && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && constant_expression() && token(GlslToken::operator_bracket_end));
}

bool GlslParser::parameter_type_specifier()
{
	return type_specifier();
}

bool GlslParser::fully_specified_type()
{
	return type_specifier() || (type_qualifier() && type_specifier());
}

bool GlslParser::type_specifier()
{
	return 
		type_specifier_no_prec() ||
		(precision_qualifier() && type_specifier_no_prec());
}

bool GlslParser::precision_qualifier()
{
	return
		token(GlslToken::keyword_highp) ||
		token(GlslToken::keyword_mediump) ||
		token(GlslToken::keyword_lowp);
}

bool GlslParser::type_specifier_no_prec()
{
	return
		type_specifier_nonarray() ||
		(type_specifier_nonarray() && token(GlslToken::operator_bracket_begin) && token(GlslToken::operator_bracket_end)) ||
		(type_specifier_nonarray() && token(GlslToken::operator_bracket_begin) && constant_expression() && token(GlslToken::operator_bracket_end));
}

bool GlslParser::type_specifier_nonarray()
{
	return
		token(GlslToken::keyword_void) ||
		token(GlslToken::keyword_float) ||
		token(GlslToken::keyword_double) ||
		token(GlslToken::keyword_int) ||
		token(GlslToken::keyword_uint) ||
		token(GlslToken::keyword_bool) ||
		token(GlslToken::keyword_vec2) ||
		token(GlslToken::keyword_vec3) ||
		token(GlslToken::keyword_vec4) ||
		token(GlslToken::keyword_dvec2) ||
		token(GlslToken::keyword_dvec3) ||
		token(GlslToken::keyword_dvec4) ||
		token(GlslToken::keyword_bvec2) ||
		token(GlslToken::keyword_bvec3) ||
		token(GlslToken::keyword_bvec4) ||
		token(GlslToken::keyword_ivec2) ||
		token(GlslToken::keyword_ivec3) ||
		token(GlslToken::keyword_ivec4) ||
		token(GlslToken::keyword_uvec2) ||
		token(GlslToken::keyword_uvec3) ||
		token(GlslToken::keyword_uvec4) ||
		token(GlslToken::keyword_mat2) ||
		token(GlslToken::keyword_mat3) ||
		token(GlslToken::keyword_mat4) ||
		token(GlslToken::keyword_mat2x2) ||
		token(GlslToken::keyword_mat2x3) ||
		token(GlslToken::keyword_mat2x4) ||
		token(GlslToken::keyword_mat3x2) ||
		token(GlslToken::keyword_mat3x3) ||
		token(GlslToken::keyword_mat3x4) ||
		token(GlslToken::keyword_mat4x2) ||
		token(GlslToken::keyword_mat4x3) ||
		token(GlslToken::keyword_mat4x4) ||
		token(GlslToken::keyword_dmat2) ||
		token(GlslToken::keyword_dmat3) ||
		token(GlslToken::keyword_dmat4) ||
		token(GlslToken::keyword_dmat2x2) ||
		token(GlslToken::keyword_dmat2x3) ||
		token(GlslToken::keyword_dmat2x4) ||
		token(GlslToken::keyword_dmat3x2) ||
		token(GlslToken::keyword_dmat3x3) ||
		token(GlslToken::keyword_dmat3x4) ||
		token(GlslToken::keyword_dmat4x2) ||
		token(GlslToken::keyword_dmat4x3) ||
		token(GlslToken::keyword_dmat4x4) ||
		token(GlslToken::keyword_sampler1D) ||
		token(GlslToken::keyword_sampler2D) ||
		token(GlslToken::keyword_sampler3D) ||
		token(GlslToken::keyword_samplerCube) ||
		token(GlslToken::keyword_sampler1DShadow) ||
		token(GlslToken::keyword_sampler2DShadow) ||
		token(GlslToken::keyword_samplerCubeShadow) ||
		token(GlslToken::keyword_sampler1DArray) ||
		token(GlslToken::keyword_sampler2DArray) ||
		token(GlslToken::keyword_sampler1DArrayShadow) ||
		token(GlslToken::keyword_sampler2DArrayShadow) ||
		token(GlslToken::keyword_samplerCubeArray) ||
		token(GlslToken::keyword_samplerCubeArrayShadow) ||
		token(GlslToken::keyword_isampler1D) ||
		token(GlslToken::keyword_isampler2D) ||
		token(GlslToken::keyword_isampler3D) ||
		token(GlslToken::keyword_isamplerCube) ||
		token(GlslToken::keyword_isampler1DArray) ||
		token(GlslToken::keyword_isampler2DArray) ||
		token(GlslToken::keyword_isamplerCubeArray) ||
		token(GlslToken::keyword_usampler1D) ||
		token(GlslToken::keyword_usampler2D) ||
		token(GlslToken::keyword_usampler3D) ||
		token(GlslToken::keyword_usamplerCube) ||
		token(GlslToken::keyword_usampler1DArray) ||
		token(GlslToken::keyword_usampler2DArray) ||
		token(GlslToken::keyword_usamplerCubeArray) ||
		token(GlslToken::keyword_sampler2DRect) ||
		token(GlslToken::keyword_sampler2DRectShadow) ||
		token(GlslToken::keyword_isampler2DRect) ||
		token(GlslToken::keyword_usampler2DRect) ||
		token(GlslToken::keyword_samplerBuffer) ||
		token(GlslToken::keyword_isamplerBuffer) ||
		token(GlslToken::keyword_usamplerBuffer) ||
		token(GlslToken::keyword_sampler2DMS) ||
		token(GlslToken::keyword_isampler2DMS) ||
		token(GlslToken::keyword_usampler2DMS) ||
		token(GlslToken::keyword_sampler2DMSArray) ||
		token(GlslToken::keyword_isampler2DMSArray) ||
		token(GlslToken::keyword_usampler2DMSArray) ||
		struct_specifier() ||
		type_name();
}

bool GlslParser::type_qualifier()
{
	return
		storage_qualifier() ||
		layout_qualifier() ||
		(layout_qualifier() && storage_qualifier()) ||
		(interpolation_qualifier() && storage_qualifier()) ||
		interpolation_qualifier() ||
		(invariant_qualifier() && storage_qualifier()) ||
		(invariant_qualifier() && interpolation_qualifier() && storage_qualifier()) ||
		invariant_qualifier();
}

bool GlslParser::storage_qualifier()
{
	return
		token(GlslToken::keyword_const) ||
		token(GlslToken::keyword_in) ||
		token(GlslToken::keyword_out) ||
		(token(GlslToken::keyword_centroid) && token(GlslToken::keyword_in)) ||
		(token(GlslToken::keyword_centroid) && token(GlslToken::keyword_out)) ||
		(token(GlslToken::keyword_patch) && token(GlslToken::keyword_in)) ||
		(token(GlslToken::keyword_patch) && token(GlslToken::keyword_out)) ||
		(token(GlslToken::keyword_sample) && token(GlslToken::keyword_in)) ||
		(token(GlslToken::keyword_sample) && token(GlslToken::keyword_out)) ||
		token(GlslToken::keyword_uniform);
}

bool GlslParser::layout_qualifier()
{
	return token(GlslToken::keyword_layout) && token(GlslToken::operator_paranthesis_begin) && layout_qualifier_id_list() && token(GlslToken::operator_paranthesis_end);
}

bool GlslParser::layout_qualifier_id_list()
{
	return
		layout_qualifier_id() ||
		(layout_qualifier_id_list() && token(GlslToken::operator_comma) && layout_qualifier_id());
}

bool GlslParser::layout_qualifier_id()
{
	return
		token(GlslToken::type_identifier) ||
		(token(GlslToken::type_identifier) && token(GlslToken::operator_assign) && token(GlslToken::type_int_constant));
}

bool GlslParser::interpolation_qualifier()
{
	return
		token(GlslToken::keyword_smooth) ||
		token(GlslToken::keyword_flat) ||
		token(GlslToken::keyword_noperspective);
}

bool GlslParser::invariant_qualifier()
{
	return token(GlslToken::keyword_invariant);
}

bool GlslParser::compound_statement_no_new_scope()
{
	return 
		(token(GlslToken::type_scope_begin) && token(GlslToken::type_scope_end)) ||
		(token(GlslToken::type_scope_begin) && statement_list() && token(GlslToken::type_scope_end));
}

bool GlslParser::constant_expression()
{
	return conditional_expression();
}

bool GlslParser::declaration()
{
	return
		(function_prototype() && token(GlslToken::type_semicolon)) ||
		(init_declarator_list() && token(GlslToken::type_semicolon)) ||
		(token(GlslToken::keyword_precision) && precision_qualifier() && type_specifier_no_prec() && token(GlslToken::type_semicolon)) ||
		(type_qualifier() && token(GlslToken::type_identifier) && token(GlslToken::type_scope_begin) && struct_declaration_list() && token(GlslToken::type_scope_end) && token(GlslToken::type_semicolon)) ||
		(type_qualifier() && token(GlslToken::type_identifier) && token(GlslToken::type_scope_begin) && struct_declaration_list() && token(GlslToken::type_scope_end) && token(GlslToken::type_identifier) && token(GlslToken::type_semicolon)) ||
		(type_qualifier() && token(GlslToken::type_identifier) && token(GlslToken::type_scope_begin) && struct_declaration_list() && token(GlslToken::type_scope_end) && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && token(GlslToken::operator_bracket_end) && token(GlslToken::type_semicolon)) ||
		(type_qualifier() && token(GlslToken::type_identifier) && token(GlslToken::type_scope_begin) && struct_declaration_list() && token(GlslToken::type_scope_end) && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && constant_expression() && token(GlslToken::operator_bracket_end) && token(GlslToken::type_semicolon)) ||
		(type_qualifier() && token(GlslToken::type_semicolon));
}

bool GlslParser::init_declarator_list()
{
	return
		single_declaration() ||
		(init_declarator_list() && token(GlslToken::operator_comma) && token(GlslToken::type_identifier)) ||
		(init_declarator_list() && token(GlslToken::operator_comma) && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && token(GlslToken::operator_bracket_end)) ||
		(init_declarator_list() && token(GlslToken::operator_comma) && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && constant_expression() && token(GlslToken::operator_bracket_end)) ||
		(init_declarator_list() && token(GlslToken::operator_comma) && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && token(GlslToken::operator_bracket_end) && token(GlslToken::operator_assign) && initializer()) ||
		(init_declarator_list() && token(GlslToken::operator_comma) && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && constant_expression() && token(GlslToken::operator_bracket_end) && token(GlslToken::operator_assign) && initializer()) ||
		(init_declarator_list() && token(GlslToken::operator_comma) && token(GlslToken::type_identifier) && token(GlslToken::operator_assign) && initializer());
}

bool GlslParser::single_declaration()
{
	return
		fully_specified_type() ||
		(fully_specified_type() && token(GlslToken::type_identifier)) ||
		(fully_specified_type() && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && token(GlslToken::operator_bracket_end)) ||
		(fully_specified_type() && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && constant_expression() && token(GlslToken::operator_bracket_end)) ||
		(fully_specified_type() && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && token(GlslToken::operator_bracket_end) && token(GlslToken::operator_assign) && initializer()) ||
		(fully_specified_type() && token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && constant_expression() && token(GlslToken::operator_bracket_end) && token(GlslToken::operator_assign) && initializer()) ||
		(fully_specified_type() && token(GlslToken::type_identifier) && token(GlslToken::operator_assign) && initializer() && token(GlslToken::keyword_invariant) && token(GlslToken::type_identifier));
}

bool GlslParser::initializer()
{
	return assignment_expression();
}

bool GlslParser::assignment_expression()
{
	return
		conditional_expression() ||
		(unary_expression() && assignment_operator() && assignment_expression());
}

bool GlslParser::unary_expression()
{
	return
		postfix_expression() ||
		(token(GlslToken::operator_increment) && unary_expression()) ||
		(token(GlslToken::operator_decrement) && unary_expression()) ||
		(unary_operator() && unary_expression());
}

bool GlslParser::unary_operator()
{
	return
		token(GlslToken::operator_plus) ||
		token(GlslToken::operator_minus) ||
		token(GlslToken::operator_logical_not) ||
		token(GlslToken::operator_bit_not);
}

bool GlslParser::assignment_operator()
{
	return
		token(GlslToken::operator_assign) ||
		token(GlslToken::operator_multiply_assign) ||
		token(GlslToken::operator_divide_assign) ||
		token(GlslToken::operator_modulus_assign) ||
		token(GlslToken::operator_add_assign) ||
		token(GlslToken::operator_sub_assign) ||
		token(GlslToken::operator_shift_left_assign) ||
		token(GlslToken::operator_shift_right_assign) ||
		token(GlslToken::operator_bit_and_assign) ||
		token(GlslToken::operator_bit_xor_assign) ||
		token(GlslToken::operator_bit_or_assign);
}

bool GlslParser::postfix_expression()
{
	return
		primary_expression() ||
		(postfix_expression() && token(GlslToken::operator_bracket_begin) && integer_expression() && token(GlslToken::operator_bracket_end)) ||
		function_call() ||
		(postfix_expression() && token(GlslToken::operator_dot) && token(GlslToken::type_identifier/*FIELD_SELECTION*/)) ||
		(postfix_expression() && token(GlslToken::operator_increment)) ||
		(postfix_expression() && token(GlslToken::operator_decrement));
}

bool GlslParser::integer_expression()
{
	return expression();
}

bool GlslParser::function_call()
{
	return function_call_or_method();
}

bool GlslParser::function_call_or_method()
{
	return
		function_call_generic() ||
		(postfix_expression() && token(GlslToken::operator_dot) && function_call_generic());
}

bool GlslParser::function_call_generic()
{
	return
		(function_call_header_with_parameters() && token(GlslToken::operator_paranthesis_end)) ||
		(function_call_header_no_parameters() && token(GlslToken::operator_paranthesis_end));
}

bool GlslParser::function_call_header_no_parameters()
{
	return
		(function_call_header() && token(GlslToken::keyword_void)) ||
		function_call_header();
}

bool GlslParser::function_call_header_with_parameters()
{
	return
		(function_call_header() && assignment_expression()) ||
		(function_call_header_with_parameters() && token(GlslToken::operator_comma) && assignment_expression());
}

bool GlslParser::function_call_header()
{
	return function_identifier() && token(GlslToken::operator_paranthesis_begin);
}

bool GlslParser::function_identifier()
{
	return
		type_specifier() ||
		token(GlslToken::type_identifier)/* ||
		FIELD_SELECTION*/;
}

bool GlslParser::type_name()
{
	// To do: only match defined structs
	return token(GlslToken::type_identifier);
}

bool GlslParser::struct_specifier()
{
	return
		(token(GlslToken::keyword_struct) && token(GlslToken::type_identifier) && token(GlslToken::type_scope_begin) && struct_declaration_list() && token(GlslToken::type_scope_end)) ||
		(token(GlslToken::keyword_struct) && token(GlslToken::type_scope_begin) && struct_declaration_list() && token(GlslToken::type_scope_end));
}

bool GlslParser::struct_declaration_list()
{
	return
		struct_declaration() ||
		(struct_declaration_list() && struct_declaration());
}

bool GlslParser::struct_declaration()
{
	return
		(type_specifier() && struct_declarator_list() && token(GlslToken::type_semicolon)) ||
		(type_qualifier() && type_specifier() && struct_declarator_list() && token(GlslToken::type_semicolon));
}

bool GlslParser::struct_declarator_list()
{
	return
		struct_declarator() ||
		(struct_declarator_list() && token(GlslToken::operator_comma) && struct_declarator());
}

bool GlslParser::struct_declarator()
{
	return
		token(GlslToken::type_identifier) ||
		(token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && token(GlslToken::operator_bracket_end)) ||
		(token(GlslToken::type_identifier) && token(GlslToken::operator_bracket_begin) && constant_expression() && token(GlslToken::operator_bracket_end));
}

bool GlslParser::statement_list()
{
	return
		statement() ||
		(statement_list() && statement());
}

bool GlslParser::statement()
{
	return
		compound_statement() ||
		simple_statement();
}

bool GlslParser::compound_statement()
{
	return
		(token(GlslToken::type_scope_begin) && token(GlslToken::type_scope_end)) ||
		(token(GlslToken::type_scope_begin) && statement_list() && token(GlslToken::type_scope_end));
}

bool GlslParser::simple_statement()
{
	return
		declaration_statement() ||
		expression_statement() ||
		selection_statement() ||
		switch_statement() ||
		case_label() ||
		iteration_statement() ||
		jump_statement();
}

bool GlslParser::declaration_statement()
{
	return declaration();
}

bool GlslParser::expression_statement()
{
	return
		token(GlslToken::type_semicolon) ||
		(expression() && token(GlslToken::type_semicolon));
}

bool GlslParser::selection_statement()
{
	return token(GlslToken::keyword_if) && token(GlslToken::operator_paranthesis_begin) && expression() && token(GlslToken::operator_paranthesis_end) && selection_rest_statement();
}

bool GlslParser::selection_rest_statement()
{
	return
		(statement() && token(GlslToken::keyword_else) && statement()) ||
		statement();
}

bool GlslParser::switch_statement()
{
	return
		token(GlslToken::keyword_switch) && token(GlslToken::operator_paranthesis_begin) && expression() && token(GlslToken::operator_paranthesis_end) && token(GlslToken::type_scope_begin) && switch_statement_list() && token(GlslToken::type_scope_end);
}

bool GlslParser::switch_statement_list()
{
	return
		empty() ||
		statement_list();
}

bool GlslParser::case_label()
{
	return
		(token(GlslToken::keyword_case) && expression() && token(GlslToken::operator_colon)) ||
		(token(GlslToken::keyword_default) && token(GlslToken::operator_colon));
}

bool GlslParser::iteration_statement()
{
	return
		(token(GlslToken::keyword_while) && token(GlslToken::operator_paranthesis_begin) && condition() && token(GlslToken::operator_paranthesis_end) && statement_no_new_scope()) ||
		(token(GlslToken::keyword_do) && statement() && token(GlslToken::keyword_while) && token(GlslToken::operator_paranthesis_begin) && expression() && token(GlslToken::operator_paranthesis_end) && token(GlslToken::type_semicolon)) ||
		(token(GlslToken::keyword_for) && token(GlslToken::operator_paranthesis_begin) && for_init_statement() && for_rest_statement() && token(GlslToken::operator_paranthesis_end) && statement_no_new_scope());
}

bool GlslParser::for_init_statement()
{
	return
		expression_statement() ||
		declaration_statement();
}

bool GlslParser::conditionopt()
{
	return
		condition() ||
		empty();
}

bool GlslParser::condition()
{
	return
		expression() ||
		(fully_specified_type() && token(GlslToken::type_identifier) && token(GlslToken::operator_equal) && initializer());
}

bool GlslParser::statement_no_new_scope()
{
	return
		compound_statement_no_new_scope() ||
		simple_statement();
}

bool GlslParser::for_rest_statement()
{
	return
		(conditionopt() && token(GlslToken::type_semicolon)) ||
		(conditionopt() && token(GlslToken::type_semicolon) && expression());
}

bool GlslParser::jump_statement()
{
	return
		(token(GlslToken::keyword_continue) && token(GlslToken::type_semicolon)) ||
		(token(GlslToken::keyword_break) && token(GlslToken::type_semicolon)) ||
		(token(GlslToken::keyword_return) && token(GlslToken::type_semicolon)) ||
		(token(GlslToken::keyword_return) && expression() && token(GlslToken::type_semicolon)) ||
		(token(GlslToken::keyword_discard) && token(GlslToken::type_semicolon)); // Fragment shader only.
}

bool GlslParser::conditional_expression()
{
	return
		logical_or_expression() ||
		(logical_or_expression() && token(GlslToken::operator_questionmark) && expression() && token(GlslToken::operator_colon) && assignment_expression());
}

bool GlslParser::logical_or_expression()
{
	return
		logical_xor_expression() ||
		(logical_or_expression() && token(GlslToken::operator_logical_or) && logical_xor_expression());
}

bool GlslParser::logical_xor_expression()
{
	return
		logical_and_expression() ||
		(logical_xor_expression() && token(GlslToken::operator_logical_xor) && logical_and_expression());
}

bool GlslParser::logical_and_expression()
{
	return
		inclusive_or_expression() ||
		(logical_and_expression() && token(GlslToken::operator_logical_and) && inclusive_or_expression());
}

bool GlslParser::inclusive_or_expression()
{
	return
		exclusive_or_expression() ||
		(inclusive_or_expression() && token(GlslToken::operator_bit_or) && exclusive_or_expression());
}

bool GlslParser::exclusive_or_expression()
{
	return
		and_expression() ||
		(exclusive_or_expression() && token(GlslToken::operator_bit_xor) && and_expression());
}

bool GlslParser::and_expression()
{
	return
		equality_expression() ||
		(and_expression() && token(GlslToken::operator_bit_and) && equality_expression());
}

bool GlslParser::equality_expression()
{
	return
		relational_expression() ||
		(equality_expression() && token(GlslToken::operator_equal) && relational_expression()) ||
		(equality_expression() && token(GlslToken::operator_not_equal) && relational_expression());
}

bool GlslParser::relational_expression()
{
	return
		shift_expression() ||
		(relational_expression() && token(GlslToken::operator_less) && shift_expression()) ||
		(relational_expression() && token(GlslToken::operator_greater) && shift_expression()) ||
		(relational_expression() && token(GlslToken::operator_less_equal) && shift_expression()) ||
		(relational_expression() && token(GlslToken::operator_greater_equal) && shift_expression());
}

bool GlslParser::shift_expression()
{
	return
		additive_expression() ||
		(shift_expression() && token(GlslToken::operator_shift_left) && additive_expression()) ||
		(shift_expression() && token(GlslToken::operator_shift_right) && additive_expression());
}

bool GlslParser::additive_expression()
{
	return
		multiplicative_expression() ||
		(additive_expression() && token(GlslToken::operator_plus) && multiplicative_expression()) ||
		(additive_expression() && token(GlslToken::operator_minus) && multiplicative_expression());
}

bool GlslParser::multiplicative_expression()
{
	return
		unary_expression() ||
		(multiplicative_expression() && token(GlslToken::operator_multiply) && unary_expression()) ||
		(multiplicative_expression() && token(GlslToken::operator_divide) && unary_expression()) ||
		(multiplicative_expression() && token(GlslToken::operator_modulus) && unary_expression());
}

bool GlslParser::expression()
{
	return
		assignment_expression() ||
		(expression() && token(GlslToken::operator_comma) && assignment_expression());
}

bool GlslParser::primary_expression()
{
	return 
		variable_identifier() ||
		token(GlslToken::type_int_constant) ||
		token(GlslToken::type_uint_constant) ||
		token(GlslToken::type_float_constant) ||
		token(GlslToken::type_double_constant) ||
		token(GlslToken::keyword_true) ||
		token(GlslToken::keyword_false) ||
		(token(GlslToken::operator_paranthesis_begin) && expression() && token(GlslToken::operator_paranthesis_end));
}

bool GlslParser::variable_identifier()
{
	return token(GlslToken::type_identifier);
}

#endif
