
#include "precomp.h"
#include "glsl_ast_garbage_collector.h"
#include "glsl_ast_node.h"
#include <exception>
#include <algorithm>

using namespace clan;

GlslAstGarbageCollector::~GlslAstGarbageCollector()
{
	for (size_t i = 0; i < gc_nodes.size(); i++)
	{
		gc_nodes[i]->~GlslAstNode();
		free(gc_nodes[i]);
		gc_nodes[i] = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////

void *GlslAstNode::operator new(size_t size, GlslAstGarbageCollector *gc)
{
	void *ptr = malloc(size);
	gc->gc_nodes.push_back(static_cast<GlslAstNode*>(ptr));
	return ptr;
}

void GlslAstNode::operator delete(void *ptr, GlslAstGarbageCollector *gc)
{
	std::vector<GlslAstNode *>::iterator it = std::find(gc->gc_nodes.begin(), gc->gc_nodes.end(), ptr);
	if (it != gc->gc_nodes.end())
		gc->gc_nodes.erase(it);
	free(ptr);
}

void GlslAstNode::operator delete(void *ptr)
{
	std::terminate();
}
