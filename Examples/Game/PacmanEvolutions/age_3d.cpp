
#include "precomp.h"
#include "age_3d.h"
#include "game_world.h"

Age3D::Age3D(CL_GraphicContext &gc, GameWorld *game)
: game(game), num_wall_vertices()
{
	std::vector<Age3D::WallVertex> cpu_wall_vertices = create_walls();
	num_wall_vertices = cpu_wall_vertices.size();

	wall_vertices = CL_VertexArrayVector<WallVertex>(gc, cpu_wall_vertices);

	walls_prim_array = CL_PrimitivesArray(gc);
	walls_prim_array.set_attributes(0, wall_vertices, cl_offsetof(WallVertex, position));
	walls_prim_array.set_attributes(1, wall_vertices, cl_offsetof(WallVertex, normal));
	walls_prim_array.set_attributes(2, wall_vertices, cl_offsetof(WallVertex, texcoord0));

	walls_program = CL_ProgramObject::load(gc, "vertex_walls.glsl", "fragment_walls.glsl");
	walls_program.bind_attribute_location(0, "AttrPositionInObject");
	walls_program.bind_attribute_location(1, "AttrNormalInObject");
	walls_program.bind_attribute_location(2, "AttrTexCoord");
	walls_program.bind_frag_data_location(0, "FragColor");
	if (!walls_program.link())
		throw CL_Exception("Walls program link failed");
	walls_program.set_uniform_buffer_index("Uniforms", 0);
	walls_program.set_uniform1i("Texture", 0);
	walls_program.set_uniform1i("TextureSampler", 0);

	wall_texture = CL_Texture2D(gc, "wall.jpg");
	wall_texture.generate_mipmap();
	wall_texture.set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
	wall_texture.set_min_filter(cl_filter_linear_mipmap_linear);
	wall_texture.set_mag_filter(cl_filter_linear);

	uniforms = CL_UniformVector<Uniforms>(gc, 1);

	CL_RasterizerStateDescription raster_desc;
	raster_desc.set_culled(true);
	raster_desc.set_front_face(cl_face_side_counter_clockwise);
	raster_state = CL_RasterizerState(gc, raster_desc);

	CL_DepthStencilStateDescription depth_desc;
	depth_desc.enable_depth_test(true);
	depth_desc.enable_depth_write(true);
	depth_desc.set_depth_compare_function(cl_comparefunc_lequal);
	depth_stencil_state = CL_DepthStencilState(gc, depth_desc);

	final_texture = CL_Texture2D(gc, gc.get_width(), gc.get_height(), cl_rgba8);

	depth_buffer = CL_RenderBuffer(gc, gc.get_width(), gc.get_height(), cl_depth_component32f);
	fb = CL_FrameBuffer(gc);
	fb.attach_color(0, final_texture);
	fb.attach_depth(depth_buffer);

	create_billboards(gc);
}

void Age3D::render(CL_GraphicContext &gc)
{
	if (game->player)
		look_at = CL_Vec2f(game->player->get_x(), game->player->get_y());

	float dir = 0.0f;
	float up = 90.0f;
	float tilt = 0.0f;
	CL_Vec3f position(look_at.x + 0.5f, 20.0f, look_at.y + 0.5f);
	CL_Quaternionf orientation = CL_Quaternionf(up, dir, tilt, cl_degrees, cl_YXZ);
	orientation.inverse();

	Uniforms cpu_uniforms;
	cpu_uniforms.object_to_world = CL_Mat4f::identity();
	cpu_uniforms.world_to_eye = orientation.to_matrix() * CL_Mat4f::translate(-position);
	cpu_uniforms.eye_to_projection = CL_Mat4f::perspective(60.0f, gc.get_width()/(float)gc.get_height(), 1.0f, 1e+9, cl_left_handed, gc.get_clip_z_range());
	CL_Mat3f object_normal_to_eye = CL_Mat3f(cpu_uniforms.world_to_eye * cpu_uniforms.object_to_world); // Assumes uniform scale
	cpu_uniforms.object_normal_to_eye[0] = CL_Vec4f(object_normal_to_eye[0], object_normal_to_eye[1], object_normal_to_eye[2], 0.0f);
	cpu_uniforms.object_normal_to_eye[1] = CL_Vec4f(object_normal_to_eye[3], object_normal_to_eye[4], object_normal_to_eye[5], 0.0f);
	cpu_uniforms.object_normal_to_eye[2] = CL_Vec4f(object_normal_to_eye[6], object_normal_to_eye[7], object_normal_to_eye[8], 0.0f);
	uniforms.upload_data(gc, &cpu_uniforms, 1);

	gc.set_frame_buffer(fb);
	gc.clear(CL_Colorf::black);
	gc.clear_depth(1.0f);
	gc.set_rasterizer_state(raster_state);
	gc.set_depth_stencil_state(depth_stencil_state);
	gc.set_program_object(walls_program);
	gc.set_uniform_buffer(0, uniforms);
	gc.set_texture(0, wall_texture);
	gc.draw_primitives(cl_triangles, num_wall_vertices, walls_prim_array);
	gc.reset_texture(0);
	gc.reset_uniform_buffer(0);
	gc.reset_program_object();
	gc.reset_depth_stencil_state();
	gc.reset_rasterizer_state();
	gc.reset_frame_buffer();

	CL_OpenGL::check_error();

	CL_Image image(gc, final_texture, final_texture.get_size());
	image.draw(gc, 0, 0);

	// Draw billboards on top:

	for (int i = 0; i < 4; i++)
		pacman[i].update();
	for (int i = 0; i < 2; i++)
		ghost[i].update();

	CL_Mat4f world_to_projection = cpu_uniforms.eye_to_projection * cpu_uniforms.world_to_eye;

	for (int y = 0; y < game->map.get_height(); y++)
	{
		for (int x = 0; x < game->map.get_width(); x++)
		{
			if (game->map.has_egg(x, y) || game->map.has_powerup(x, y) || game->map.has_evolutionup(x, y) || game->map.has_trail(x, y))
			{
				tiles.set_frame(game->map.get_tile(x, y));
				draw_billboard(gc, tiles, world_to_projection, (float)x, (float)y);
			}
		}
	}

	if (game->player)
	{
		int direction = game->player->get_move_dir();
		draw_billboard(gc, pacman[direction], world_to_projection, game->player->get_x(), game->player->get_y());
	}

	for (size_t i = 0; i < game->ghosts.size(); i++)
	{
		int color = game->ghosts[i]->is_red_ghost() ? 1 : 0;
		draw_billboard(gc, ghost[color], world_to_projection, game->ghosts[i]->get_x(), game->ghosts[i]->get_y());
	}
}

void Age3D::draw_billboard(CL_GraphicContext &gc, CL_Sprite &sprite, const CL_Mat4f &world_to_projection, float x, float y)
{
	CL_Vec4f position_in_world = CL_Vec4f(x + 0.5f, 0.5f, y + 0.5f, 1.0f);
	CL_Vec2f center = project(gc, world_to_projection, position_in_world);
	CL_Vec2f corner = project(gc, world_to_projection, position_in_world + CL_Vec4f(1.0f, 0.0f, 1.0f, 0.0f));
	float scale = cl_max(std::abs(corner.x - center.x), std::abs(corner.y - center.y)) / 30.0f;
	sprite.set_scale(scale, scale);
	sprite.draw(gc, center.x, center.y);
}

CL_Vec2f Age3D::project(CL_GraphicContext &gc, const CL_Mat4f &world_to_projection, CL_Vec4f position_in_world)
{
	CL_Vec4f position_in_projection = world_to_projection * position_in_world;
	CL_Vec2f projected(position_in_projection.x / position_in_projection.w, position_in_projection.y / position_in_projection.w);
	CL_Vec2f screen((projected.x * 0.5f + 0.5f) * gc.get_width(), (0.5f + projected.y * 0.5f) * gc.get_height());
	return screen;
}

std::vector<Age3D::WallVertex> Age3D::create_walls()
{
	int width = game->map.get_width();
	int height = game->map.get_height();
	std::vector<WallVertex> v;

//	write_wall(v, CL_Vec3i(-10, 0, -10), CL_Vec3i(width+10, 0, height+10), CL_Vec3f(0.0f, 1.0f, 0.0f));
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (game->map.has_wall(x,y))
			{
				bool west_wall = game->map.has_wall(x-1, y);
				bool east_wall = game->map.has_wall(x+1, y);
				bool south_wall = game->map.has_wall(x, y-1);
				bool north_wall = game->map.has_wall(x, y+1);

				float x1 = (float)x;
				float x2 = (float)(x+1);
				float y1 = (float)y;
				float y2 = (float)(y+1);
				if (((east_wall || west_wall) && !north_wall && !south_wall) ||
					((north_wall || south_wall) && !west_wall && !east_wall) ||
					(!east_wall && !west_wall && !north_wall && !south_wall))
				{
					if (!west_wall)
						x1 += 0.25f;
					if (!east_wall)
						x2 -= 0.25f;
					if (!south_wall)
						y1 += 0.25f;
					if (!north_wall)
						y2 -= 0.25f;
					
					write_wall(v, CL_Vec3f(x1, 1, y1), CL_Vec3f(x2, 1, y2), CL_Vec3f(0.0f, 1.0f, 0.0f));
					if (!west_wall)
						write_wall(v, CL_Vec3f(x1, 1, y2), CL_Vec3f(x1, 0, y1), CL_Vec3f(-1.0f, 0.0f,  0.0f));
					if (!south_wall)
						write_wall(v, CL_Vec3f(x2, 1, y1), CL_Vec3f(x1, 0, y1), CL_Vec3f( 0.0f, 0.0f, -1.0f));
					if (!east_wall)
						write_wall(v, CL_Vec3f(x2, 1, y1), CL_Vec3f(x2, 0, y2), CL_Vec3f( 1.0f, 0.0f,  0.0f));
					if (!north_wall)
						write_wall(v, CL_Vec3f(x1, 1, y2), CL_Vec3f(x2, 0, y2), CL_Vec3f( 0.0f, 0.0f,  1.0f));
				}
				else if (west_wall && east_wall && north_wall && !south_wall)
				{
					write_wall(v, CL_Vec3f(x1, 1, y1+0.25f), CL_Vec3f(x2, 1, y2-0.25f), CL_Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2-0.25f), CL_Vec3f(x2-0.25f, 1, y2), CL_Vec3f( 0.0f, 1.0f, 0.0f));

					write_wall(v, CL_Vec3f(x2, 1, y1+0.25f), CL_Vec3f(x1, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));
					write_wall(v, CL_Vec3f(x1, 1, y2-0.25f), CL_Vec3f(x1+0.25f, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y2-0.25f), CL_Vec3f(x2, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));

					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2), CL_Vec3f(x1+0.25f, 0, y2-0.25f), CL_Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y2-0.25f), CL_Vec3f(x2-0.25f, 0, y2), CL_Vec3f( 1.0f, 0.0f,  0.0f));
				}
				else if (west_wall && east_wall && !north_wall && south_wall)
				{
					write_wall(v, CL_Vec3f(x1, 1, y1+0.25f), CL_Vec3f(x2, 1, y2-0.25f), CL_Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1), CL_Vec3f(x2-0.25f, 1, y1+0.25f), CL_Vec3f( 0.0f, 1.0f, 0.0f));

					write_wall(v, CL_Vec3f(x1, 1, y2-0.25f), CL_Vec3f(x2, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1+0.25f), CL_Vec3f(x1, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));
					write_wall(v, CL_Vec3f(x2, 1, y1+0.25f), CL_Vec3f(x2-0.25f, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));

					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1+0.25f), CL_Vec3f(x1+0.25f, 0, y1), CL_Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y1), CL_Vec3f(x2-0.25f, 0, y1+0.25f), CL_Vec3f( 1.0f, 0.0f,  0.0f));
				}
				else if (!west_wall && east_wall && north_wall && south_wall)
				{
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1), CL_Vec3f(x2-0.25f, 1, y2), CL_Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y1+0.25f), CL_Vec3f(x2, 1, y2-0.25f), CL_Vec3f( 0.0f, 1.0f, 0.0f));

					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2), CL_Vec3f(x1+0.25f, 0, y1), CL_Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y1), CL_Vec3f(x2-0.25f, 0, y1+0.25f), CL_Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y2-0.25f), CL_Vec3f(x2-0.25f, 0, y2), CL_Vec3f( 1.0f, 0.0f,  0.0f));

					write_wall(v, CL_Vec3f(x2-0.25f, 1, y2-0.25f), CL_Vec3f(x2, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, CL_Vec3f(x2, 1, y1+0.25f), CL_Vec3f(x2-0.25f, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));
				}
				else if (west_wall && !east_wall && north_wall && south_wall)
				{
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1), CL_Vec3f(x2-0.25f, 1, y2), CL_Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, CL_Vec3f(x1, 1, y1+0.25f), CL_Vec3f(x1+0.25f, 1, y2-0.25f), CL_Vec3f( 0.0f, 1.0f, 0.0f));

					write_wall(v, CL_Vec3f(x2-0.25f, 1, y1), CL_Vec3f(x2-0.25f, 0, y2), CL_Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1+0.25f), CL_Vec3f(x1+0.25f, 0, y1), CL_Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2), CL_Vec3f(x1+0.25f, 0, y2-0.25f), CL_Vec3f(-1.0f, 0.0f,  0.0f));

					write_wall(v, CL_Vec3f(x1, 1, y2-0.25f), CL_Vec3f(x1+0.25f, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1+0.25f), CL_Vec3f(x1, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));
				}
				else if (west_wall && east_wall && north_wall && south_wall)
				{
					write_wall(v, CL_Vec3f(x1, 1, y1+0.25f), CL_Vec3f(x2, 1, y2-0.25f), CL_Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1), CL_Vec3f(x2-0.25f, 1, y1+0.25f), CL_Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2-0.25f), CL_Vec3f(x2-0.25f, 1, y2), CL_Vec3f(0.0f, 1.0f, 0.0f));

					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1+0.25f), CL_Vec3f(x1+0.25f, 0, y1), CL_Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2), CL_Vec3f(x1+0.25f, 0, y2-0.25f), CL_Vec3f(-1.0f, 0.0f,  0.0f));

					write_wall(v, CL_Vec3f(x2-0.25f, 1, y1), CL_Vec3f(x2-0.25f, 0, y1+0.25f), CL_Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y2-0.25f), CL_Vec3f(x2-0.25f, 0, y2), CL_Vec3f( 1.0f, 0.0f,  0.0f));

					write_wall(v, CL_Vec3f(x1, 1, y2-0.25f), CL_Vec3f(x1+0.25f, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y2-0.25f), CL_Vec3f(x2, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));

					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1+0.25f), CL_Vec3f(x1, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));
					write_wall(v, CL_Vec3f(x2, 1, y1+0.25f), CL_Vec3f(x2-0.25f, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));
				}
				else if (west_wall && north_wall)
				{
					write_wall(v, CL_Vec3f(x1, 1, y1+0.25f), CL_Vec3f(x2-0.25f, 1, y2-0.25f), CL_Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2-0.25f), CL_Vec3f(x2-0.25f, 1, y2), CL_Vec3f(0.0f, 1.0f, 0.0f));

					write_wall(v, CL_Vec3f(x2-0.25f, 1, y1+0.25f), CL_Vec3f(x1, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y1+0.25f), CL_Vec3f(x2-0.25f, 0, y2), CL_Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x1, 1, y2-0.25f), CL_Vec3f(x1+0.25f, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2), CL_Vec3f(x1+0.25f, 0, y2-0.25f), CL_Vec3f(-1.0f, 0.0f,  0.0f));
				}
				else if (east_wall && north_wall)
				{
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1+0.25f), CL_Vec3f(x2, 1, y2-0.25f), CL_Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2-0.25f), CL_Vec3f(x2-0.25f, 1, y2), CL_Vec3f(0.0f, 1.0f, 0.0f));

					write_wall(v, CL_Vec3f(x2, 1, y1+0.25f), CL_Vec3f(x1+0.25f, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2), CL_Vec3f(x1+0.25f, 0, y1+0.25f), CL_Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y2-0.25f), CL_Vec3f(x2, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y2-0.25f), CL_Vec3f(x2-0.25f, 0, y2), CL_Vec3f( 1.0f, 0.0f,  0.0f));
				}
				else if (west_wall && south_wall)
				{
					write_wall(v, CL_Vec3f(x1, 1, y1+0.25f), CL_Vec3f(x2-0.25f, 1, y2-0.25f), CL_Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1), CL_Vec3f(x2-0.25f, 1, y1+0.25f), CL_Vec3f(0.0f, 1.0f, 0.0f));

					write_wall(v, CL_Vec3f(x1, 1, y2-0.25f), CL_Vec3f(x2-0.25f, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y1), CL_Vec3f(x2-0.25f, 0, y2-0.25f), CL_Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1+0.25f), CL_Vec3f(x1+0.25f, 0, y1), CL_Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1+0.25f), CL_Vec3f(x1, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));
				}
				else if (east_wall && south_wall)
				{
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1+0.25f), CL_Vec3f(x2, 1, y2-0.25f), CL_Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y1), CL_Vec3f(x2-0.25f, 1, y1+0.25f), CL_Vec3f(0.0f, 1.0f, 0.0f));

					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2-0.25f), CL_Vec3f(x2, 0, y2-0.25f), CL_Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, CL_Vec3f(x1+0.25f, 1, y2-0.25f), CL_Vec3f(x1+0.25f, 0, y1), CL_Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x2-0.25f, 1, y1), CL_Vec3f(x2-0.25f, 0, y1+0.25f), CL_Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, CL_Vec3f(x2, 1, y1+0.25f), CL_Vec3f(x2-0.25f, 0, y1+0.25f), CL_Vec3f( 0.0f, 0.0f, -1.0f));
				}
			}
		}
	}
	return v;
}

void Age3D::write_wall(std::vector<WallVertex> &in_out_vertices, const CL_Vec3f &p1, const CL_Vec3f &p2, const CL_Vec3f &normal)
{
	WallVertex v[6];

	float uv_scale = 0.01f;

	if (p1.y == p2.y)
	{
		v[0].position = CL_Vec3f(p1.x, p1.y, p2.z);
		v[1].position = CL_Vec3f(p1.x, p1.y, p1.z);
		v[2].position = CL_Vec3f(p2.x, p1.y, p1.z);
		v[3].position = CL_Vec3f(p2.x, p1.y, p1.z);
		v[4].position = CL_Vec3f(p2.x, p1.y, p2.z);
		v[5].position = CL_Vec3f(p1.x, p1.y, p2.z);

		for (int i = 0; i < 6; i++)
			v[i].texcoord0 = CL_Vec2f(v[i].position.x * uv_scale, v[i].position.z * uv_scale);
	}
	else if (p1.x == p2.x)
	{
		v[0].position = CL_Vec3f(p1.x, p1.y, p2.z);
		v[1].position = CL_Vec3f(p1.x, p1.y, p1.z);
		v[2].position = CL_Vec3f(p1.x, p2.y, p1.z);
		v[3].position = CL_Vec3f(p1.x, p2.y, p1.z);
		v[4].position = CL_Vec3f(p1.x, p2.y, p2.z);
		v[5].position = CL_Vec3f(p1.x, p1.y, p2.z);

		for (int i = 0; i < 6; i++)
			v[i].texcoord0 = CL_Vec2f(v[i].position.y * uv_scale, v[i].position.z * uv_scale);
	}
	else if (p1.z == p2.z)
	{
		v[0].position = CL_Vec3f(p1.x, p2.y, p1.z);
		v[1].position = CL_Vec3f(p1.x, p1.y, p1.z);
		v[2].position = CL_Vec3f(p2.x, p1.y, p1.z);
		v[3].position = CL_Vec3f(p2.x, p1.y, p1.z);
		v[4].position = CL_Vec3f(p2.x, p2.y, p1.z);
		v[5].position = CL_Vec3f(p1.x, p2.y, p1.z);

		for (int i = 0; i < 6; i++)
			v[i].texcoord0 = CL_Vec2f(v[i].position.x * uv_scale, v[i].position.y * uv_scale);
	}
	else
	{
		throw CL_Exception(L"write_wall misuse!");
	}

	for (int i = 0; i < 6; i++)
	{
		v[i].normal = normal;
	}
	
	for (int i = 0; i < 6; i++)
		in_out_vertices.push_back(v[i]);
}

void Age3D::create_billboards(CL_GraphicContext &gc)
{
	CL_SpriteDescription left_pacman_desc;
	left_pacman_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 0, 0, 28, 32, 6, 1);
	pacman[0] = CL_Sprite(gc, left_pacman_desc);
	pacman[0].set_alignment(origin_center);
	pacman[0].set_play_pingpong();

	CL_SpriteDescription right_pacman_desc;
	right_pacman_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 28*6, 0, 28, 32, 6, 1);
	pacman[1] = CL_Sprite(gc, right_pacman_desc);
	pacman[1].set_alignment(origin_center);
	pacman[1].set_play_pingpong();

	CL_SpriteDescription up_pacman_desc;
	up_pacman_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 0, 32, 28, 32, 6, 1);
	pacman[2] = CL_Sprite(gc, up_pacman_desc);
	pacman[2].set_alignment(origin_center);
	pacman[2].set_play_pingpong();

	CL_SpriteDescription down_pacman_desc;
	down_pacman_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 28*6, 32, 28, 32, 6, 1);
	pacman[3] = CL_Sprite(gc, down_pacman_desc);
	pacman[3].set_alignment(origin_center);
	pacman[3].set_play_pingpong();

	CL_SpriteDescription green_ghost_desc;
	green_ghost_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 336, 0, 28, 32, 6, 1);
	ghost[0] = CL_Sprite(gc, green_ghost_desc);
	ghost[0].set_alignment(origin_center);
	ghost[0].set_play_pingpong();
	ghost[0].set_delay(100);

	CL_SpriteDescription red_ghost_desc;
	red_ghost_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 336, 32, 28, 32, 6, 1);
	ghost[1] = CL_Sprite(gc, red_ghost_desc);
	ghost[1].set_alignment(origin_center);
	ghost[1].set_play_pingpong();
	ghost[1].set_delay(100);

	CL_SpriteDescription tiles_desc;
	tiles_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 0, 64, 34, 38, 10, 2);
	tiles = CL_Sprite(gc, tiles_desc);
	tiles.set_alignment(origin_center);
}
