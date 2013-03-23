
#pragma once

#include <vector>

class GlslAstNode;

class GlslAstGarbageCollector
{
public:
	~GlslAstGarbageCollector();

private:
	std::vector<GlslAstNode *> gc_nodes;
	friend class GlslAstNode;
};
