
#pragma once

#include "orientation.h"
#include "position.h"
#include "light_model.h"

#include "../../../../../3D/Clan3D/Collada/collada.h"
#include "../../../../../3D/Clan3D/Collada/collada_geometry.h"
#include "../../../../../3D/Clan3D/Collada/collada_mesh.h"
#include "../../../../../3D/Clan3D/Collada/collada_triangles.h"
#include "../../../../../3D/Clan3D/Collada/collada_source.h"
#include "../../../../../3D/Clan3D/Collada/collada_input_shared.h"
#include "../../../../../3D/Clan3D/Collada/collada_image.h"
#include "../../../../../3D/Clan3D/Collada/collada_effect.h"
#include "../../../../../3D/Clan3D/Collada/collada_effect_texture.h"
#include "../../../../../3D/Clan3D/Collada/collada_material.h"
#include "../../../../../3D/Clan3D/Collada/collada_effect_color_texture.h"
#include "../../../../../3D/Clan3D/Collada/collada_effect_float_param.h"

struct CL_ObjectMesh
{
	std::vector<CL_Collada_Triangles> positions_surface_list;
	std::vector<CL_Vec3f> object_positions;
	std::vector<CL_Vec3f> object_normals;

	std::vector<CL_Collada_Triangles> texcoord_surface_list;
	std::vector<CL_Vec2f> object_texcoords;
};

class ModelCollada
{
//! Construction:
public:
	ModelCollada(CL_GraphicContext &gc, const CL_StringRef &filename);
	~ModelCollada();

//! Attributes:
public:
//! Operations:
public:
	void render(CL_GraphicContext &gc, const LightModel &light_model, const Position &position, const Orientation &orientation);

//! Implementation:
private:
	void load_textures(CL_GraphicContext &gc);
	void setup_light(CL_ProgramObject &program, const Material &material, const LightModel &light_model);
	void load(const CL_StringRef &filename);
	void scale_points(CL_Vec3f *points_ptr, int size, const CL_Vec3f &object_scale);
	void invert_uvmap(CL_Vec2f *points_ptr, int size);
	void translate_points(CL_Vec3f *points_ptr, int size, const CL_Vec3f &object_translation);
	void insert_object(CL_Collada_Geometry &geometry, CL_ObjectMesh &model, std::vector<CL_Collada_Image> &library_images, const CL_Angle &smooth_threshold);
	void set_texture(CL_GraphicContext &gc, CL_Collada_Triangles &surface);

	std::vector<CL_Collada_Image> library_images;

	std::map<CL_String, CL_Texture> textures;

	CL_ProgramObject program_object_texture;
	CL_ProgramObject program_object_no_texture;

	CL_ObjectMesh mesh;

};
