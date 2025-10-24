/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "api_csslayout.h"
#include "../Core/Signals/callback_2.h"
#include "../Core/System/uniqueptr.h"
#include <memory>

class CL_CSSBoxElement;
class CL_GraphicContext;
class CL_CSSLayoutObject;
class CL_CSSLayoutElement;
class CL_CSSLayoutText;
class CL_CSSLayoutNode;
class CL_CSSHitTestResult;
class CL_CSSLayout_Impl;
class CL_Size;
class CL_Point;
class CL_Image;
class CL_Rect;

class CL_API_CSSLAYOUT CL_CSSLayout
{
public:
	CL_CSSLayout();

	bool is_null() const;

	void load_xml(const CL_String &filename, const CL_String &style_sheet);
	void layout(CL_GraphicContext &gc, const CL_Rect &viewport);
	void render(CL_GraphicContext &gc) { render_impl(gc); }

	template<typename GUIComponent>
	void render(CL_GraphicContext &gc, GUIComponent *component)
	{
		render_impl(gc, CL_UniquePtr<ClipWrapper>(new GUIComponentWrapper<GUIComponent>(component)));
	}

	CL_CSSHitTestResult hit_test(CL_GraphicContext &gc, const CL_Point &pos);
	void clear_selection();
	void set_selection(CL_CSSLayoutNode start, size_t start_text_offset, CL_CSSLayoutNode end, size_t end_text_offset);

	void clear();
	void set_root_element(CL_CSSLayoutElement element);
	CL_CSSLayoutElement get_root_element();

	void set_html_body_element(CL_CSSLayoutElement element);
	CL_CSSLayoutElement get_html_body_element();

	CL_CSSLayoutObject create_object();
	CL_CSSLayoutElement create_element(const CL_String &name = CL_String());
	CL_CSSLayoutText create_text(const CL_String &text);

	CL_CSSLayoutElement find_element(const CL_String &name);

	// CL_Image on_get_image(CL_GraphicContext &gc, const CL_String &uri);
	CL_Callback_2<CL_Image, CL_GraphicContext &, const CL_String &> &func_get_image();

	class ClipWrapper
	{
	public:
		virtual ~ClipWrapper() { }
		virtual void set_cliprect(CL_GraphicContext &gc, const CL_Rect &rect) = 0;
		virtual void reset_cliprect(CL_GraphicContext &gc) = 0;
		virtual void push_cliprect(CL_GraphicContext &gc, const CL_Rect &rect) = 0;
		virtual void pop_cliprect(CL_GraphicContext &gc) = 0;
	};

private:
	void render_impl(CL_GraphicContext &gc, CL_UniquePtr<ClipWrapper> wrapper = CL_UniquePtr<ClipWrapper>());

	template<typename GUIComponent>
	class GUIComponentWrapper : public ClipWrapper
	{
	public:
		GUIComponentWrapper(GUIComponent *component) : component(component) { }
		void set_cliprect(CL_GraphicContext &gc, const CL_Rect &rect) { component->set_cliprect(gc, rect); }
		void reset_cliprect(CL_GraphicContext &gc) { component->reset_cliprect(gc); }
		void push_cliprect(CL_GraphicContext &gc, const CL_Rect &rect) { component->push_cliprect(gc, rect); }
		void pop_cliprect(CL_GraphicContext &gc) { component->pop_cliprect(gc); }

	private:
		GUIComponent *component;
	};

	CL_CSSLayout(CL_SharedPtr<CL_CSSLayout_Impl> impl);
	CL_SharedPtr<CL_CSSLayout_Impl> impl;
	friend class CL_CSSLayout_Impl;
};
