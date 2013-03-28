
#pragma once

#include <vector>
#include "glsl_ast_node.h"
#include "glsl_ast_garbage_collector.h"

class GlslAstFunction;
class GlslAstType;
class GlslAstGlobalVariable;

class GlslAstTranslationUnit : public GlslAstGarbageCollector
{
public:
	GlslAstTranslationUnit() { }

	std::vector<GlslAstFunction *> functions;
	std::vector<GlslAstType *> types;
	std::vector<GlslAstGlobalVariable *> globals;

private:
	GlslAstTranslationUnit(const GlslAstTranslationUnit &other); // Do not implement
	GlslAstTranslationUnit &operator=(const GlslAstTranslationUnit &other); // Do not implement
};
