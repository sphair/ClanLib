
#pragma once

#include "GLSL/Lex/glsl_tokenizer.h"
#include "GLSL/AST/glsl_ast.h"

class GlslParser
{
public:
	GlslParser(std::string source);
	std::unique_ptr<GlslAstTranslationUnit> parse();

private:
	std::unique_ptr<GlslAstTranslationUnit> ast;

	struct FunctionOrGlobalPreparse
	{
		FunctionOrGlobalPreparse()
		: is_const(false), is_in(false), is_out(false), is_centroid(false), is_patch(false), is_sample(false),
		  is_uniform(false), interpolation(GlslAstGlobalVariable::perspective), is_invariant(false), precision(highp),
		  type(0), is_array(false), array_size(0)
		{
		}

		bool is_const;
		bool is_in;
		bool is_out;
		bool is_centroid;
		bool is_patch;
		bool is_sample;
		bool is_uniform;
		std::vector<GlslAstLayoutQualifierId *> layout_ids;
		GlslAstGlobalVariable::Interpolation interpolation;
		bool is_invariant;
		GlslAstPrecision precision;
		GlslAstType *type;
		std::string name;
		bool is_array;
		GlslAstIntegralConstantExpression *array_size;
	};

	typedef std::vector<GlslAstVariable *> VariableScope;

	enum ExpressionEndConditionFlags
	{
		semicolon_end    = 0x01,
		paranthesis_end  = 0x02,
		bracket_end      = 0x04,
		colon_end        = 0x08,
		comma_end        = 0x10
	};

	typedef int ExpressionEndCondition;

	FunctionOrGlobalPreparse preparse_function_or_global();
	void parse_layout(std::vector<GlslAstLayoutQualifierId *> &layout_ids);
	GlslAstType *find_type(const std::string &identifier);
	GlslAstVariable *find_variable(const std::string &identifier);
	GlslAstBuiltInType *parse_built_in_type();
	GlslAstIntegralConstantExpression *parse_array_size();
	GlslAstStructType *parse_struct();
	GlslAstFunction *parse_function(const FunctionOrGlobalPreparse &preparse);
	GlslAstStatement *parse_statement();
	GlslAstCompoundStatement *parse_compound_statement();
	GlslAstIfStatement *parse_if_statement();
	GlslAstSwitchStatement *parse_switch_statement();
	GlslAstCaseLabelStatement *parse_case_label_statement();
	GlslAstForStatement *parse_for_statement();
	GlslAstDoStatement *parse_do_statement();
	GlslAstWhileStatement *parse_while_statement();
	GlslAstContinueStatement *parse_continue_statement();
	GlslAstBreakStatement *parse_break_statement();
	GlslAstDiscardStatement *parse_discard_statement();
	GlslAstReturnStatement *parse_return_statement();
	GlslAstSimpleStatement *parse_declaration_or_expression_statement(ExpressionEndCondition end_condition);
	GlslAstDeclarationStatement *try_parse_declaration_statement(ExpressionEndCondition end_condition);
	GlslAstExpressionStatement *parse_expression_statement(ExpressionEndCondition end_condition);
	GlslAstExpression *parse_expression(ExpressionEndCondition end_condition);
	GlslAstExpression *parse_unary(ExpressionEndCondition end_condition);
	GlslAstExpression *parse_unary_prefix(ExpressionEndCondition end_condition);
	GlslAstExpression *parse_binary(int lhs_precendence, GlslAstExpression *lhs, ExpressionEndCondition end_condition);
	int get_token_precendence();
	GlslAstBinaryExpression *create_token_expression();
	GlslAstFunctionCall *parse_function_call();
	GlslAstConstructorCall *parse_constructor_call();

	void next();
	bool is_type(GlslToken::Type type) const;
	bool is_keyword(GlslToken::Keyword keyword) const;
	bool is_operator(GlslToken::Operator oper) const;
	bool is_built_in_type_keyword() const;
	bool is_end_condition(ExpressionEndCondition end_condition) const;

	GlslAstGarbageCollector *gc() { return ast.get(); }

	GlslTokenizer tokenizer;
	GlslToken token;
	std::map<GlslToken::Keyword, GlslAstBuiltInType *> built_in_types;
	std::vector<VariableScope> variable_scopes;
};
