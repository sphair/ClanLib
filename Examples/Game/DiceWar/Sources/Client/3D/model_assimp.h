
#pragma once

#include "orientation.h"
#include "position.h"
#include "light_model.h"

#include "assimp/assimp.h"
#include "assimp/aiPostProcess.h"
#include "assimp/aiScene.h"
#include "assimp/aiConfig.h"

class ModelAssimp
{
//! Construction:
public:
	ModelAssimp(CL_GraphicContext &gc, const CL_StringRef &filename);
	~ModelAssimp();

//! Attributes:
public:
//! Operations:
public:
	void render(CL_GraphicContext &gc, const LightModel &light_model, const Position &position, const Orientation &orientation);

//! Implementation:
private:
	void load_textures(CL_GraphicContext &gc);
	void setup_light(CL_ProgramObject &program, const Material &material, const LightModel &light_model);
	void load(CL_GraphicContext &gc, const CL_StringRef &filename);
	void set_texture(CL_GraphicContext &gc, const struct aiMesh* mesh);
	void count_vertices(const struct aiScene* sc, const struct aiNode* nd, int &total_positions_size, int &total_texcoords_size);
	void insert_vbo(const struct aiScene* sc, const struct aiNode* nd, int &current_position_offset, int &current_texcoord_offset);
	void render_vbo(CL_GraphicContext &gc, const struct aiScene* sc, const struct aiNode* nd, int &current_position_offset, int &current_texcoord_offset);


	std::map<CL_String, CL_Texture> textures;

	CL_ProgramObject program_object_texture;
	CL_ProgramObject program_object_no_texture;

	CL_VertexArrayBuffer vbo_positions;
	CL_VertexArrayBuffer vbo_normals;
	CL_VertexArrayBuffer vbo_texcoords;
	int total_positions_size;
	int total_texcoords_size;

	const struct aiScene* scene;

};
