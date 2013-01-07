
#include "precomp.h"
#include "age_3d.h"
#include "game_world.h"

Age3D::Age3D(clan::Canvas &canvas, GameWorld *game)
: game(game), num_wall_vertices()
{
	std::vector<Age3D::WallVertex> cpu_wall_vertices = create_walls();
	num_wall_vertices = cpu_wall_vertices.size();

	wall_vertices = clan::VertexArrayVector<WallVertex>(canvas, cpu_wall_vertices);

	walls_prim_array = clan::PrimitivesArray(canvas);
	walls_prim_array.set_attributes(0, wall_vertices, cl_offsetof(WallVertex, position));
	walls_prim_array.set_attributes(1, wall_vertices, cl_offsetof(WallVertex, normal));
	walls_prim_array.set_attributes(2, wall_vertices, cl_offsetof(WallVertex, texcoord0));

	walls_program = clan::ProgramObject::load(canvas, "vertex_walls.glsl", "fragment_walls.glsl");
	walls_program.bind_attribute_location(0, "AttrPositionInObject");
	walls_program.bind_attribute_location(1, "AttrNormalInObject");
	walls_program.bind_attribute_location(2, "AttrTexCoord");
	walls_program.bind_frag_data_location(0, "FragColor");
	if (!walls_program.link())
		throw clan::Exception("Walls program link failed");
	walls_program.set_uniform_buffer_index("Uniforms", 0);
	walls_program.set_uniform1i("Texture", 0);
	walls_program.set_uniform1i("TextureSampler", 0);

	wall_texture = clan::Texture2D(canvas, "wall.jpg");
	wall_texture.generate_mipmap();
	wall_texture.set_wrap_mode(clan::wrap_repeat, clan::wrap_repeat);
	wall_texture.set_min_filter(clan::filter_linear_mipmap_linear);
	wall_texture.set_mag_filter(clan::filter_linear);

	uniforms = clan::UniformVector<Uniforms>(canvas, 1);

	clan::RasterizerStateDescription raster_desc;
	raster_desc.set_culled(true);
	raster_desc.set_front_face(clan::face_counter_clockwise);
	raster_state = clan::RasterizerState(canvas, raster_desc);

	clan::DepthStencilStateDescription depth_desc;
	depth_desc.enable_depth_test(true);
	depth_desc.enable_depth_write(true);
	depth_desc.set_depth_compare_function(clan::compare_lequal);
	depth_stencil_state = clan::DepthStencilState(canvas, depth_desc);

	final_texture = clan::Texture2D(canvas, canvas.get_width(), canvas.get_height(), clan::tf_rgba8);

	depth_buffer = clan::RenderBuffer(canvas, canvas.get_width(), canvas.get_height(), clan::tf_depth_component32f);
	fb = clan::FrameBuffer(canvas);
	fb.attach_color(0, final_texture);
	fb.attach_depth(depth_buffer);

	create_billboards(canvas);
}

void Age3D::render(clan::Canvas &canvas)
{
	if (game->player)
		look_at = clan::Vec2f(game->player->get_x(), game->player->get_y());

	float dir = 0.0f;
	float up = 90.0f;
	float tilt = 0.0f;
	clan::Vec3f position(look_at.x + 0.5f, 20.0f, look_at.y + 0.5f);
	clan::Quaternionf orientation = clan::Quaternionf(up, dir, tilt, clan::angle_degrees, clan::order_YXZ);
	orientation.inverse();

	Uniforms cpu_uniforms;
	cpu_uniforms.object_to_world = clan::Mat4f::identity();
	cpu_uniforms.world_to_eye = orientation.to_matrix() * clan::Mat4f::translate(-position);
	cpu_uniforms.eye_to_projection = clan::Mat4f::perspective(60.0f, canvas.get_width()/(float)canvas.get_height(), 1.0f, 1e+9, clan::handed_left, canvas.get_gc().get_clip_z_range());
	clan::Mat3f object_normal_to_eye = clan::Mat3f(cpu_uniforms.world_to_eye * cpu_uniforms.object_to_world); // Assumes uniform scale
	cpu_uniforms.object_normal_to_eye[0] = clan::Vec4f(object_normal_to_eye[0], object_normal_to_eye[1], object_normal_to_eye[2], 0.0f);
	cpu_uniforms.object_normal_to_eye[1] = clan::Vec4f(object_normal_to_eye[3], object_normal_to_eye[4], object_normal_to_eye[5], 0.0f);
	cpu_uniforms.object_normal_to_eye[2] = clan::Vec4f(object_normal_to_eye[6], object_normal_to_eye[7], object_normal_to_eye[8], 0.0f);
	uniforms.upload_data(canvas, &cpu_uniforms, 1);

	clan::Canvas canvas_fb(canvas, fb);
	clan::GraphicContext gc_fb = canvas_fb.get_gc();
	gc_fb.clear(clan::Colorf::black);
	gc_fb.clear_depth(1.0f);
	gc_fb.set_rasterizer_state(raster_state);
	gc_fb.set_depth_stencil_state(depth_stencil_state);
	gc_fb.set_program_object(walls_program);
	gc_fb.set_uniform_buffer(0, uniforms);
	gc_fb.set_texture(0, wall_texture);
	gc_fb.draw_primitives(clan::type_triangles, num_wall_vertices, walls_prim_array);
	gc_fb.reset_texture(0);
	gc_fb.reset_uniform_buffer(0);
	gc_fb.reset_program_object();
	gc_fb.reset_depth_stencil_state();
	gc_fb.reset_rasterizer_state();

	clan::OpenGL::check_error();

	clan::Image image(canvas, final_texture, final_texture.get_size());
	image.draw(canvas, 0, 0);

	// Draw billboards on top:

	for (int i = 0; i < 4; i++)
		pacman[i].update();
	for (int i = 0; i < 2; i++)
		ghost[i].update();

	clan::Mat4f world_to_projection = cpu_uniforms.eye_to_projection * cpu_uniforms.world_to_eye;

	for (int y = 0; y < game->map.get_height(); y++)
	{
		for (int x = 0; x < game->map.get_width(); x++)
		{
			if (game->map.has_egg(x, y) || game->map.has_powerup(x, y) || game->map.has_evolutionup(x, y) || game->map.has_trail(x, y))
			{
				tiles.set_frame(game->map.get_tile(x, y));
				draw_billboard(canvas, tiles, world_to_projection, (float)x, (float)y);
			}
		}
	}

	if (game->player)
	{
		int direction = game->player->get_move_dir();
		draw_billboard(canvas, pacman[direction], world_to_projection, game->player->get_x(), game->player->get_y());
	}

	for (size_t i = 0; i < game->ghosts.size(); i++)
	{
		int color = game->ghosts[i]->is_red_ghost() ? 1 : 0;
		draw_billboard(canvas, ghost[color], world_to_projection, game->ghosts[i]->get_x(), game->ghosts[i]->get_y());
	}
}

void Age3D::draw_billboard(clan::Canvas &canvas, clan::Sprite &sprite, const clan::Mat4f &world_to_projection, float x, float y)
{
	clan::Vec4f position_in_world = clan::Vec4f(x + 0.5f, 0.5f, y + 0.5f, 1.0f);
	clan::Vec2f center = project(canvas, world_to_projection, position_in_world);
	clan::Vec2f corner = project(canvas, world_to_projection, position_in_world + clan::Vec4f(1.0f, 0.0f, 1.0f, 0.0f));
	float scale = clan::max(std::abs(corner.x - center.x), std::abs(corner.y - center.y)) / 30.0f;
	sprite.set_scale(scale, scale);
	sprite.draw(canvas, center.x, center.y);
}

clan::Vec2f Age3D::project(clan::GraphicContext &gc, const clan::Mat4f &world_to_projection, clan::Vec4f position_in_world)
{
	clan::Vec4f position_in_projection = world_to_projection * position_in_world;
	clan::Vec2f projected(position_in_projection.x / position_in_projection.w, position_in_projection.y / position_in_projection.w);
	clan::Vec2f screen((projected.x * 0.5f + 0.5f) * gc.get_width(), (0.5f + projected.y * 0.5f) * gc.get_height());
	return screen;
}

std::vector<Age3D::WallVertex> Age3D::create_walls()
{
	int width = game->map.get_width();
	int height = game->map.get_height();
	std::vector<WallVertex> v;

//	write_wall(v, clan::Vec3i(-10, 0, -10), clan::Vec3i(width+10, 0, height+10), clan::Vec3f(0.0f, 1.0f, 0.0f));
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
					
					write_wall(v, clan::Vec3f(x1, 1, y1), clan::Vec3f(x2, 1, y2), clan::Vec3f(0.0f, 1.0f, 0.0f));
					if (!west_wall)
						write_wall(v, clan::Vec3f(x1, 1, y2), clan::Vec3f(x1, 0, y1), clan::Vec3f(-1.0f, 0.0f,  0.0f));
					if (!south_wall)
						write_wall(v, clan::Vec3f(x2, 1, y1), clan::Vec3f(x1, 0, y1), clan::Vec3f( 0.0f, 0.0f, -1.0f));
					if (!east_wall)
						write_wall(v, clan::Vec3f(x2, 1, y1), clan::Vec3f(x2, 0, y2), clan::Vec3f( 1.0f, 0.0f,  0.0f));
					if (!north_wall)
						write_wall(v, clan::Vec3f(x1, 1, y2), clan::Vec3f(x2, 0, y2), clan::Vec3f( 0.0f, 0.0f,  1.0f));
				}
				else if (west_wall && east_wall && north_wall && !south_wall)
				{
					write_wall(v, clan::Vec3f(x1, 1, y1+0.25f), clan::Vec3f(x2, 1, y2-0.25f), clan::Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2-0.25f), clan::Vec3f(x2-0.25f, 1, y2), clan::Vec3f( 0.0f, 1.0f, 0.0f));

					write_wall(v, clan::Vec3f(x2, 1, y1+0.25f), clan::Vec3f(x1, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));
					write_wall(v, clan::Vec3f(x1, 1, y2-0.25f), clan::Vec3f(x1+0.25f, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y2-0.25f), clan::Vec3f(x2, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));

					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2), clan::Vec3f(x1+0.25f, 0, y2-0.25f), clan::Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y2-0.25f), clan::Vec3f(x2-0.25f, 0, y2), clan::Vec3f( 1.0f, 0.0f,  0.0f));
				}
				else if (west_wall && east_wall && !north_wall && south_wall)
				{
					write_wall(v, clan::Vec3f(x1, 1, y1+0.25f), clan::Vec3f(x2, 1, y2-0.25f), clan::Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1), clan::Vec3f(x2-0.25f, 1, y1+0.25f), clan::Vec3f( 0.0f, 1.0f, 0.0f));

					write_wall(v, clan::Vec3f(x1, 1, y2-0.25f), clan::Vec3f(x2, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1+0.25f), clan::Vec3f(x1, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));
					write_wall(v, clan::Vec3f(x2, 1, y1+0.25f), clan::Vec3f(x2-0.25f, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));

					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1+0.25f), clan::Vec3f(x1+0.25f, 0, y1), clan::Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y1), clan::Vec3f(x2-0.25f, 0, y1+0.25f), clan::Vec3f( 1.0f, 0.0f,  0.0f));
				}
				else if (!west_wall && east_wall && north_wall && south_wall)
				{
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1), clan::Vec3f(x2-0.25f, 1, y2), clan::Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y1+0.25f), clan::Vec3f(x2, 1, y2-0.25f), clan::Vec3f( 0.0f, 1.0f, 0.0f));

					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2), clan::Vec3f(x1+0.25f, 0, y1), clan::Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y1), clan::Vec3f(x2-0.25f, 0, y1+0.25f), clan::Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y2-0.25f), clan::Vec3f(x2-0.25f, 0, y2), clan::Vec3f( 1.0f, 0.0f,  0.0f));

					write_wall(v, clan::Vec3f(x2-0.25f, 1, y2-0.25f), clan::Vec3f(x2, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, clan::Vec3f(x2, 1, y1+0.25f), clan::Vec3f(x2-0.25f, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));
				}
				else if (west_wall && !east_wall && north_wall && south_wall)
				{
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1), clan::Vec3f(x2-0.25f, 1, y2), clan::Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, clan::Vec3f(x1, 1, y1+0.25f), clan::Vec3f(x1+0.25f, 1, y2-0.25f), clan::Vec3f( 0.0f, 1.0f, 0.0f));

					write_wall(v, clan::Vec3f(x2-0.25f, 1, y1), clan::Vec3f(x2-0.25f, 0, y2), clan::Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1+0.25f), clan::Vec3f(x1+0.25f, 0, y1), clan::Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2), clan::Vec3f(x1+0.25f, 0, y2-0.25f), clan::Vec3f(-1.0f, 0.0f,  0.0f));

					write_wall(v, clan::Vec3f(x1, 1, y2-0.25f), clan::Vec3f(x1+0.25f, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1+0.25f), clan::Vec3f(x1, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));
				}
				else if (west_wall && east_wall && north_wall && south_wall)
				{
					write_wall(v, clan::Vec3f(x1, 1, y1+0.25f), clan::Vec3f(x2, 1, y2-0.25f), clan::Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1), clan::Vec3f(x2-0.25f, 1, y1+0.25f), clan::Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2-0.25f), clan::Vec3f(x2-0.25f, 1, y2), clan::Vec3f(0.0f, 1.0f, 0.0f));

					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1+0.25f), clan::Vec3f(x1+0.25f, 0, y1), clan::Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2), clan::Vec3f(x1+0.25f, 0, y2-0.25f), clan::Vec3f(-1.0f, 0.0f,  0.0f));

					write_wall(v, clan::Vec3f(x2-0.25f, 1, y1), clan::Vec3f(x2-0.25f, 0, y1+0.25f), clan::Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y2-0.25f), clan::Vec3f(x2-0.25f, 0, y2), clan::Vec3f( 1.0f, 0.0f,  0.0f));

					write_wall(v, clan::Vec3f(x1, 1, y2-0.25f), clan::Vec3f(x1+0.25f, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y2-0.25f), clan::Vec3f(x2, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));

					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1+0.25f), clan::Vec3f(x1, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));
					write_wall(v, clan::Vec3f(x2, 1, y1+0.25f), clan::Vec3f(x2-0.25f, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));
				}
				else if (west_wall && north_wall)
				{
					write_wall(v, clan::Vec3f(x1, 1, y1+0.25f), clan::Vec3f(x2-0.25f, 1, y2-0.25f), clan::Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2-0.25f), clan::Vec3f(x2-0.25f, 1, y2), clan::Vec3f(0.0f, 1.0f, 0.0f));

					write_wall(v, clan::Vec3f(x2-0.25f, 1, y1+0.25f), clan::Vec3f(x1, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y1+0.25f), clan::Vec3f(x2-0.25f, 0, y2), clan::Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x1, 1, y2-0.25f), clan::Vec3f(x1+0.25f, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2), clan::Vec3f(x1+0.25f, 0, y2-0.25f), clan::Vec3f(-1.0f, 0.0f,  0.0f));
				}
				else if (east_wall && north_wall)
				{
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1+0.25f), clan::Vec3f(x2, 1, y2-0.25f), clan::Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2-0.25f), clan::Vec3f(x2-0.25f, 1, y2), clan::Vec3f(0.0f, 1.0f, 0.0f));

					write_wall(v, clan::Vec3f(x2, 1, y1+0.25f), clan::Vec3f(x1+0.25f, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2), clan::Vec3f(x1+0.25f, 0, y1+0.25f), clan::Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y2-0.25f), clan::Vec3f(x2, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y2-0.25f), clan::Vec3f(x2-0.25f, 0, y2), clan::Vec3f( 1.0f, 0.0f,  0.0f));
				}
				else if (west_wall && south_wall)
				{
					write_wall(v, clan::Vec3f(x1, 1, y1+0.25f), clan::Vec3f(x2-0.25f, 1, y2-0.25f), clan::Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1), clan::Vec3f(x2-0.25f, 1, y1+0.25f), clan::Vec3f(0.0f, 1.0f, 0.0f));

					write_wall(v, clan::Vec3f(x1, 1, y2-0.25f), clan::Vec3f(x2-0.25f, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y1), clan::Vec3f(x2-0.25f, 0, y2-0.25f), clan::Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1+0.25f), clan::Vec3f(x1+0.25f, 0, y1), clan::Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1+0.25f), clan::Vec3f(x1, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));
				}
				else if (east_wall && south_wall)
				{
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1+0.25f), clan::Vec3f(x2, 1, y2-0.25f), clan::Vec3f(0.0f, 1.0f, 0.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y1), clan::Vec3f(x2-0.25f, 1, y1+0.25f), clan::Vec3f(0.0f, 1.0f, 0.0f));

					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2-0.25f), clan::Vec3f(x2, 0, y2-0.25f), clan::Vec3f( 0.0f, 0.0f,  1.0f));
					write_wall(v, clan::Vec3f(x1+0.25f, 1, y2-0.25f), clan::Vec3f(x1+0.25f, 0, y1), clan::Vec3f(-1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x2-0.25f, 1, y1), clan::Vec3f(x2-0.25f, 0, y1+0.25f), clan::Vec3f( 1.0f, 0.0f,  0.0f));
					write_wall(v, clan::Vec3f(x2, 1, y1+0.25f), clan::Vec3f(x2-0.25f, 0, y1+0.25f), clan::Vec3f( 0.0f, 0.0f, -1.0f));
				}
			}
		}
	}
	return v;
}

void Age3D::write_wall(std::vector<WallVertex> &in_out_vertices, const clan::Vec3f &p1, const clan::Vec3f &p2, const clan::Vec3f &normal)
{
	WallVertex v[6];

	float uv_scale = 0.01f;

	if (p1.y == p2.y)
	{
		v[0].position = clan::Vec3f(p1.x, p1.y, p2.z);
		v[1].position = clan::Vec3f(p1.x, p1.y, p1.z);
		v[2].position = clan::Vec3f(p2.x, p1.y, p1.z);
		v[3].position = clan::Vec3f(p2.x, p1.y, p1.z);
		v[4].position = clan::Vec3f(p2.x, p1.y, p2.z);
		v[5].position = clan::Vec3f(p1.x, p1.y, p2.z);

		for (int i = 0; i < 6; i++)
			v[i].texcoord0 = clan::Vec2f(v[i].position.x * uv_scale, v[i].position.z * uv_scale);
	}
	else if (p1.x == p2.x)
	{
		v[0].position = clan::Vec3f(p1.x, p1.y, p2.z);
		v[1].position = clan::Vec3f(p1.x, p1.y, p1.z);
		v[2].position = clan::Vec3f(p1.x, p2.y, p1.z);
		v[3].position = clan::Vec3f(p1.x, p2.y, p1.z);
		v[4].position = clan::Vec3f(p1.x, p2.y, p2.z);
		v[5].position = clan::Vec3f(p1.x, p1.y, p2.z);

		for (int i = 0; i < 6; i++)
			v[i].texcoord0 = clan::Vec2f(v[i].position.y * uv_scale, v[i].position.z * uv_scale);
	}
	else if (p1.z == p2.z)
	{
		v[0].position = clan::Vec3f(p1.x, p2.y, p1.z);
		v[1].position = clan::Vec3f(p1.x, p1.y, p1.z);
		v[2].position = clan::Vec3f(p2.x, p1.y, p1.z);
		v[3].position = clan::Vec3f(p2.x, p1.y, p1.z);
		v[4].position = clan::Vec3f(p2.x, p2.y, p1.z);
		v[5].position = clan::Vec3f(p1.x, p2.y, p1.z);

		for (int i = 0; i < 6; i++)
			v[i].texcoord0 = clan::Vec2f(v[i].position.x * uv_scale, v[i].position.y * uv_scale);
	}
	else
	{
		throw clan::Exception("write_wall misuse!");
	}

	for (int i = 0; i < 6; i++)
	{
		v[i].normal = normal;
	}
	
	for (int i = 0; i < 6; i++)
		in_out_vertices.push_back(v[i]);
}

void Age3D::create_billboards(clan::GraphicContext &gc)
{
	clan::SpriteDescription left_pacman_desc;
	left_pacman_desc.add_gridclipped_frames(clan::PNGProvider::load("pacman.png"), 0, 0, 28, 32, 6, 1);
	pacman[0] = clan::Sprite(gc, left_pacman_desc);
	pacman[0].set_alignment(clan::origin_center);
	pacman[0].set_play_pingpong();

	clan::SpriteDescription right_pacman_desc;
	right_pacman_desc.add_gridclipped_frames(clan::PNGProvider::load("pacman.png"), 28*6, 0, 28, 32, 6, 1);
	pacman[1] = clan::Sprite(gc, right_pacman_desc);
	pacman[1].set_alignment(clan::origin_center);
	pacman[1].set_play_pingpong();

	clan::SpriteDescription up_pacman_desc;
	up_pacman_desc.add_gridclipped_frames(clan::PNGProvider::load("pacman.png"), 0, 32, 28, 32, 6, 1);
	pacman[2] = clan::Sprite(gc, up_pacman_desc);
	pacman[2].set_alignment(clan::origin_center);
	pacman[2].set_play_pingpong();

	clan::SpriteDescription down_pacman_desc;
	down_pacman_desc.add_gridclipped_frames(clan::PNGProvider::load("pacman.png"), 28*6, 32, 28, 32, 6, 1);
	pacman[3] = clan::Sprite(gc, down_pacman_desc);
	pacman[3].set_alignment(clan::origin_center);
	pacman[3].set_play_pingpong();

	clan::SpriteDescription green_ghost_desc;
	green_ghost_desc.add_gridclipped_frames(clan::PNGProvider::load("pacman.png"), 336, 0, 28, 32, 6, 1);
	ghost[0] = clan::Sprite(gc, green_ghost_desc);
	ghost[0].set_alignment(clan::origin_center);
	ghost[0].set_play_pingpong();
	ghost[0].set_delay(100);

	clan::SpriteDescription red_ghost_desc;
	red_ghost_desc.add_gridclipped_frames(clan::PNGProvider::load("pacman.png"), 336, 32, 28, 32, 6, 1);
	ghost[1] = clan::Sprite(gc, red_ghost_desc);
	ghost[1].set_alignment(clan::origin_center);
	ghost[1].set_play_pingpong();
	ghost[1].set_delay(100);

	clan::SpriteDescription tiles_desc;
	tiles_desc.add_gridclipped_frames(clan::PNGProvider::load("pacman.png"), 0, 64, 34, 38, 10, 2);
	tiles = clan::Sprite(gc, tiles_desc);
	tiles.set_alignment(clan::origin_center);
}
