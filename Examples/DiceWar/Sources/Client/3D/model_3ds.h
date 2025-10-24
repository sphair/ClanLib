
#pragma once

#include "lib3ds_help.h"
#include "orientation.h"
#include "position.h"
#include "light_model.h"

class Model3DS
{
//! Construction:
public:
	Model3DS(CL_GraphicContext &gc, const CL_StringRef &filename);
	~Model3DS();

//! Attributes:
public:
//! Operations:
public:
	void render(CL_GraphicContext &gc, const LightModel &light_model, const Position &position, const Orientation &orientation);

//! Implementation:
private:
	void load_textures(CL_GraphicContext &gc);
	void generate_vertex_buffer(CL_GraphicContext &gc);
	void render_node(CL_GraphicContext &gc, const LightModel &light_model, Lib3dsFile *file, Lib3dsNode *node);
	void setup_light(CL_ProgramObject &program, const Material &material, const LightModel &light_model);

	CL_Lib3dsFile model_file;

	struct MeshVertex
	{
		CL_Vec3f position;
		CL_Vec3f normal;
		CL_Vec2f texcoord0;
	};

	struct MeshElementsRange
	{
		int start, length;
		Material material;
		CL_Texture texture;
	};

	std::map<CL_String8, CL_Texture> textures;
	std::map<Lib3dsMaterial *, Material> materials;
	std::map<Lib3dsMesh *, std::vector<MeshElementsRange> > mesh_ranges;
	CL_VertexArrayBuffer vertices;

	CL_ProgramObject program_object_texture;
	CL_ProgramObject program_object_no_texture;
};
