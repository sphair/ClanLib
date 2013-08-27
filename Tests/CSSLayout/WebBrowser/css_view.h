
#pragma once

#include "html_page.h"

class clan::CSSLayout;
class clan::CSSBoxElement;

class CSSView : public clan::GUIComponent
{
public:
	CSSView(clan::GUIComponent *parent);
	~CSSView();

	void navigate(const std::string &page_url, const std::string &refererer_url = std::string());

private:
	void on_resized();
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	void on_scroll();
	clan::Image on_layout_get_image(clan::Canvas &canvas, const std::string &uri);

	void reset();
	void load_html();
	bool is_end_tag_forbidden(const std::string &name);

	class ReplacedObject
	{
	public:
		virtual ~ReplacedObject() { }
		virtual bool has_intrinsic_values() const { return true; }
		virtual int get_intrinsic_width() const = 0;
		virtual int get_intrinsic_height() const = 0;
		virtual bool has_intrinsic_ratio() const { return false; }
		virtual float get_intrinsic_ratio() const { return get_intrinsic_width()/(float)get_intrinsic_height(); }
		virtual void set_geometry(const clan::Rect &new_geometry) { }
		virtual void render(clan::Canvas &canvas) { }
	};

	class Img : public ReplacedObject
	{
	public:
		Img(clan::Image image) : image(image) { }
		bool has_intrinsic_values() const { return !image.is_null(); }
		int get_intrinsic_width() const { return image.get_width(); }
		int get_intrinsic_height() const { return image.get_height(); }
		bool has_intrinsic_ratio() const { return true; }
		float get_intrinsic_ratio() const { return image.get_height()/(float)image.get_width(); }
		void set_geometry(const clan::Rect &new_geometry) { geometry = new_geometry; }
		void render(clan::Canvas &canvas) { if (!image.is_null()) image.draw(canvas, geometry); }

		clan::Image image;
		clan::Rect geometry;
	};

	class TextInput : public ReplacedObject
	{
	public:
		TextInput(clan::Canvas &canvas, int num_chars) : num_chars(num_chars) { }
		int get_intrinsic_width() const { return num_chars * 5; }
		int get_intrinsic_height() const { return 18; }
		void set_geometry(const clan::Rect &new_geometry) { geometry = new_geometry; }
		void render(clan::Canvas &canvas) { canvas.fill_rect(geometry, clan::Colorf::greenyellow); }

		int num_chars;
		clan::Rect geometry;
	};

	class CheckboxInput : public ReplacedObject
	{
	public:
		CheckboxInput(clan::Canvas &canvas) { }
		int get_intrinsic_width() const { return 16; }
		int get_intrinsic_height() const { return 16; }
		void set_geometry(const clan::Rect &new_geometry) { geometry = new_geometry; }
		void render(clan::Canvas &canvas) { canvas.fill_rect(geometry, clan::Colorf::greenyellow); }

		clan::Rect geometry;
	};

	class RadioInput : public ReplacedObject
	{
	public:
		RadioInput(clan::Canvas &canvas) { }
		int get_intrinsic_width() const { return 16; }
		int get_intrinsic_height() const { return 16; }
		void set_geometry(const clan::Rect &new_geometry) { geometry = new_geometry; }
		void render(clan::Canvas &canvas) { canvas.fill_rect(geometry, clan::Colorf::greenyellow); }

		clan::Rect geometry;
	};

	class ButtonInput : public ReplacedObject
	{
	public:
		ButtonInput(clan::Canvas &canvas, const std::string &text) : text(text) { }
		int get_intrinsic_width() const { return text.length() * 5; }
		int get_intrinsic_height() const { return 24; }
		void set_geometry(const clan::Rect &new_geometry) { geometry = new_geometry; }
		void render(clan::Canvas &canvas) { canvas.fill_rect(geometry, clan::Colorf::greenyellow); }

		std::string text;
		clan::Rect geometry;
	};

	HTMLPage page;
	clan::CSSLayout layout;
	clan::Rect last_layout_viewport;
	clan::CSSDocument css_document;
	clan::ScrollBar *scrollbar;

	clan::Rect client_box;
	clan::Rect scrollbar_box;
	clan::Rect view_box;
	std::map<std::string, clan::Image> image_cache;
	std::vector<ReplacedObject *> replaced_objects;
};
