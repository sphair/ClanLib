/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Mark Page
*/
#include "precomp.h"
#include "model.h"
#include "3D/lib3ds_help.h"

Model::Model(GraphicContext &gc, const std::string &filename, TextureTactic texture_tactic)
: scale(1.0f)
{
	PixelBuffer image;
	if (texture_tactic == use_silver)
		image = load_image("Resources/silver.jpg");
	else if (texture_tactic == use_wood)
		image = load_image("Resources/wood.jpg");
	if (!image.is_null())
	{
		wood_texture = Texture(gc, image.get_size());
		wood_texture.set_image(image);

		// FIXME: This does not work, maybe because "Rectangular textures do not support mipmapping"
		// FIXME: wood_texture.generate_mipmap();
		// FIXME: wood_texture.set_min_filter(cl_filter_linear_mipmap_linear);
		wood_texture.set_min_filter(cl_filter_linear);
		wood_texture.set_mag_filter(cl_filter_linear);
		wood_texture.set_max_anisotropy(8.0f);
	}

	create_shader_programs(gc);
	load_model(gc, filename);
}

Model::~Model()
{
}

void Model::render(GraphicContext &gc, const LightModel &light_model, const Position &position, const Orientation &orientation)
{
	Mat4f modelmatrix = Mat4f::identity();
	modelmatrix.multiply(position.to_matrix());
	modelmatrix.multiply(orientation.to_matrix());
	modelmatrix.multiply(Mat4f::scale(scale, scale, scale));
	gc.push_modelview();
	gc.mult_modelview(modelmatrix);

	PolygonRasterizer polygon_rasterizer;
	polygon_rasterizer.set_culled(false/*true*/);
	polygon_rasterizer.set_front_face(cl_face_side_counter_clockwise);
	gc.set_polygon_rasterizer(polygon_rasterizer);

	PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, vertices, 3, cl_type_float, &static_cast<Vertex*>(0)->position, sizeof(Vertex));
	prim_array.set_attributes(1, vertices, 3, cl_type_float, &static_cast<Vertex*>(0)->normal, sizeof(Vertex));
	prim_array.set_attributes(2, vertices, 2, cl_type_float, &static_cast<Vertex*>(0)->texcoord0, sizeof(Vertex));
	gc.set_primitives_array(prim_array);

	program_object_texture.set_uniform1i("texture1", 0);

	std::map<int, MaterialRange>::iterator it;
	for (it = material_ranges.begin(); it != material_ranges.end(); ++it)
	{
		if (!wood_texture.is_null())
		{
			gc.set_texture(0, wood_texture);
			gc.set_program_object(program_object_texture);
			setup_light(program_object_texture, it->second.material, light_model);
		}
		else
		{
			if (!it->second.texture.is_null())
			{
				gc.set_texture(0, it->second.texture);
				gc.set_program_object(program_object_texture);
				setup_light(program_object_texture, it->second.material, light_model);
			}
			else
			{
				gc.set_program_object(program_object_no_texture);
				setup_light(program_object_no_texture, it->second.material, light_model);
			}
		}

		gc.draw_primitives_array(type_triangles, it->second.start, it->second.length);
	}

	gc.reset_texture(0);
	gc.reset_program_object();

	gc.reset_primitives_array();
	gc.reset_polygon_rasterizer();
	gc.pop_modelview();
}

void Model::setup_light(ProgramObject &program, const Material &material, const LightModel &light_model)
{
/*
uniform vec4 lightSourcePosition;
uniform vec4 lightSourceHalfVector;
uniform vec4 lightSourceDiffuse;
uniform vec4 lightSourceAmbient;
uniform vec4 lightSourceSpecular;
uniform float lightSourceConstantAttenuation;
uniform float lightSourceLinearAttenuation;
uniform float lightSourceQuadraticAttenuation;
uniform vec4 frontMaterialDiffuse;
uniform vec4 frontMaterialAmbient;
uniform vec4 frontMaterialSpecular;
uniform float frontMaterialShininess;
uniform vec4 lightModelAmbient;
*/
	program.set_uniform4f("lightSourcePosition", light_model.light_sources[0].position);
	program.set_uniform4f("lightSourceHalfVector", light_model.light_sources[0].get_halfvector());
	program.set_uniform4f("lightSourceDiffuse", light_model.light_sources[0].diffuse);
	program.set_uniform4f("lightSourceAmbient", light_model.light_sources[0].ambient);
	program.set_uniform4f("lightSourceSpecular", light_model.light_sources[0].specular);
	program.set_uniform1f("lightSourceConstantAttenuation", light_model.light_sources[0].constant_attenuation);
	program.set_uniform1f("lightSourceLinearAttenuation", light_model.light_sources[0].constant_attenuation);
	program.set_uniform1f("lightSourceQuadraticAttenuation", light_model.light_sources[0].quadratic_attenuation);
	program.set_uniform4f("frontMaterialDiffuse", material.diffuse);
	program.set_uniform4f("frontMaterialAmbient", material.ambient);
	program.set_uniform4f("frontMaterialSpecular", material.specular);
	program.set_uniform1f("frontMaterialShininess", material.shininess);
	program.set_uniform4f("lightModelAmbient", light_model.scene_ambient);

/*
	program.set_uniform4f("lightSourcePosition", light_model.light_sources[0].position);
	program.set_uniform4f("frontLightProductAmbient", light_model.get_light_ambient(material, light_model.light_sources[0]));
	program.set_uniform4f("frontLightProductDiffuse", light_model.get_light_diffuse(material, light_model.light_sources[0]));
	program.set_uniform4f("frontLightProductSpecular", light_model.get_light_specular(material, light_model.light_sources[0]));
	program.set_uniform1f("frontMaterialShininess", material.shininess);
	program.set_uniform4f("frontLightModelProductSceneColor", light_model.get_scene_color(material));
*/
}

void Model::create_shader_programs(GraphicContext &gc)
{
	std::string resource_dir = "Resources/"; // Directory::get_resourcedata(app_name);

	VirtualDirectory vd;
	program_object_texture = ProgramObject::load(gc, resource_dir + "model_3ds_vertex.glsl", resource_dir + "model_3ds_fragment.glsl", vd);
	program_object_texture.bind_attribute_location(0, "in_position");
	program_object_texture.bind_attribute_location(1, "in_normal");
	program_object_texture.bind_attribute_location(2, "in_texcoord");
	if (!program_object_texture.link())
		throw Exception("Unable to link 3ds model program object (texture)");
	program_object_no_texture = ProgramObject::load(gc, resource_dir + "model_3ds_vertex.glsl", resource_dir + "model_3ds_fragment_no_texture.glsl", vd);
	program_object_no_texture.bind_attribute_location(0, "in_position");
	program_object_no_texture.bind_attribute_location(1, "in_normal");
	if (!program_object_no_texture.link())
		throw Exception("Unable to link 3ds model program object (no texture)");
}

void Model::load_model(GraphicContext &gc, const std::string & filename)
{
	File file(filename, File::open_existing, File::access_read);
	Lib3dsFile model_file;
	model_file.load(file);
	file.close();

	std::vector<Lib3dsMesh> meshes = model_file.export_meshes();
	size_t total_vertices = 0;
	for (size_t i = 0; i < meshes.size(); i++)
	{
		for (size_t j = 0; j < meshes[i].face_materials.size(); j++)
			material_ranges[meshes[i].face_materials[j]].length += 3;
		total_vertices += meshes[i].positions.size();
	}

	vertices = VertexArrayBuffer(gc, sizeof(Vertex)*total_vertices);
	vertices.lock(cl_access_write_only);

	Vertex *vertices_data = reinterpret_cast<Vertex *>(vertices.get_data());
	int offset = 0;

	std::map<int, MaterialRange>::iterator it;
	for (it = material_ranges.begin(); it != material_ranges.end(); ++it)
	{
		it->second.start = offset;
		for (size_t i = 0; i < meshes.size(); i++)
		{
			for (size_t j = 0; j < meshes[i].face_materials.size(); j++)
			{
				if (meshes[i].face_materials[j] == it->first)
				{
					for (int k = 0; k < 3; k++)
					{
						Vertex v;
						v.position = meshes[i].positions[j*3+k];
						v.normal = meshes[i].normals[j*3+k];
						v.texcoord0 = meshes[i].texcooords[j*3+k];
						vertices_data[offset+k] = v;
					}
					offset += 3;
				}
			}
		}

		if (it->first == -1)
		{
			// Default (null) material:
			Material material;
			material.ambient = Vec4f(0.2f, 0.2f, 0.2f, 1.0f);
			material.diffuse = Vec4f(0.8f, 0.8f, 0.8f, 1.0f);
			material.specular = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
			it->second.material = material;
		}
		else
		{
			Lib3dsMaterial *m = model_file->materials[it->first];

			if (wood_texture.is_null())
			{
				if (m->texture1_map.name[0])
				{
					if (textures.find(m->texture1_map.name) == textures.end())
					{
						PixelBuffer image = load_image("Resources/Sponza/" + StringHelp::local8_to_text(m->texture1_map.name));
						textures[m->texture1_map.name] = Texture(gc, image.get_size());
						textures[m->texture1_map.name].set_image(image);
						textures[m->texture1_map.name].set_wrap_mode(wrap_repeat, wrap_repeat);
						textures[m->texture1_map.name].set_max_anisotropy(8.0f);

						// FIXME: This does not work, maybe because "Rectangular textures do not support mipmapping"
						//textures[m->texture1_map.name].generate_mipmap();
						//textures[m->texture1_map.name].set_min_filter(cl_filter_linear_mipmap_linear);
						textures[m->texture1_map.name].set_min_filter(cl_filter_linear);
						textures[m->texture1_map.name].set_mag_filter(cl_filter_linear);
					}
					it->second.texture = textures[m->texture1_map.name];
				}
			}

			// Should we also bind texture1_mask, texture2_map and texture2_mask?
			// In which situations are those textures used?
			// Textures can also be rotated and scaled on a material. How often is this done?

			Material material;
			material.ambient = Vec4f(m->ambient[0], m->ambient[1], m->ambient[2], m->ambient[3]);
			material.diffuse = Vec4f(m->diffuse[0], m->diffuse[1], m->diffuse[2], m->diffuse[3]);
			material.specular = Vec4f(m->specular[0], m->specular[1], m->specular[2], m->specular[3]);
			float s = pow(2, 10.0f * m->shininess);
			if (s > 128.0f)
				s = 128.0f;
			material.shininess = s;
			it->second.material = material;
		}
	}

	vertices.unlock();
}

PixelBuffer Model::load_image(const std::string &filename)
{
	// We flip the image because Texture loads the image where 0,0 is upper left corner,
	// but most 3D software expects 0,0 to be the lower left corner.

	PixelBuffer image = ImageProviderFactory::load(filename).to_format(cl_rgba8);
	Size image_size = image.get_size();
	unsigned int *p = static_cast<unsigned int *>(image.get_data());
	for (int y = 0; y < image_size.height/2; y++)
	{
		unsigned int *line1 = p+y*image_size.width;
		unsigned int *line2 = p+(image_size.height-y-1)*image_size.width;
		for (int x = 0; x < image_size.width; x++)
		{
			unsigned int t = line1[x];
			line1[x] = line2[x];
			line2[x] = t;
		}
	}
	return image;
}
