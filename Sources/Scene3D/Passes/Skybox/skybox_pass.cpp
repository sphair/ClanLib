/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "Scene3D/precomp.h"
#include "skybox_pass.h"
#include "API/Scene3D/scene.h"
#include "noise_2d.h"
#include "noise_3d.h"
#include "Scene3D/scene_impl.h"

namespace clan
{

SkyboxPass::SkyboxPass()
{
}

void SkyboxPass::run(GraphicContext &gc, Scene_Impl *scene)
{
	setup(gc);

	gc.set_frame_buffer(fb);

	Size viewport_size = viewport->get_size();
	gc.set_viewport(viewport_size);

	gc.set_depth_stencil_state(depth_stencil_state);
	gc.set_blend_state(blend_state);
	gc.set_rasterizer_state(rasterizer_state);

	Mat4f eye_to_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 1.e4f, handed_left, gc.get_clip_z_range());

	Uniforms cpu_uniforms;
	cpu_uniforms.eye_to_projection = eye_to_projection;
	cpu_uniforms.object_to_eye = Quaternionf::inverse(scene->get_camera().get_orientation()).to_matrix();
	uniforms.upload_data(gc, &cpu_uniforms, 1);

	gc.set_depth_range(0.9f, 1.0f);

	gc.set_program_object(billboard_program);
	gc.set_primitives_array(billboard_prim_array);
	gc.set_uniform_buffer(0, uniforms);
	gc.set_texture(0, star_instance_texture);
	gc.set_texture(1, star_texture);
	gc.draw_primitives_array_instanced(type_triangles, 0, 6, num_star_instances);
	gc.reset_primitives_array();

	gc.set_program_object(cube_program);
	gc.set_primitives_array(cube_prim_array);
	gc.set_uniform_buffer(0, uniforms);
	gc.set_texture(0, cloud_texture);
	gc.draw_primitives_array(type_triangles, 0, 6*6);
	gc.reset_primitives_array();

	gc.set_depth_range(0.0f, 1.0f);

	gc.reset_blend_state();
	gc.reset_rasterizer_state();
	gc.reset_depth_stencil_state();
	gc.reset_program_object();
	gc.reset_primitives_elements();
	gc.reset_texture(0);
	gc.reset_texture(1);
	gc.reset_uniform_buffer(0);

	gc.reset_frame_buffer();
}

void SkyboxPass::setup(GraphicContext &gc)
{
	if (cube_program.is_null())
	{
		create_clouds(gc);
		create_stars(gc);
		create_programs(gc);
	}

	Size viewport_size = viewport->get_size();
	if (fb.is_null() || !gc.is_frame_buffer_owner(fb) || zbuffer.updated())
	{
		fb = FrameBuffer(gc);
		fb.attach_color(0, self_illumination_gbuffer.get());
/*
		fb.attach_color(0, diffuse_color_gbuffer.get());
		fb.attach_color(1, specular_color_gbuffer.get());
		fb.attach_color(2, specular_level_gbuffer.get());
		fb.attach_color(3, self_illumination_gbuffer.get());
		fb.attach_color(4, normal_z_gbuffer.get());
*/
		fb.attach_depth(zbuffer.get());

		BlendStateDescription blend_desc;
		blend_desc.enable_blending(true);
		blend_desc.set_blend_function(blend_src_alpha, blend_one_minus_src_alpha, blend_zero, blend_zero);
		blend_state = BlendState(gc, blend_desc);

		DepthStencilStateDescription depth_stencil_desc;
		depth_stencil_desc.enable_depth_write(false);
		depth_stencil_desc.enable_depth_test(true);
		depth_stencil_desc.set_depth_compare_function(compare_lequal);
		depth_stencil_state = DepthStencilState(gc, depth_stencil_desc);

		RasterizerStateDescription rasterizer_desc;
		rasterizer_desc.set_culled(false);
		rasterizer_state = RasterizerState(gc, rasterizer_desc);

		billboard_prim_array = PrimitivesArray(gc);
		billboard_prim_array.set_attributes(0, billboard_positions);

		cube_prim_array = PrimitivesArray(gc);
		cube_prim_array.set_attributes(0, cube_positions);
	}
}

void SkyboxPass::create_clouds(GraphicContext &gc)
{
	create_cloud_texture(gc);
}

void SkyboxPass::create_cloud_texture(GraphicContext &gc)
{
	int width = 512;
	int height = 512;
	int turbulence = 300;
	Color color = Color::mediumslateblue;

	Noise2D noise(width, height);
	PixelBuffer cloud(width, height, tf_rgba16);
	Vec4us *cloud_data = cloud.get_data<Vec4us>();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int alpha = (int)clamp(noise.turbulence(x, y, (double)turbulence) * 65535.0 - 20000.0, 0.0, 65535.0);
			cloud_data[x+y*512] = Vec4us(color.r * 64, color.g * 64, color.b * 64, alpha);
		}
	}
	cloud_texture = Texture2D(gc, width, height, tf_rgba16, 0);
	cloud_texture.set_image(gc, cloud);
	cloud_texture.generate_mipmap();
}

void SkyboxPass::create_stars(GraphicContext &gc)
{
	create_star_texture(gc);

	PixelBuffer instances(num_star_instances, 1, tf_rgba32f);
	Vec4f *instances_data = instances.get_data<Vec4f>();
	for (size_t i = 0; i < num_star_instances; i++)
	{
		float range = 700.0f;
		do
		{
			instances_data[i] = Vec4f(random(-range, range), random(-range, range), random(-range, range), 1.0f);
		} while (instances_data[i].length3() < range * 0.5f);
	}

	star_instance_texture = Texture2D(gc, num_star_instances, 1, tf_rgba32f);
	star_instance_texture.set_image(gc, instances);
}

void SkyboxPass::create_star_texture(GraphicContext &gc)
{
	PixelBuffer star_icon(512, 512, tf_rgba16);
	Vec4us *star_icon_data = star_icon.get_data<Vec4us>();
	for (int y = 0; y < 512; y++)
	{
		for (int x = 0; x < 512; x++)
		{
			float s = (x + 0.5f) / 512.0f;
			float t = (y + 0.5f) / 512.0f;
			float distance = 1.0f - (Vec2f(s,t) * 2.0f - 1.0f).length();
			distance *= 1.25f;
			star_icon_data[x + y * 512] = Vec4us(230*256, 230*256, 255*256, clamp((int)(distance * 65535.0f + 0.5f), 0, 65535));
		}
	}
	star_texture = Texture2D(gc, 512, 512, tf_rgba16);
	star_texture.set_image(gc, star_icon);
	star_texture.generate_mipmap();
}

void SkyboxPass::create_programs(GraphicContext &gc)
{
	uniforms = UniformVector<Uniforms>(gc, 1);
	create_billboard_program(gc);
	create_cube_program(gc);
}

void SkyboxPass::create_billboard_program(GraphicContext &gc)
{
	billboard_positions = VertexArrayVector<Vec3f>(gc, cpu_billboard_positions, 6);

	billboard_program = ProgramObject::load(gc, "Resources/Engine/Skybox/vertex_billboard.hlsl", "Resources/Engine/Skybox/fragment_billboard.hlsl");
	billboard_program.bind_attribute_location(0, "AttrPosition");
	billboard_program.bind_frag_data_location(0, "FragColor");
	if (!billboard_program.link())
		throw Exception(string_format("Link failed: %1", billboard_program.get_info_log()));
	billboard_program.set_uniform1i("InstanceTexture", 0);
	billboard_program.set_uniform1i("ParticleTexture", 1);
	billboard_program.set_uniform1i("ParticleTextureSampler", 1);
	billboard_program.set_uniform_buffer_index("Uniforms", 0);
}

void SkyboxPass::create_cube_program(GraphicContext &gc)
{
	cube_positions = VertexArrayVector<Vec3f>(gc, cpu_cube_positions, 36);

	cube_program = ProgramObject::load(gc, "Resources/Engine/Skybox/vertex_cube.hlsl", "Resources/Engine/Skybox/fragment_sphere.hlsl");
	cube_program.bind_attribute_location(0, "AttrPosition");
	cube_program.bind_frag_data_location(0, "FragColor");
	if (!cube_program.link())
		throw Exception(string_format("Link failed: %1", cube_program.get_info_log()));
	cube_program.set_uniform1i("SkyboxTexture", 0);
	cube_program.set_uniform1i("SkyboxTextureSampler", 0);
	cube_program.set_uniform_buffer_index("Uniforms", 0);
}

float SkyboxPass::random(float min_value, float max_value)
{
	float v = rand() / (float)RAND_MAX;
	return min_value + v * (max_value - min_value);
}

Vec3f SkyboxPass::cpu_billboard_positions[6] = 
{
	Vec3f(-1.0f, -1.0f, 0.0f),
	Vec3f( 1.0f, -1.0f, 0.0f),
	Vec3f(-1.0f,  1.0f, 0.0f),
	Vec3f(-1.0f,  1.0f, 0.0f),
	Vec3f( 1.0f, -1.0f, 0.0f),
	Vec3f( 1.0f,  1.0f, 0.0f)
};

Vec3f SkyboxPass::cpu_cube_positions[6 * 6] =
{
	Vec3f(-1.0f,  1.0f,  1.0f),
	Vec3f( 1.0f,  1.0f,  1.0f),
	Vec3f( 1.0f, -1.0f,  1.0f),

	Vec3f( 1.0f, -1.0f,  1.0f),
	Vec3f(-1.0f, -1.0f,  1.0f),
	Vec3f(-1.0f,  1.0f,  1.0f),


	Vec3f( 1.0f, -1.0f, -1.0f),
	Vec3f( 1.0f,  1.0f, -1.0f),
	Vec3f(-1.0f,  1.0f, -1.0f),

	Vec3f(-1.0f,  1.0f, -1.0f),
	Vec3f(-1.0f, -1.0f, -1.0f),
	Vec3f( 1.0f, -1.0f, -1.0f),


	Vec3f( 1.0f,  1.0f, -1.0f),
	Vec3f( 1.0f,  1.0f,  1.0f),
	Vec3f(-1.0f,  1.0f,  1.0f),

	Vec3f(-1.0f,  1.0f,  1.0f),
	Vec3f(-1.0f,  1.0f, -1.0f),
	Vec3f( 1.0f,  1.0f, -1.0f),


	Vec3f(-1.0f, -1.0f,  1.0f),
	Vec3f( 1.0f, -1.0f,  1.0f),
	Vec3f( 1.0f, -1.0f, -1.0f),

	Vec3f( 1.0f, -1.0f, -1.0f),
	Vec3f(-1.0f, -1.0f, -1.0f),
	Vec3f(-1.0f, -1.0f,  1.0f),


	Vec3f( 1.0f, -1.0f,  1.0f),
	Vec3f( 1.0f,  1.0f,  1.0f),
	Vec3f( 1.0f,  1.0f, -1.0f),

	Vec3f( 1.0f,  1.0f, -1.0f),
	Vec3f( 1.0f, -1.0f, -1.0f),
	Vec3f( 1.0f, -1.0f,  1.0f),


	Vec3f(-1.0f,  1.0f, -1.0f),
	Vec3f(-1.0f,  1.0f,  1.0f),
	Vec3f(-1.0f, -1.0f,  1.0f),

	Vec3f(-1.0f, -1.0f,  1.0f),
	Vec3f(-1.0f, -1.0f, -1.0f),
	Vec3f(-1.0f,  1.0f, -1.0f)
};

}

