
#include "precomp.h"
#include "program.h"
#include "Controllers/ChatWindow/chat_window_controller.h"
#include "Models/app_model.h"

using namespace clan;

class DisplayResources : public DisplayCache
{
public:
	Resource<Sprite> get_sprite(Canvas &canvas, const std::string &id) override { throw Exception("No sprite resources"); }
	Resource<Texture> get_texture(GraphicContext &gc, const std::string &id) override { throw Exception("No texture resources"); }

	Resource<Image> get_image(Canvas &canvas, const std::string &id) override
	{
		if (loaded_images.find(id) == loaded_images.end())
		{
			loaded_images[id] = Image(canvas, PathHelp::combine(resource_path, id));
		}
		return loaded_images[id];
	}

	Resource<Font> get_font(Canvas &canvas, const std::string &family_name, const FontDescription &desc) override
	{
		if (loaded_fonts.find(family_name) == loaded_fonts.end())
		{
			if (family_name == "Source Sans Pro")
			{
				FontFamily family(family_name);

				FontDescription normal;
				family.add(normal, PathHelp::combine(resource_path, "SourceSansPro/SourceSansPro-Regular.ttf"));

				FontDescription bold;
				bold.set_weight(FontWeight::bold);
				family.add(bold, PathHelp::combine(resource_path, "SourceSansPro/SourceSansPro-Bold.ttf"));

				FontDescription italic;
				italic.set_style(FontStyle::italic);
				family.add(italic, PathHelp::combine(resource_path, "SourceSansPro/SourceSansPro-Italic.ttf"));

				FontDescription bold_italic;
				bold_italic.set_weight(FontWeight::bold);
				bold_italic.set_style(FontStyle::italic);
				family.add(bold_italic, PathHelp::combine(resource_path, "SourceSansPro/SourceSansPro-BoldItalic.ttf"));

				loaded_fonts[family_name] = family;
			}
			else
			{
				loaded_fonts[family_name] = FontFamily(family_name);
			}
		}
		return Font(loaded_fonts[family_name], desc);
	}


private:
	std::string resource_path = "Resources";
	std::map<std::string, FontFamily > loaded_fonts;
	std::map<std::string, Resource<Image>> loaded_images;
};

int Program::main(const std::vector<std::string> &args)
{
	/*
	#ifndef _DEBUG
	std::string appdata_dir = clan::Directory::get_appdata("ClanLib", "Carambola", "1.0");
	CrashReporter crash_reporter(appdata_dir);

	DetectHang detect_hang;
	#endif
	*/
	OpenGLTarget::enable();

	ResourceManager resources;
	DisplayCache::set(resources, std::make_shared<DisplayResources>());

	UIThread ui_thread(resources);

	AppModel app_model;

	auto chat_window = std::make_shared<ChatWindowViewController>();

	bool exit = false;
	Slot exit_slot = chat_window->window_view()->sig_close().connect([&](CloseEvent &e) { exit = true; });
	while (!exit)
	{
		RunLoop::process(250);
	}

	return 0;
}

Application app(&Program::main);
