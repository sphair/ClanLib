
#include "precomp.h"
#include "program.h"
#include "elapsed_timer.h"
#include "cache_provider.h"
#include <ClanLib/application.h>

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupD3D setup_d3d;

	DisplayWindow window("Scene3D Example", 1600, 900, false, true);

	GraphicContext gc = window.get_gc();

	bool exit = false;
	Slot slot_close = window.sig_window_close().connect_functor([&exit]() { exit = true; });

	SceneCache cache(new ExampleSceneCacheProvider());
	std::string shader_path = "../../../Resources/Scene3D";

	Scene scene(gc, cache, shader_path);

	SceneCamera camera(scene);
	camera.set_position(Vec3f(0.0f, 70.0f, -70.0f));
	camera.set_orientation(Quaternionf(52.0f, 0.0f, 0.0f, angle_degrees, order_YXZ));
	scene.set_camera(camera);

	SceneLight light(scene);
	light.set_position(Vec3f(100.0f, 100.0f, 100.0f));
	light.set_type(SceneLight::type_omni);
	light.set_attenuation_end(200.0f);

	SceneModel model(gc, scene, "plane");
	SceneObject object(scene, model, Vec3f(0.0f, 0.0f, 0.0f));

	ElapsedTimer elapsed_timer;

	while (!exit)
	{
		scene.update(gc, elapsed_timer.seconds_elapsed());

		scene.set_viewport(window.get_viewport());
		scene.render(gc);
		
		window.flip(1);

		KeepAlive::process();
	}

	return 0;
}
