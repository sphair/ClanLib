/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include <memory>

namespace clan
{
/// \addtogroup clanCSSLayout_Layout clanCSSLayout Layout
/// \{

class CSSBoxElement;
class CSSLayoutObject;
class CSSLayoutElement;
class CSSLayoutText;
class CSSLayoutNode;
class CSSHitTestResult;
class CSSDocument;
class CSSLayout_Impl;
class GraphicContext;
class Size;
class Point;
class Image;
class Rect;
class Canvas;

class CSSLayout
{
public:
	CSSLayout();

	bool is_null() const;

	void set_dpi(float new_dpi);

	void set_css_document(const CSSDocument &doc);

	void layout(Canvas &canvas, const Rect &viewport);
	void render(Canvas &canvas) { render_impl(canvas); }

	template<typename GUIElement>
	void render(Canvas &canvas, GUIElement *component)
	{
		render_impl(canvas, std::unique_ptr<ClipWrapper>(new GUIElementWrapper<GUIElement>(component)));
	}

	CSSHitTestResult hit_test(Canvas &canvas, const Point &pos);

	void set_document_element(CSSLayoutElement element);
	CSSLayoutElement get_document_element();

	CSSLayoutObject create_object();
	CSSLayoutElement create_element(const std::string &name = std::string());
	CSSLayoutText create_text(const std::string &text);

	CSSLayoutElement find_element(const std::string &name);

	// Image on_get_image(Canvas &canvas, const std::string &uri);
	std::function<Image(Canvas &, const std::string &)> &func_get_image();

	class ClipWrapper
	{
	public:
		virtual ~ClipWrapper() { }
		virtual void set_cliprect(Canvas &canvas, const Rect &rect) = 0;
		virtual void reset_cliprect(Canvas &canvas) = 0;
		virtual void push_cliprect(Canvas &canvas, const Rect &rect) = 0;
		virtual void pop_cliprect(Canvas &canvas) = 0;
	};

private:
	void render_impl(Canvas &canvas, std::unique_ptr<ClipWrapper> wrapper = std::unique_ptr<ClipWrapper>());

	template<typename GUIElement>
	class GUIElementWrapper : public ClipWrapper
	{
	public:
		GUIElementWrapper(GUIElement *component) : component(component) { }
		void set_cliprect(Canvas &canvas, const Rect &rect) { component->set_cliprect(canvas, rect); }
		void reset_cliprect(Canvas &canvas) { component->reset_cliprect(canvas); }
		void push_cliprect(Canvas &canvas, const Rect &rect) { component->push_cliprect(canvas, rect); }
		void pop_cliprect(Canvas &canvas) { component->pop_cliprect(canvas); }

	private:
		GUIElement *component;
	};

	CSSLayout(std::shared_ptr<CSSLayout_Impl> impl);
	std::shared_ptr<CSSLayout_Impl> impl;
	friend class CSSLayout_Impl;
};

/// \}
}
