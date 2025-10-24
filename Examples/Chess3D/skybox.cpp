
#include "precomp.h"
#include "skybox.h"

Skybox::Skybox(CL_GraphicContext &gc)
{
	CL_PixelBuffer pb_positive_x = CL_ImageProviderFactory::load(cl_text("Resources/skybox_x_positive.png"));
	CL_PixelBuffer pb_negative_x = CL_ImageProviderFactory::load(cl_text("Resources/skybox_x_negative.png"));
	CL_PixelBuffer pb_positive_y = CL_ImageProviderFactory::load(cl_text("Resources/skybox_y_positive.png"));
	CL_PixelBuffer pb_negative_y = CL_ImageProviderFactory::load(cl_text("Resources/skybox_y_negative.png"));
	CL_PixelBuffer pb_positive_z = CL_ImageProviderFactory::load(cl_text("Resources/skybox_z_positive.png"));
	CL_PixelBuffer pb_negative_z = CL_ImageProviderFactory::load(cl_text("Resources/skybox_z_negative.png"));

	CL_VirtualDirectory vd;
	program_object = CL_ProgramObject::load_and_link(gc, cl_text("Resources/skybox_vertex.glsl"), cl_text("Resources/skybox_fragment.glsl"), vd);

	skybox_texture = CL_Texture(gc, cl_texture_cube_map);
//	skybox_texture.set_generate_mipmap(true);
	skybox_texture.set_cube_map(
		pb_positive_x,
		pb_negative_x,
		pb_positive_y,
		pb_negative_y,
		pb_positive_z,
		pb_negative_z);
	skybox_texture.set_min_filter(cl_filter_linear);
	skybox_texture.set_mag_filter(cl_filter_linear);
	//skybox_texture.set_min_filter(cl_filter_linear_mipmap_linear);
	//skybox_texture.set_mag_filter(cl_filter_linear_mipmap_linear);
}

Skybox::~Skybox()
{
}

void Skybox::render(CL_GraphicContext &gc, const Camera &camera)
{
	Camera cam = camera;
	cam.get_position().set_position(0,0,0);
	cam.setup_gc(gc, 0.1f, 10.0f);

	gc.set_texture(0, skybox_texture);
	gc.set_program_object(program_object);
	program_object.set_uniform1f(cl_text("texture1"), 0);

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	gc.draw_primitives(cl_quads, 6*4, prim_array);

	gc.reset_program_object();
	gc.reset_texture(0);

}

CL_Vec3f Skybox::positions[6*4] =
{
	CL_Vec3f(-0.5f,  0.5f,  0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f( 0.5f, -0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f,  0.5f),

	CL_Vec3f( 0.5f, -0.5f, -0.5f),
	CL_Vec3f( 0.5f,  0.5f, -0.5f),
	CL_Vec3f(-0.5f,  0.5f, -0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f),

	CL_Vec3f( 0.5f,  0.5f, -0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f,  0.5f, -0.5f),

	CL_Vec3f(-0.5f, -0.5f,  0.5f),
	CL_Vec3f( 0.5f, -0.5f,  0.5f),
	CL_Vec3f( 0.5f, -0.5f, -0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f),

	CL_Vec3f( 0.5f, -0.5f,  0.5f),
	CL_Vec3f( 0.5f,  0.5f,  0.5f),
	CL_Vec3f( 0.5f,  0.5f, -0.5f),
	CL_Vec3f( 0.5f, -0.5f, -0.5f),

	CL_Vec3f(-0.5f,  0.5f, -0.5f),
	CL_Vec3f(-0.5f,  0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f,  0.5f),
	CL_Vec3f(-0.5f, -0.5f, -0.5f)
};
