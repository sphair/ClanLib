
#include "precomp.h"
#include "welcome_page.h"

using namespace clan;

WelcomePage::WelcomePage()
: welcome(0)
{
	set_title("Welcome");
	set_dockable_window_geometry(Rect(Point(50, 50), Size(300, 400)));

	//welcome = new Label(this);
	logo = new ImageView(this);
	logo->set_scale_to_fit(false);

	//welcome->set_text("Start a new project to get started.");

	ResourceManager resources = get_resources();
	logo->set_sprite(Sprite(get_canvas(), "GameIdeLogo", &resources));
}

void WelcomePage::on_resized()
{
	Rect client_box = get_size();
	client_box.shrink(5);
	//welcome->set_geometry(client_box);
	logo->set_geometry(client_box);
}
