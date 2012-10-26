
#include "precomp.h"
#include "game_screen.h"

GameScreen::GameScreen(CL_GUIManager *gui)
: UIScreen(gui)
{
	func_render().set(this, &GameScreen::on_render);

	CL_GraphicContext gc = get_gc();
	load_skybox(gc);
}

GameScreen::~GameScreen()
{
}

void GameScreen::on_render(CL_GraphicContext &gc, const CL_Rect &rect)
{
	render_skybox(gc);
}

void GameScreen::load_skybox(CL_GraphicContext &gc)
{
	CL_PixelBuffer pb_positive_x = CL_ImageProviderFactory::load(L"Resources/GameScreen/Skybox/ice_field_90.png");
	CL_PixelBuffer pb_negative_x = CL_ImageProviderFactory::load(L"Resources/GameScreen/Skybox/ice_field_270.png");
	CL_PixelBuffer pb_positive_y = CL_ImageProviderFactory::load(L"Resources/GameScreen/Skybox/ice_field_top.png");
	CL_PixelBuffer pb_negative_y = CL_ImageProviderFactory::load(L"Resources/GameScreen/Skybox/ice_field_bottom.png");
	CL_PixelBuffer pb_positive_z = CL_ImageProviderFactory::load(L"Resources/GameScreen/Skybox/ice_field_0.png");
	CL_PixelBuffer pb_negative_z = CL_ImageProviderFactory::load(L"Resources/GameScreen/Skybox/ice_field_180.png");

	skybox_program = CL_ProgramObject::load_and_link(gc, "Resources/GameScreen/Skybox/vertex.glsl", "Resources/GameScreen/Skybox/fragment.glsl");
	skybox_texture = CL_Texture(gc, cl_texture_cube_map);
	skybox_texture.set_cube_map(
		pb_positive_x,
		pb_negative_x,
		pb_positive_y,
		pb_negative_y,
		pb_positive_z,
		pb_negative_z);
	skybox_texture.set_min_filter(cl_filter_linear);
	skybox_texture.set_mag_filter(cl_filter_linear);
}

void GameScreen::render_skybox(CL_GraphicContext &gc)
{
	gc.set_map_mode(cl_user_projection);
	gc.set_projection(CL_Mat4f::perspective(60.0f, get_width()/(float)get_height(), 0.1f, 10.0f));
	gc.set_modelview(CL_Mat4f::identity());

	gc.set_texture(0, skybox_texture);
	gc.set_program_object(skybox_program);
	skybox_program.set_uniform1i("texture1", 0);

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, skybox_positions);
	gc.draw_primitives(cl_triangles, 6*6, prim_array);

	gc.reset_program_object();
	gc.reset_texture(0);

	gc.set_map_mode(cl_map_2d_upper_left);
}

CL_Vec3f GameScreen::skybox_positions[6*6] =
{
	CL_Vec3f(-0.5f,  0.5f,  0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f( 0.5f, -0.5f,  0.5f),

	CL_Vec3f( 0.5f, -0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f,  0.5f),
	CL_Vec3f(-0.5f,  0.5f,  0.5f),


	CL_Vec3f( 0.5f, -0.5f, -0.5f),
	CL_Vec3f( 0.5f,  0.5f, -0.5f),
	CL_Vec3f(-0.5f,  0.5f, -0.5f),

	CL_Vec3f(-0.5f,  0.5f, -0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f),
	CL_Vec3f( 0.5f, -0.5f, -0.5f),


	CL_Vec3f( 0.5f,  0.5f, -0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f,  0.5f,  0.5f),

	CL_Vec3f(-0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f,  0.5f, -0.5f),
	CL_Vec3f( 0.5f,  0.5f, -0.5f),


	CL_Vec3f(-0.5f, -0.5f,  0.5f),
	CL_Vec3f( 0.5f, -0.5f,  0.5f),
	CL_Vec3f( 0.5f, -0.5f, -0.5f),

	CL_Vec3f( 0.5f, -0.5f, -0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f),
	CL_Vec3f(-0.5f, -0.5f,  0.5f),


	CL_Vec3f( 0.5f, -0.5f,  0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f( 0.5f,  0.5f, -0.5f),

	CL_Vec3f( 0.5f,  0.5f, -0.5f),
	CL_Vec3f( 0.5f, -0.5f, -0.5f),
	CL_Vec3f( 0.5f, -0.5f,  0.5f),


	CL_Vec3f(-0.5f,  0.5f, -0.5f),
	CL_Vec3f(-0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f,  0.5f),

	CL_Vec3f(-0.5f, -0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f),
	CL_Vec3f(-0.5f,  0.5f, -0.5f)
};
