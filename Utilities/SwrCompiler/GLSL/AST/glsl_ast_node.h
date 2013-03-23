
#pragma once

class GlslAstGarbageCollector;

class GlslAstNode
{
public:
	virtual ~GlslAstNode() { }

	void *operator new(size_t size, GlslAstGarbageCollector *gc);
	void operator delete(void *ptr, GlslAstGarbageCollector *gc);

private:
	void *operator new(size_t size); // do not implement
	void operator delete(void *ptr);
};
