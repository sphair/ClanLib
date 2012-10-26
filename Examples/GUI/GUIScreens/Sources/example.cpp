
#include "precomp.h"
#include "example.h"
#include "window_manager.h"
#include "game_screen.h"
#include "splash_screen.h"
#include "error_screen.h"
#include "postprocess_scene.h"
#include "postprocess_effect.h"

int Example::exec()
{
	CL_DisplayWindow display_window("ClanLib GUIScreens Example", 1024, 768);
	slots.connect(display_window.sig_window_close(), this, &Example::on_window_close);

	PostProcessScene post_process_scene(display_window);
	CL_GUIWindowManager wm = WindowManager::create(&post_process_scene);
	gui = CL_GUIManager(wm, "../../../Resources/GUIThemeAeroPacked");

	GameScreen game_screen(&gui);
	game_screen.set_geometry(CL_Rect(0,0,1024,768));
	game_screen.set_visible(true);

/*	effect_transparency = new PostProcessEffectTransparency(&post_process_scene);
	effect_darken = new PostProcessEffectDarken(&post_process_scene);

	splash_screen = new SplashScreen(&gui);
	splash_screen->set_geometry(CL_Rect(0,0,1024,768));
	splash_screen->set_visible(true);

	error_screen = new ErrorScreen(&gui);
	error_screen->set_geometry(CL_Rect(200,200,824,568));
	error_screen->set_visible(false);

	timer.func_expired().set(this, &Example::on_timer);
	timer.start(50);
*/
	return gui.exec();
}

void Example::on_window_close()
{
	gui.exit_with_code(0);
}

void Example::on_timer()
{
	static float total_time = 0;
	static bool first = true;
	total_time += 50;

	if(total_time > 2000)
	{
		if(first)
		{
			splash_screen->set_postprocess_effect(effect_darken);

			error_screen->set_visible(true);
			error_screen->set_postprocess_effect(effect_transparency);
			effect_transparency->set_transparency(0.0f);

			first = false;
		}

		effect_transparency->set_transparency(effect_transparency->get_transparency() + 0.01f);
	}
}
