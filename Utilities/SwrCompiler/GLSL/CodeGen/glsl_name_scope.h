
#pragma once

#include <map>

namespace llvm { class Value; }
class GlslAstVariable;

class GlslNameScope
{
public:
	std::map<GlslAstVariable *, llvm::Value *> variables;
};
