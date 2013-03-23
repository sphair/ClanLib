
#pragma once

#include "GLSL/AST/glsl_ast.h"
#include "GLSL/FixedFunction/glsl_program.h"

class GlslFrontend_Impl
{
public:
	GlslFrontend_Impl() : draw_triangles_func(0) { }

	std::unique_ptr<GlslAstTranslationUnit> ast_vertex;
	std::unique_ptr<GlslAstTranslationUnit> ast_tesselation_control;
	std::unique_ptr<GlslAstTranslationUnit> ast_tesselation_evaluation;
	std::unique_ptr<GlslAstTranslationUnit> ast_geometry;
	std::unique_ptr<GlslAstTranslationUnit> ast_fragment;
	std::unique_ptr<GlslProgram> program;

	typedef void (DrawTrianglesFunc)(
		int input_width, int input_height, const unsigned char *input_data,
		int output_width, int output_height, unsigned char *output_data,
		int viewport_x, int viewport_y, int viewport_width, int viewport_height,
		float *uniforms,
		int first_vertex, int num_vertices, float **vertex_attributes,
		int core, int num_cores);
	DrawTrianglesFunc *draw_triangles_func;
};
