
#pragma once

#include "html_page.h"

class CL_CSSLayout;
class CL_CSSBoxElement;

class CSSView : public CL_GUIComponent
{
public:
	CSSView(CL_GUIComponent *parent);
	~CSSView();

	void navigate(const CL_String &page_url, const CL_String &refererer_url = CL_String());

private:
	void on_resized();
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_scroll();
	CL_Image on_layout_get_image(CL_GraphicContext &gc, const CL_String &uri);

	void reset();
	void load_html();
	bool is_end_tag_forbidden(const CL_String &name);

	class ReplacedObject
	{
	public:
		virtual ~ReplacedObject() { }
		virtual bool has_intrinsic_values() const { return true; }
		virtual int get_intrinsic_width() const = 0;
		virtual int get_intrinsic_height() const = 0;
		virtual bool has_intrinsic_ratio() const { return false; }
		virtual float get_intrinsic_ratio() const { return get_intrinsic_width()/(float)get_intrinsic_height(); }
		virtual void set_geometry(const CL_Rect &new_geometry) { }
		virtual void render(CL_GraphicContext &gc) { }
	};

	class Image : public ReplacedObject
	{
	public:
		Image(CL_Image image) : image(image) { }
		bool has_intrinsic_values() const { return !image.is_null(); }
		int get_intrinsic_width() const { return image.get_width(); }
		int get_intrinsic_height() const { return image.get_height(); }
		bool has_intrinsic_ratio() const { return true; }
		float get_intrinsic_ratio() const { return image.get_height()/(float)image.get_width(); }
		void set_geometry(const CL_Rect &new_geometry) { geometry = new_geometry; }
		void render(CL_GraphicContext &gc) { if (!image.is_null()) image.draw(gc, geometry); }

		CL_Image image;
		CL_Rect geometry;
	};

	class TextInput : public ReplacedObject
	{
	public:
		TextInput(CL_GraphicContext &gc, int num_chars) : num_chars(num_chars) { }
		int get_intrinsic_width() const { return num_chars * 5; }
		int get_intrinsic_height() const { return 18; }
		void set_geometry(const CL_Rect &new_geometry) { geometry = new_geometry; }
		void render(CL_GraphicContext &gc) { CL_Draw::box(gc, geometry, CL_Colorf::greenyellow); }

		int num_chars;
		CL_Rect geometry;
	};

	class CheckboxInput : public ReplacedObject
	{
	public:
		CheckboxInput(CL_GraphicContext &gc) { }
		int get_intrinsic_width() const { return 16; }
		int get_intrinsic_height() const { return 16; }
		void set_geometry(const CL_Rect &new_geometry) { geometry = new_geometry; }
		void render(CL_GraphicContext &gc) { CL_Draw::box(gc, geometry, CL_Colorf::greenyellow); }

		CL_Rect geometry;
	};

	class RadioInput : public ReplacedObject
	{
	public:
		RadioInput(CL_GraphicContext &gc) { }
		int get_intrinsic_width() const { return 16; }
		int get_intrinsic_height() const { return 16; }
		void set_geometry(const CL_Rect &new_geometry) { geometry = new_geometry; }
		void render(CL_GraphicContext &gc) { CL_Draw::box(gc, geometry, CL_Colorf::greenyellow); }

		CL_Rect geometry;
	};

	class ButtonInput : public ReplacedObject
	{
	public:
		ButtonInput(CL_GraphicContext &gc, const CL_String &text) : text(text) { }
		int get_intrinsic_width() const { return text.length() * 5; }
		int get_intrinsic_height() const { return 24; }
		void set_geometry(const CL_Rect &new_geometry) { geometry = new_geometry; }
		void render(CL_GraphicContext &gc) { CL_Draw::box(gc, geometry, CL_Colorf::greenyellow); }

		CL_String text;
		CL_Rect geometry;
	};

	HTMLPage page;
	CL_CSSLayout layout;
	CL_Rect last_layout_viewport;
	CL_CSSDocument2 css_document;
	CL_ScrollBar *scrollbar;

	CL_Rect client_box;
	CL_Rect scrollbar_box;
	CL_Rect view_box;
	std::map<CL_String, CL_Image> image_cache;
	std::vector<ReplacedObject *> replaced_objects;
};
