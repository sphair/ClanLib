
#pragma once

class LightModel;

class GameTerrain
{
public:
	GameTerrain(
		GraphicContext &gc, 
		const std::string &heightmap_png, 
		const std::string &texture_png, 
		const std::string &areas_png, 
		const std::string &borders_png, 
		int area_count,
		float vertical_scale);

	void render(Canvas &canvas, const LightModel &light_model);
	Point from_screen_point(GraphicContext &gc, const Point &screen_point, const Mat4f &projection, const Mat4f &modelview, const Rect &viewport);

	const Size &get_size() const { return size; }

	unsigned int get_area(const Point &map_point);

	void set_area_colors(GraphicContext &gc, const std::vector<unsigned int> &colors);

private:
	void render_sprite(Canvas &canvas, const LightModel &light_model, Rectf dest);

	inline Vec3f calc_normal(int x, int y, unsigned char *data, int width, int height, int pitch);
	inline Vec3f get_vector(int x, int y, int xx, int yy, unsigned char *data, int width, int height, int pitch);

	static Vec3f unproject(Vec3f win_pos, const Mat4f &modelview, const Mat4f &projection, const Rect &viewport);

	struct Vertex
	{
		Vec3f position;
		Vec3f normal;
	};

	VertexArrayBuffer vertex_buffer;
	ProgramObject shader_program;
	Texture texture_base;
	Texture texture_colors;
	Texture texture_areas;
	Texture texture_borders;

	PixelBuffer color_areas;
	int area_count;

	int num_vertices;
	Size size;
};
