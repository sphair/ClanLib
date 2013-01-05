/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#pragma once

namespace clan
{

class CSSLayoutTreeNode;
class CSSResourceCache;
class CSSLayoutGraphics;

class CSSStackingContext
{
public:
	CSSStackingContext(CSSLayoutTreeNode *layout);
	~CSSStackingContext();
	bool operator <(const CSSStackingContext &other) const;
	int get_level() const { return level; }
	void push_back(CSSStackingContext *child);
	void sort();
	void render(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, bool root = false);

private:
	int level;
	CSSLayoutTreeNode *layout;
	std::vector<CSSStackingContext *> children;
	int tree_order;
	int next_child_tree_order;

	struct SortPred
	{
		bool operator()(CSSStackingContext *a, CSSStackingContext *b)
		{
			return (*a) < (*b);
		}
	};
};

}
