
#pragma once

#include "glsl_ast_node.h"
#include "glsl_expression_visitor.h"

class GlslAstExpression : public GlslAstNode
{
public:
	virtual void codegen(GlslExpressionVisitor *visitor) = 0;
};

class GlslAstIntConstant : public GlslAstExpression
{
public:
	GlslAstIntConstant() : value() { }
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	int value;
};

class GlslAstUIntConstant : public GlslAstExpression
{
public:
	GlslAstUIntConstant() : value() { }
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	unsigned int value;
};

class GlslAstFloatConstant : public GlslAstExpression
{
public:
	GlslAstFloatConstant() : value() { }
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	float value;
};

class GlslAstDoubleConstant : public GlslAstExpression
{
public:
	GlslAstDoubleConstant() : value() { }
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	double value;
};

class GlslAstBoolConstant : public GlslAstExpression
{
public:
	GlslAstBoolConstant() : value() { }
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	bool value;
};

class GlslAstVariableIdentifer : public GlslAstExpression
{
public:
	GlslAstVariableIdentifer() : variable() { }
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	GlslAstVariable *variable;
};

class GlslAstFieldSelectorOrSwizzle : public GlslAstExpression
{
public:
	GlslAstFieldSelectorOrSwizzle() : operand() { }
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	GlslAstExpression *operand;
	std::string name;
};

class GlslAstArraySubscript : public GlslAstExpression
{
public:
	GlslAstArraySubscript() : operand(), index() { }
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	GlslAstExpression *operand;
	GlslAstExpression *index;
};

class GlslAstFunctionCall : public GlslAstExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	std::string name;
	std::vector<GlslAstExpression *> parameters;
};

class GlslAstConstructorCall : public GlslAstExpression
{
public:
	GlslAstConstructorCall() : type() { }
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	GlslAstType *type;
	std::vector<GlslAstExpression *> parameters;
};

class GlslAstUnaryExpression : public GlslAstExpression
{
public:
	GlslAstUnaryExpression() : operand() { }

	GlslAstExpression *operand;
};

class GlslAstBinaryExpression : public GlslAstExpression
{
public:
	GlslAstBinaryExpression() : operand1(), operand2() { }

	GlslAstExpression *operand1;
	GlslAstExpression *operand2;
};

class GlslAstTrinaryExpression : public GlslAstExpression
{
public:
	GlslAstTrinaryExpression() : operand1(), operand2(), operand3() { }

	GlslAstExpression *operand1;
	GlslAstExpression *operand2;
	GlslAstExpression *operand3;
};

class GlslAstUnaryPrefixIncrementExpression : public GlslAstUnaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstUnaryPrefixDecrementExpression : public GlslAstUnaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstUnaryPostfixIncrementExpression : public GlslAstUnaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstUnaryPostfixDecrementExpression : public GlslAstUnaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstUnaryPlusExpression : public GlslAstUnaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstUnaryMinusExpression : public GlslAstUnaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstUnaryBitNotExpression : public GlslAstUnaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstUnaryLogicalNotExpression : public GlslAstUnaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstAssignmentExpression : public GlslAstBinaryExpression
{
public:
	GlslAstAssignmentExpression() : assignment_type() { }
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}

	GlslToken::Operator assignment_type;
};

class GlslAstPlusExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstMinusExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstMultiplyExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstDivideExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstModulusExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstShiftLeftExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstShiftRightExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstLessExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstLessEqualExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstGreaterExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstGreaterEqualExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstEqualExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstNotEqualExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstBitAndExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstBitXorExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstBitOrExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstLogicalAndExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstLogicalXorExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstLogicalOrExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstSelectExpression : public GlslAstTrinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};

class GlslAstSequenceExpression : public GlslAstBinaryExpression
{
public:
	void codegen(GlslExpressionVisitor *visitor)
	{
		visitor->expression(this);
	}
};
