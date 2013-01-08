
#include "precomp.h"
#include "game_terrain.h"
#include "3D/light_model.h"

GameTerrain::GameTerrain(
	GraphicContext &gc, 
	const std::string &heightmap_png, 
	const std::string &texture_png, 
	const std::string &areas_png, 
	const std::string &borders_png, 
	int area_count, 
	float vertical_scale)
: num_vertices(0)
{
	color_areas = PNGProvider::load(areas_png).to_format(tf_r8);

	PixelBuffer pb = PNGProvider::load(heightmap_png).to_format(tf_rgba8);
	int width = pb.get_width()-1;
	int height = pb.get_height()-1;

	num_vertices = width*height*3*4 /*+ width*6*2 + height*6*2 + 6*/;
	vertex_buffer = VertexArrayBuffer(gc, num_vertices*sizeof(Vertex));
	vertex_buffer.lock(cl_access_write_only);
	Vertex *vertex_data = reinterpret_cast<Vertex *>(vertex_buffer.get_data());

	int pitch = pb.get_pitch();
	unsigned char *data = reinterpret_cast<unsigned char *>(pb.get_data());
	for (int y = 0; y < height; y++)
	{
		unsigned int *line1 = reinterpret_cast<unsigned int *>(data + y*pitch);
		unsigned int *line2 = reinterpret_cast<unsigned int *>(data + (y+1)*pitch);
		Vertex *vertex_line = vertex_data + 3*4*width*y;
		for (int x = 0; x < width; x++)
		{
			float height1 = (line1[x] >> 24) * vertical_scale;
			float height2 = (line1[x+1] >> 24) * vertical_scale;
			float height3 = (line2[x] >> 24) * vertical_scale;
			float height4 = (line2[x+1] >> 24) * vertical_scale;
			float height5 = (height1 + height2 + height3 + height4) / 4.0f;
			Vertex *vertex_quad = vertex_line + x*3*4;

			Vec3f positions[12] =
			{
				Vec3f(x+0.0f, height1, y+0.0f),
				Vec3f(x+1.0f, height2, y+0.0f),
				Vec3f(x+0.5f, height5, y+0.5f),

				Vec3f(x+1.0f, height2, y+0.0f),
				Vec3f(x+1.0f, height4, y+1.0f),
				Vec3f(x+0.5f, height5, y+0.5f),

				Vec3f(x+1.0f, height4, y+1.0f),
				Vec3f(x+0.0f, height3, y+1.0f),
				Vec3f(x+0.5f, height5, y+0.5f),

				Vec3f(x+0.0f, height3, y+1.0f),
				Vec3f(x+0.0f, height1, y+0.0f),
				Vec3f(x+0.5f, height5, y+0.5f)
			};

			for (int i = 0; i < 12; i++)
				vertex_quad[i].position = positions[i];

			Vec3f normal1 = calc_normal(x, y, data, width, height, pitch);
			Vec3f normal2 = calc_normal(x+1, y, data, width, height, pitch);
			Vec3f normal3 = calc_normal(x, y+1, data, width, height, pitch);
			Vec3f normal4 = calc_normal(x+1, y+1, data, width, height, pitch);
			Vec3f normal5 = (normal1+normal2+normal3+normal4)/4.0f;

			vertex_quad[0].normal = normal1;
			vertex_quad[1].normal = normal2;
			vertex_quad[2].normal = normal5;
			vertex_quad[3].normal = normal2;
			vertex_quad[4].normal = normal4;
			vertex_quad[5].normal = normal5;
			vertex_quad[6].normal = normal4;
			vertex_quad[7].normal = normal3;
			vertex_quad[8].normal = normal5;
			vertex_quad[9].normal = normal3;
			vertex_quad[10].normal = normal1;
			vertex_quad[11].normal = normal5;
		}
	}

	vertex_data += width*height*3*4;
/*
	for (int y = 0; y < height; y++)
	{
		unsigned int *line1 = reinterpret_cast<unsigned int *>(data + y*pitch);
		unsigned int *line2 = reinterpret_cast<unsigned int *>(data + (y+1)*pitch);
		float scale = 0.4f;
		float height1 = (line1[0] >> 24)*scale;
		float height2 = (line2[0] >> 24)*scale;
		float height3 = (line1[width] >> 24)*scale;
		float height4 = (line2[width] >> 24)*scale;

		vertex_data[y*6*2+0].position = Vec3f(0, -0.1f, (float)y);
		vertex_data[y*6*2+1].position = Vec3f(0, (float)height1, (float)y);
		vertex_data[y*6*2+2].position = Vec3f(0, (float)height2, (float)y+1);
		vertex_data[y*6*2+3].position = Vec3f(0, (float)height2, (float)y+1);
		vertex_data[y*6*2+4].position = Vec3f(0, -0.1f, (float)y+1);
		vertex_data[y*6*2+5].position = Vec3f(0, -0.1f, (float)y);

		vertex_data[y*6*2+11].position = Vec3f((float)width+1, -0.1f, (float)y);
		vertex_data[y*6*2+10].position = Vec3f((float)width+1, (float)height3, (float)y);
		vertex_data[y*6*2+9].position = Vec3f((float)width+1, (float)height4, (float)y+1);
		vertex_data[y*6*2+8].position = Vec3f((float)width+1, (float)height4, (float)y+1);
		vertex_data[y*6*2+7].position = Vec3f((float)width+1, -0.1f, (float)y+1);
		vertex_data[y*6*2+6].position = Vec3f((float)width+1, -0.1f, (float)y);

		for (int i = 0; i < 6; i++)
			vertex_data[y*6*2+i].normal = Vec3f(-1, 0, 0);
		for (int i = 0; i < 6; i++)
			vertex_data[y*6*2+6+i].normal = Vec3f(1, 0, 0);
	}

	vertex_data += height*6*2;

	unsigned int *line1 = reinterpret_cast<unsigned int *>(data);
	unsigned int *line2 = reinterpret_cast<unsigned int *>(data + height*pitch);

	for (int x = 0; x < width; x++)
	{
		float scale = 0.4f;
		float height1 = (line1[x] >> 24)*scale;
		float height2 = (line1[x+1] >> 24)*scale;
		float height3 = (line2[x] >> 24)*scale;
		float height4 = (line2[x+1] >> 24)*scale;

		vertex_data[x*6*2+5].position = Vec3f((float)x, -0.1f, 0);
		vertex_data[x*6*2+4].position = Vec3f((float)x, (float)height1, 0);
		vertex_data[x*6*2+3].position = Vec3f((float)x+1, (float)height2, 0);
		vertex_data[x*6*2+2].position = Vec3f((float)x+1, (float)height2, 0);
		vertex_data[x*6*2+1].position = Vec3f((float)x+1, -0.1f, 0);
		vertex_data[x*6*2+0].position = Vec3f((float)x, -0.1f, 0);

		vertex_data[x*6*2+6].position = Vec3f((float)x, -0.1f, (float)height+1);
		vertex_data[x*6*2+7].position = Vec3f((float)x, (float)height3, (float)height+1);
		vertex_data[x*6*2+8].position = Vec3f((float)x+1, (float)height4, (float)height+1);
		vertex_data[x*6*2+9].position = Vec3f((float)x+1, (float)height4, (float)height+1);
		vertex_data[x*6*2+10].position = Vec3f((float)x+1, -0.1f, (float)height+1);
		vertex_data[x*6*2+11].position = Vec3f((float)x, -0.1f, (float)height+1);

		for (int i = 0; i < 6; i++)
			vertex_data[x*6*2+i].normal = Vec3f(0, 0, -1);
		for (int i = 0; i < 6; i++)
			vertex_data[x*6*2+6+i].normal = Vec3f(0, 0, 1);
	}

	vertex_data += width*6*2;

	vertex_data[0].position = Vec3f(0, -0.1f, 0);
	vertex_data[1].position = Vec3f((float)width, -0.1f, 0);
	vertex_data[2].position = Vec3f((float)width, -0.1f, (float)height);
	vertex_data[3].position = Vec3f((float)width, -0.1f, (float)height);
	vertex_data[4].position = Vec3f(0, -0.1f, (float)height);
	vertex_data[5].position = Vec3f(0, -0.1f, 0);
	for (int i = 0; i < 6; i++)
		vertex_data[i].normal = Vec3f(0, -1, 0);
*/
	vertex_buffer.unlock();

	size = Size(width+1, height+1);

	this->area_count = area_count;

	VirtualDirectory vdir;
	shader_program = ProgramObject::load(gc, "Resources/map_vertex.glsl", "Resources/map_fragment.glsl", vdir);
	shader_program.bind_attribute_location(0, "in_position");
	shader_program.bind_attribute_location(1, "in_normal");
	if (!shader_program.link())
		throw Exception("Map shader program failed to link: " + shader_program.get_info_log());

	texture_base = Texture2D(gc, texture_png);
	texture_base.set_min_filter(filter_linear);
	texture_base.set_mag_filter(filter_linear);

	// Change the values between area colors to avoid GPUs merging them together
	PixelBuffer image = ImageProviderFactory::load(areas_png).to_format(tf_rgba8);
	unsigned int* image_data = reinterpret_cast<unsigned int*>(image.get_data());
	int image_total_pixels = image.get_width()*image.get_height();
	for (int i = 0; i < image_total_pixels; i++)
	{
		unsigned int area_color = (image_data[i]>>24)*255/area_count;
		image_data[i] = (area_color<<24)+(area_color<<16)+(area_color<<8)+area_color;
	}
	texture_areas = Texture2D(gc, image.get_size());
	texture_areas.set_image(image);
	texture_areas.set_min_filter(filter_nearest);
	texture_areas.set_mag_filter(filter_nearest);

	texture_borders = Texture2D(gc, borders_png);
	texture_borders.set_min_filter(filter_linear);
	texture_borders.set_mag_filter(filter_linear);
}

void GameTerrain::set_area_colors(GraphicContext &gc, const std::vector<unsigned int> &colors)
{
	PixelBuffer buffer_colors(colors.size(), 1, tf_rgba8);
	unsigned int *col = reinterpret_cast<unsigned int *>(buffer_colors.get_data());
	for(unsigned int i=0;i<colors.size();++i)
		col[i] = colors[i];

	texture_colors = Texture2D(gc, area_count+1, 1);
	texture_colors.set_image(buffer_colors);
	texture_colors.set_wrap_mode(cl_wrap_clamp_to_edge, cl_wrap_clamp_to_edge);
	texture_colors.set_min_filter(filter_nearest);
	texture_colors.set_mag_filter(filter_nearest);
}

inline Vec3f GameTerrain::calc_normal(int x, int y, unsigned char *data, int width, int height, int pitch)
{
	Vec3f vectors[9];
	for (int yy = 0; yy < 3; yy++)
	{
		for (int xx = 0; xx < 3; xx++)
		{
			vectors[xx+yy*3] = get_vector(x, y, xx-1, yy-1, data, width, height, pitch);
		}
	}

	Vec3f normals[8] =
	{
		Vec3f::cross(vectors[1], vectors[0]),
		Vec3f::cross(vectors[2], vectors[1]),
		Vec3f::cross(vectors[5], vectors[2]),
		Vec3f::cross(vectors[8], vectors[5]),
		Vec3f::cross(vectors[7], vectors[8]),
		Vec3f::cross(vectors[6], vectors[7]),
		Vec3f::cross(vectors[3], vectors[6]),
		Vec3f::cross(vectors[0], vectors[3])
	};
	for (int i = 0; i < 8; i++)
		normals[i].normalize();

	Vec3f normal(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 8; i++)
		normal += normals[i];
	normal /= 8.0f;
	normal.normalize();
	return normal;
}

inline Vec3f GameTerrain::get_vector(int x, int y, int xx, int yy, unsigned char *data, int width, int height, int pitch)
{
	Vec3f v((float) xx, 0.0f, (float) yy);
	int px = x+xx;
	int py = y+yy;
	if (px < 0)
		px = 0;
	if (px > width)
		px = width;
	if (py < 0)
		py = 0;
	if (py > height)
		py = height;

	unsigned int *pixel1 = reinterpret_cast<unsigned int *>(data+y*pitch+x*4);
	unsigned int *pixel2 = reinterpret_cast<unsigned int *>(data+py*pitch+px*4);
	float h1 = ((*pixel1) >> 24) * 0.4f;
	float h2 = ((*pixel2) >> 24) * 0.4f;
	v.y = h2 - h1;
	return v;
}

void GameTerrain::render(Canvas &canvas, const LightModel &light_model)
{
	Rectf rect(0, 0, (float)size.width-1, (float)size.height-1);
	render_sprite(canvas, light_model, rect);
}

void GameTerrain::render_sprite(Canvas &canvas, const LightModel &light_model, Rectf dest)
{
	canvas.push_modelview();
	canvas.mult_scale(4.0f, 1.0f, 4.0f);
	canvas.mult_modelview(Mat4f::translate(-size.width/2.0f, 0.0f, -size.height/2.0f));

	gc.set_culled(true);
	gc.set_front_face(cl_face_side_counter_clockwise);

	PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, vertex_buffer, 3, cl_type_float, (int) &static_cast<Vertex*>(0)->position, sizeof(Vertex));
	prim_array.set_attributes(1, vertex_buffer, 3, cl_type_float, (int) &static_cast<Vertex*>(0)->normal, sizeof(Vertex));

	gc.set_texture(0, texture_base);
	gc.set_texture(1, texture_areas);
	gc.set_texture(2, texture_colors);
	gc.set_texture(3, texture_borders);

	gc.set_program_object(shader_program);

	Material material;
	// material.shininess = 25.0f;

	shader_program.set_uniform4f("area", Vec4f(dest.left, dest.top, dest.get_width(), dest.get_height()));
	shader_program.set_uniform1i("texture_base", 0);
	shader_program.set_uniform1i("texture_areas", 1);
	shader_program.set_uniform1i("texture_colors", 2);
	shader_program.set_uniform1i("texture_borders", 3);
/*
	shader_program.set_uniform("lightSourcePosition", light_model.light_sources[0].position);
	shader_program.set_uniform("frontLightProductAmbient", light_model.get_light_ambient(material, light_model.light_sources[0]));
	shader_program.set_uniform("frontLightProductDiffuse", light_model.get_light_diffuse(material, light_model.light_sources[0]));
	shader_program.set_uniform("frontLightProductSpecular", light_model.get_light_specular(material, light_model.light_sources[0]));
	shader_program.set_uniform("frontMaterialShininess", material.shininess);
	shader_program.set_uniform("frontLightModelProductSceneColor", light_model.get_scene_color(material));
*/
	gc.draw_primitives(cl_triangles, num_vertices, prim_array);

	gc.reset_program_object();
	gc.reset_texture(0);
	gc.reset_texture(1);
	gc.reset_texture(2);
	gc.reset_texture(3);

	gc.reset_polygon_rasterizer();

	canvas.pop_modelview();
}

Point GameTerrain::from_screen_point(GraphicContext &gc, const Point &screen_point, const Mat4f &projection, const Mat4f &modelview2, const Rect &viewport)
{
	OpenGL::set_active(gc);
	
	float depth;
	glReadPixels(screen_point.x, screen_point.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	Mat4f modelview = modelview2;
	modelview = modelview * Mat4f::translate(-size.width*2.0f, 0.0f, -size.height*2.0f);
	Vec3f pos = unproject(Vec3f((float)screen_point.x, (float)screen_point.y, depth), modelview, projection, viewport);
	return Point((int)(pos.x+0.5f),(int)(pos.z+0.5f));
}

unsigned int GameTerrain::get_area(const Point &map_point)
{
	int x = map_point.x * 4;
	int y = map_point.y * 4;

	if (x >= 0 && x < color_areas.get_width())
	{
		if (y >= 0 && y < color_areas.get_height())
		{
			char *area_data = (char *)color_areas.get_data();
			
			char color = area_data[y * color_areas.get_width() + x];
			return (unsigned int) color;
		}
	}

	return 0;
}

Vec3f GameTerrain::unproject(Vec3f win_pos, const Mat4f &modelview, const Mat4f &projection, const Rect &viewport)
{
	Mat4f matrix = projection;
	matrix = matrix * modelview;
	matrix.inverse();
	Vec4f v(
		2*(win_pos.x - (float)viewport.left)/(float)viewport.get_width() - 1,
		2*(win_pos.y - (float)(viewport.top))/(float)viewport.get_height() - 1,
		2*win_pos.z - 1,
		1.0f);
	Vec4f result = matrix * v;
	return Vec3f(result.x/result.w, result.y/result.w, result.z/result.w);
}
