
#pragma once

#include "orientation.h"
#include "position.h"
#include "light_model.h"

#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/config.h"

class ModelAssimp
{
//! Construction:
public:
	ModelAssimp(GraphicContext &gc, const std::string &filename);
	~ModelAssimp();

//! Attributes:
public:
//! Operations:
public:
	void render(Canvas &canvas, const LightModel &light_model, const Position &position, const Orientation &orientation);

//! Implementation:
private:
	void load_textures(GraphicContext &gc);
	void setup_light(ProgramObject &program, const Material &material, const LightModel &light_model);
	void load(GraphicContext &gc, const std::string &filename);
	void set_texture(GraphicContext &gc, const struct aiMesh* mesh);
	void count_vertices(const struct aiScene* sc, const struct aiNode* nd, int &total_positions_size, int &total_texcoords_size);
	void insert_vbo(const struct aiScene* sc, const struct aiNode* nd, int &current_position_offset, int &current_texcoord_offset);
	void render_vbo(GraphicContext &gc, const struct aiScene* sc, const struct aiNode* nd, int &current_position_offset, int &current_texcoord_offset);


	std::map<std::string, Texture> textures;

	ProgramObject program_object_texture;
	ProgramObject program_object_no_texture;

	VertexArrayBuffer vbo_positions;
	VertexArrayBuffer vbo_normals;
	VertexArrayBuffer vbo_texcoords;
	int total_positions_size;
	int total_texcoords_size;

	const struct aiScene* scene;

};
