
#include "precomp.h"
#include "program.h"
#include "fbx_scene_cache.h"
#include <ClanLib/application.h>

#pragma comment(lib, "libfbxsdk-mt.lib")

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupD3D setup_d3d;

	ResourceManager resources;
	SceneCache::set(resources, std::shared_ptr<SceneCache>(new FBXSceneCache()));

	DisplayWindow window("FBX Viewer", 1024, 768, false, true);
	InputContext ic = window.get_ic();
	Canvas canvas(window);
	GraphicContext gc = canvas.get_gc();

	bool exit = false;
	Slot slot_window_close = window.sig_window_close().connect_functor([&exit]() { exit = true; });

	Scene scene(gc, resources, "../../Resources/Scene3D");

	scene.show_skybox_stars(false);
	std::vector<Colorf> gradient;
	gradient.push_back(Colorf::gray10);
	gradient.push_back(Colorf::gray20);
	gradient.push_back(Colorf::gray30);
	gradient.push_back(Colorf::gray20);
	gradient.push_back(Colorf::gray10);
	scene.set_skybox_gradient(gc, gradient);

	SceneLight light1(scene);
	light1.set_position(Vec3f(100.0f, 100.0f, 100.0f));
	light1.set_type(SceneLight::type_omni);
	light1.set_attenuation_start(900.0f);
	light1.set_attenuation_end(1000.0f);
	light1.set_color(Vec3f(0.8f));

	SceneLight light2(scene);
	light2.set_position(Vec3f(-100.0f, -100.0f, -100.0f));
	light2.set_type(SceneLight::type_omni);
	light2.set_attenuation_start(900.0f);
	light2.set_attenuation_end(1000.0f);
	light2.set_color(Vec3f(0.4f));

	SceneModel model1(gc, scene, "Models/HammerTime/hammer_time.fbx");
	SceneModel model2(gc, scene, "Models/YellowPowerup/yellow_powerup.fbx");
	SceneModel model3(gc, scene, "Models/TestCube/test_cube.fbx");

	SceneObject object1(scene, model1, Vec3f(30.0f, 30.0f, 30.0f), Quaternionf(), Vec3f(10.0f));
	SceneObject object2(scene, model2, Vec3f(), Quaternionf(), Vec3f(10.0f));
	SceneObject object3(scene, model3, Vec3f(-30.0f, 30.0f, 30.0f), Quaternionf(), Vec3f(10.0f));

	SceneCamera camera(scene);
	scene.set_camera(camera);

	GameTime gametime;

	float dir = 0.0f;
	float up = 0.0f;
	float turn_speed = 90.0f;

	while (!exit)
	{
		KeepAlive::process();

		gametime.update();

		if (ic.get_keyboard().get_keycode(keycode_left))
			dir = std::fmod(dir - turn_speed * gametime.get_time_elapsed(), 360.0f);
		else if (ic.get_keyboard().get_keycode(keycode_right))
			dir = std::fmod(dir + turn_speed * gametime.get_time_elapsed(), 360.0f);

		if (ic.get_keyboard().get_keycode(keycode_up))
			up = clamp(up - turn_speed * gametime.get_time_elapsed(), -90.0f, 90.0f);
		else if (ic.get_keyboard().get_keycode(keycode_down))
			up = clamp(up + turn_speed * gametime.get_time_elapsed(), -90.0f, 90.0f);

		camera.set_orientation(Quaternionf(up, dir, 0.0f, angle_degrees, order_YXZ));
		camera.set_position(camera.get_orientation().rotate_vector(Vec3f(0.0f, 0.0f, -50.0f)));

		object1.update(gametime.get_time_elapsed());
		object2.update(gametime.get_time_elapsed());
		object3.update(gametime.get_time_elapsed());
		scene.update(gc, gametime.get_time_elapsed());

		scene.set_viewport(gc.get_size());
		scene.render(gc);

		window.flip(1);
	}

	return 0;
}
