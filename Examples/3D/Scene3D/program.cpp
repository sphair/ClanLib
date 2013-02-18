
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
	scene.set_camera(camera);

	SceneLight omni(scene);
	omni.set_type(SceneLight::type_omni);
	omni.set_color(Vec3f(0.5f, 0.5f, 0.5f));
	omni.set_position(Vec3f(100.0f, 100.0f, 100.0f));
	omni.set_attenuation_end(200.0f);

	SceneLight spot(scene);
	spot.set_type(SceneLight::type_spot);
	spot.set_position(Vec3f(0.0f, 100.0f, 0.0f));
	spot.set_color(Vec3f(0.7f, 4.0f, 0.7f));
	spot.set_falloff(45.0f);
	spot.set_hotspot(15.0f);
	spot.set_orientation(Quaternionf(90.0f, 0.0f, 0.0f, angle_degrees, order_YXZ));
	spot.set_attenuation_start(50.0f);
	spot.set_attenuation_end(200.0f);
	spot.set_shadow_caster(true);
	spot.set_rectangle_shape(false);
	spot.set_aspect_ratio(1.0f);

	SceneModel model(gc, scene, "plane");
	SceneObject object(scene, model, Vec3f(0.0f, 0.0f, 0.0f));

	ElapsedTimer elapsed_timer;

	float up = 20.0f;
	float tilt = 0.0f;
	float dir = 0.0f;

	while (!exit)
	{
		float time_elapsed = elapsed_timer.seconds_elapsed();

		dir = std::fmod(dir + time_elapsed * 5.0f, 360.0f);

		camera.set_orientation(Quaternionf(up, dir, tilt, angle_degrees, order_YXZ));
		camera.set_position(camera.get_orientation().rotate_vector(Vec3f(0.0f, 0.0f, -100.0f)));

		scene.update(gc, time_elapsed);

		scene.set_viewport(gc.get_size());
		scene.render(gc);
		
		window.flip(1);

		KeepAlive::process();
	}

	return 0;
}
