
#pragma once

class LightModel;

class GameTerrain
{
public:
	GameTerrain(
		CL_GraphicContext &gc, 
		const CL_String &heightmap_png, 
		const CL_String &texture_png, 
		const CL_String &areas_png, 
		const CL_String &borders_png, 
		int area_count,
		float vertical_scale);

	void render(CL_GraphicContext &gc, const LightModel &light_model);
	CL_Point from_screen_point(CL_GraphicContext &gc, const CL_Point &screen_point, const CL_Mat4f &projection, const CL_Mat4f &modelview, const CL_Rect &viewport);

	const CL_Size &get_size() const { return size; }

	unsigned int get_area(const CL_Point &map_point);

	void set_area_colors(CL_GraphicContext &gc, const std::vector<unsigned int> &colors);

private:
	void render_sprite(CL_GraphicContext &gc, const LightModel &light_model, CL_Rectf dest);

	inline CL_Vec3f calc_normal(int x, int y, unsigned char *data, int width, int height, int pitch);
	inline CL_Vec3f get_vector(int x, int y, int xx, int yy, unsigned char *data, int width, int height, int pitch);

	static CL_Vec3f unproject(CL_Vec3f win_pos, const CL_Mat4f &modelview, const CL_Mat4f &projection, const CL_Rect &viewport);

	struct Vertex
	{
		CL_Vec3f position;
		CL_Vec3f normal;
	};

	CL_VertexArrayBuffer vertex_buffer;
	CL_ProgramObject shader_program;
	CL_Texture texture_base;
	CL_Texture texture_colors;
	CL_Texture texture_areas;
	CL_Texture texture_borders;

	CL_PixelBuffer color_areas;
	int area_count;

	int num_vertices;
	CL_Size size;
};
