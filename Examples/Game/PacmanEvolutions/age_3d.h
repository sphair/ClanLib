
#pragma once

class GameWorld;

class Age3D
{
public:
	Age3D(clan::Canvas &canvas, GameWorld *game);
	void render(clan::Canvas &canvas);

private:
	struct WallVertex
	{
		clan::Vec3f position;
		clan::Vec2f texcoord0;
		clan::Vec3f normal;
	};

	struct Uniforms
	{
		clan::Vec4f object_normal_to_eye[3]; // mat3, but with std140 layout it is stored with 3*vec4
		clan::Mat4f object_to_world;
		clan::Mat4f world_to_eye;
		clan::Mat4f eye_to_projection;
	};

	std::vector<WallVertex> create_walls();
	void write_wall(std::vector<WallVertex> &in_out_vertices, const clan::Vec3f &p1, const clan::Vec3f &p2, const clan::Vec3f &normal);

	void create_billboards(clan::GraphicContext &gc);
	void draw_billboard(clan::Canvas &canvas, clan::Sprite &sprite, const clan::Mat4f &world_to_projection, float x, float y);
	clan::Vec2f project(clan::GraphicContext &gc, const clan::Mat4f &world_to_projection, clan::Vec4f position_in_world);

	GameWorld *game;
	int num_wall_vertices;
	clan::VertexArrayVector<WallVertex> wall_vertices;
	clan::PrimitivesArray walls_prim_array;
	clan::ProgramObject walls_program;
	clan::UniformVector<Uniforms> uniforms;
	clan::Texture2D wall_texture;
	clan::RasterizerState raster_state;
	clan::DepthStencilState depth_stencil_state;
	clan::FrameBuffer fb;
	clan::RenderBuffer depth_buffer;
	clan::Texture2D final_texture;

	clan::Sprite tiles;
	clan::Sprite pacman[4]; // left, right, up, down
	clan::Sprite ghost[2]; // green and red ghost animations
	clan::Vec2f look_at;
};
