
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

	std::vector<SceneLight> omni_lights;
	for (int i = 0; i < 4; i++)
	{
		SceneLight omni(scene);
		omni.set_type(SceneLight::type_omni);
		omni.set_color(Vec3f(0.005f));
		omni.set_position(Quaternionf(45.0f, 45.0f + i * 90.0f, 0.0f, angle_degrees, order_YXZ).rotate_vector(Vec3f(0.0f, 0.0f, -100.0f)));
		omni.set_attenuation_end(200.0f);

		omni_lights.push_back(omni);
	}

	SceneLight spot(scene);
	spot.set_type(SceneLight::type_spot);
	spot.set_orientation(Quaternionf(30.0f, 30.0f, 0.0f, angle_degrees, order_YXZ));
	spot.set_position(spot.get_orientation().rotate_vector(Vec3f(0.0f, 0.0f, -100.0f)));
	spot.set_color(Vec3f(1.0f, 1.0f, 0.2f));
	spot.set_falloff(45.0f);
	spot.set_hotspot(15.0f);
	spot.set_attenuation_start(20.0f);
	spot.set_attenuation_end(200.0f);
	spot.set_shadow_caster(true);
	spot.set_rectangle_shape(false);
	spot.set_aspect_ratio(1.0f);

	SceneLight spot2(scene);
	spot2.set_type(SceneLight::type_spot);
	spot2.set_position(Vec3f(0.0f, 100.0f, 0.0f));
	spot2.set_color(Vec3f(1.0f, 1.0f, 1.0f) * 3.0f);
	spot2.set_falloff(35.0f);
	spot2.set_hotspot(30.0f);
	spot2.set_attenuation_start(20.0f);
	spot2.set_attenuation_end(130.0f);
	spot2.set_shadow_caster(true);
	spot2.set_rectangle_shape(false);

	SceneModel plane(gc, scene, "plane");
	SceneModel box(gc, scene, "box");

	SceneObject object(scene, plane, Vec3f(0.0f, 0.0f, 0.0f));

	SceneObject box0(scene, box, Vec3f(20.0f, 5.0f, 0.0f), Quaternionf(0.0f, 20.0f, 0.0f, angle_degrees, order_YXZ));
	SceneObject box1(scene, box, Vec3f(-20.0f, 5.0f, 0.0f), Quaternionf(0.0f, 50.0f, 0.0f, angle_degrees, order_YXZ));
	SceneObject box2(scene, box, Vec3f(0.0f, 5.0f, 20.0f), Quaternionf(0.0f, 80.0f, 0.0f, angle_degrees, order_YXZ));
	SceneObject box3(scene, box, Vec3f(0.0f, 5.0f, -20.0f), Quaternionf(0.0f, 100.0f, 0.0f, angle_degrees, order_YXZ));

	ElapsedTimer elapsed_timer;

	float up = 20.0f;
	float tilt = 0.0f;
	float dir = 0.0f;

	float spot_dir = 45.0f;
	float aspect_time = 0.0f;

	while (!exit)
	{
		float time_elapsed = elapsed_timer.seconds_elapsed();

		dir = std::fmod(dir + time_elapsed * 5.0f, 360.0f);
		spot_dir = std::fmod(spot_dir + time_elapsed * 30.0f, 90.0f);
		aspect_time = std::fmod(aspect_time + time_elapsed * 0.2f, 2.0f);

		spot2.set_aspect_ratio(clamp(aspect_time >= 1.0f ? 2.0f - aspect_time : aspect_time, 0.1f, 1.0f));
		spot2.set_orientation(Quaternionf(65.0f + (spot_dir >= 45.0f ? 90.0f - spot_dir : spot_dir), 60.0f, dir * 4.0f, angle_degrees, order_YXZ));

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
