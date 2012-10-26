
#pragma once

class GameWorld;

class Age3D
{
public:
	Age3D(CL_GraphicContext &gc, GameWorld *game);
	void render(CL_GraphicContext &gc);

private:
	struct WallVertex
	{
		CL_Vec3f position;
		CL_Vec2f texcoord0;
		CL_Vec3f normal;
	};

	struct Uniforms
	{
		CL_Vec4f object_normal_to_eye[3]; // mat3, but with std140 layout it is stored with 3*vec4
		CL_Mat4f object_to_world;
		CL_Mat4f world_to_eye;
		CL_Mat4f eye_to_projection;
	};

	std::vector<WallVertex> create_walls();
	void write_wall(std::vector<WallVertex> &in_out_vertices, const CL_Vec3f &p1, const CL_Vec3f &p2, const CL_Vec3f &normal);

	void create_billboards(CL_GraphicContext &gc);
	void draw_billboard(CL_GraphicContext &gc, CL_Sprite &sprite, const CL_Mat4f &world_to_projection, float x, float y);
	CL_Vec2f project(CL_GraphicContext &gc, const CL_Mat4f &world_to_projection, CL_Vec4f position_in_world);

	GameWorld *game;
	int num_wall_vertices;
	CL_VertexArrayVector<WallVertex> wall_vertices;
	CL_PrimitivesArray walls_prim_array;
	CL_ProgramObject walls_program;
	CL_UniformVector<Uniforms> uniforms;
	CL_Texture2D wall_texture;
	CL_RasterizerState raster_state;
	CL_DepthStencilState depth_stencil_state;
	CL_FrameBuffer fb;
	CL_RenderBuffer depth_buffer;
	CL_Texture2D final_texture;

	CL_Sprite tiles;
	CL_Sprite pacman[4]; // left, right, up, down
	CL_Sprite ghost[2]; // green and red ghost animations
	CL_Vec2f look_at;
};
