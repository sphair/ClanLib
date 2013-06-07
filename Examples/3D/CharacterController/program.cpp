
#include "precomp.h"
#include "program.h"
#include "elapsed_timer.h"
#include "example_scene_cache.h"
#include "character_controller.h"
#include <ClanLib/application.h>

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	//SetupD3D setup_d3d;
	SetupGL setup_gl;

	clan::OpenGLWindowDescription opengl_desc;
	opengl_desc.set_version(3, 2, false);
	clan::OpenGLTarget::set_description(opengl_desc);

	DisplayWindow window("Scene3D Example", 1600, 900, false, true);

	GraphicContext gc = window.get_gc();

	bool exit = false;
	Slot slot_close = window.sig_window_close().connect_functor([&exit]() { exit = true; });

	ResourceManager resources;
	SceneCache::set(resources, std::shared_ptr<SceneCache>(new ExampleSceneCache()));

	std::string shader_path = "../../../Resources/Scene3D";

	Scene scene(gc, resources, shader_path);

	SceneCamera camera(scene);
	scene.set_camera(camera);

	std::vector<SceneLight> omni_lights;
	for (int i = 0; i < 4; i++)
	{
		SceneLight omni(scene);
		omni.set_type(SceneLight::type_omni);
		omni.set_color(Vec3f(0.05f));
		omni.set_position(Quaternionf(45.0f, 45.0f + i * 90.0f, 0.0f, angle_degrees, order_YXZ).rotate_vector(Vec3f(0.0f, 0.0f, -100.0f)));
		omni.set_attenuation_end(200.0f);
		omni.set_ambient_illumination(0.025f);

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
	spot2.set_ambient_illumination(0.025f);

	SceneModel plane(gc, scene, "plane");
	SceneModel box(gc, scene, "box");

	SceneObject object(scene, plane, Vec3f(0.0f, 0.0f, 0.0f));

	SceneObject box0(scene, box, Vec3f(20.0f, 5.0f, 0.0f), Quaternionf(0.0f, 20.0f, 0.0f, angle_degrees, order_YXZ));
	SceneObject box1(scene, box, Vec3f(-20.0f, 5.0f, 0.0f), Quaternionf(0.0f, 50.0f, 0.0f, angle_degrees, order_YXZ));
	SceneObject box2(scene, box, Vec3f(0.0f, 5.0f, 20.0f), Quaternionf(0.0f, 80.0f, 0.0f, angle_degrees, order_YXZ));
	SceneObject box3(scene, box, Vec3f(0.0f, 5.0f, -20.0f), Quaternionf(0.0f, 100.0f, 0.0f, angle_degrees, order_YXZ));

	SceneObject player(scene, box);
	player.set_scale(Vec3f(0.25f));

	Physics3DWorld physics_world;
	
	Physics3DShape box_shape = Physics3DShape::box(Vec3f(5.0f));
	Physics3DShape plane_shape = Physics3DShape::box(Vec3f(75.0f, 1.0f, 75.0f));
	Physics3DShape sphere_shape = Physics3DShape::sphere(2.0f);

	Physics3DObject phys_object(physics_world, plane_shape, Vec3f(0.0f, -0.5f, 0.0f));

	Physics3DObject phys_box0(physics_world, box_shape, Vec3f(20.0f, 5.0f, 0.0f), Quaternionf(0.0f, 20.0f, 0.0f, angle_degrees, order_YXZ));
	Physics3DObject phys_box1(physics_world, box_shape, Vec3f(-20.0f, 5.0f, 0.0f), Quaternionf(0.0f, 50.0f, 0.0f, angle_degrees, order_YXZ));
	Physics3DObject phys_box2(physics_world, box_shape, Vec3f(0.0f, 5.0f, 20.0f), Quaternionf(0.0f, 80.0f, 0.0f, angle_degrees, order_YXZ));
	Physics3DObject phys_box3(physics_world, box_shape, Vec3f(0.0f, 5.0f, -20.0f), Quaternionf(0.0f, 100.0f, 0.0f, angle_degrees, order_YXZ));

	Physics3DSweepTest sweep_test(physics_world);

	CharacterController controller(physics_world, 1.0f, 2.89f, 0.5f, 1.0f / 60.0f);
	controller.set_position(Vec3f(0.0f, 100.0f, 0.0f));

	ElapsedTimer elapsed_timer;

	float up = 20.0f;
	float tilt = 0.0f;
	float dir = 0.0f;

	float spot_dir = 45.0f;
	float aspect_time = 0.0f;

	while (!exit)
	{
		float time_elapsed = elapsed_timer.seconds_elapsed();

		// Animate lights:

		spot_dir = std::fmod(spot_dir + time_elapsed * 30.0f, 90.0f);
		aspect_time = std::fmod(aspect_time + time_elapsed * 0.2f, 2.0f);

		spot2.set_aspect_ratio(clamp(aspect_time >= 1.0f ? 2.0f - aspect_time : aspect_time, 0.1f, 1.0f));
		spot2.set_orientation(Quaternionf(65.0f + (spot_dir >= 45.0f ? 90.0f - spot_dir : spot_dir), 60.0f, dir * 4.0f, angle_degrees, order_YXZ));

		// Update camera orientation:

		float dir_speed = 50.0f;
		if (window.get_ic().get_keyboard().get_keycode(keycode_left))
		{
			dir = std::fmod(dir - time_elapsed * dir_speed, 360.0f);
		}
		else if (window.get_ic().get_keyboard().get_keycode(keycode_right))
		{
			dir = std::fmod(dir + time_elapsed * dir_speed, 360.0f);
		}

		float up_speed = 50.0f;
		if (window.get_ic().get_keyboard().get_keycode(keycode_up))
		{
			up = clamp(up - time_elapsed * up_speed, -90.0f, 90.0f);
		}
		else if (window.get_ic().get_keyboard().get_keycode(keycode_down))
		{
			up = clamp(up + time_elapsed * up_speed, -90.0f, 90.0f);
		}

		// Move our character controller:

		Quaternionf player_orientation(0.0f, dir, 0.0f, angle_degrees, order_YXZ);

		Vec3f move_velocity;
		if (window.get_ic().get_keyboard().get_keycode(keycode_w))
		{
			move_velocity = player_orientation.rotate_vector(Vec3f(0.0f, 0.0f, 1.0f));
		}
		else if (window.get_ic().get_keyboard().get_keycode(keycode_s))
		{
			move_velocity = player_orientation.rotate_vector(Vec3f(0.0f, 0.0f, -1.0f));
		}

		controller.move(move_velocity);

		if (window.get_ic().get_keyboard().get_keycode(keycode_space))
		{
			controller.jump(move_velocity * 0.40f + Vec3f(0.0f, 0.7f, 0.0f));
		}

		// Update scene player object:

		player.set_position(controller.get_position() + Vec3f(0.0f, 1.25f, 0.0f));
		player.set_orientation(player_orientation);

		// Check for collision between camera and scene:

		Quaternionf camera_orientation(up, dir, tilt, angle_degrees, order_YXZ);

		float zoom_out = 25.0f;
		Vec3f camera_look_pos = controller.get_position();
		Vec3f camera_pos = camera_look_pos + camera_orientation.rotate_vector(Vec3f(0.0f, 0.0f, -zoom_out));

		if (sweep_test.test_first_hit(sphere_shape, camera_look_pos, Quaternionf(), camera_pos, Quaternionf()))
		{
			camera_pos = sweep_test.get_hit_position(0);
		}

		// Update scene camera:

		camera.set_orientation(camera_orientation);
		camera.set_position(camera_pos);

		// Render scene:

		scene.update(gc, time_elapsed);

		scene.set_viewport(gc.get_size());
		scene.render(gc);

		// Show result:
		
		window.flip(1);

		KeepAlive::process();
	}

	return 0;
}
