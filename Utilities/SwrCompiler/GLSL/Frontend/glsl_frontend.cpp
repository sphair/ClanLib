
#include "precomp.h"
#include "glsl_frontend.h"
#include "glsl_frontend_impl.h"
#include "GLSL/Parse/glsl_parser.h"
#include "GLSL/CodeGen/glsl_codegen.h"
#include "GLSL/FixedFunction/glsl_fixed_function.h"
#include "GLSL/FixedFunction/glsl_program.h"

using namespace clan;

GlslFrontend::GlslFrontend()
: impl(new GlslFrontend_Impl())
{
}

void GlslFrontend::compile_vertex_shader(const std::string &source_code)
{
	GlslParser parser(source_code);
	impl->ast_vertex = parser.parse();
}

void GlslFrontend::compile_tesselation_control_shader(const std::string &source_code)
{
	throw Exception("Not implemented");
}

void GlslFrontend::compile_tesselation_evaluation_shader(const std::string &source_code)
{
	throw Exception("Not implemented");
}

void GlslFrontend::compile_geometry_shader(const std::string &source_code)
{
	throw Exception("Not implemented");
}

void GlslFrontend::compile_fragment_shader(const std::string &source_code)
{
	GlslParser parser(source_code);
	impl->ast_fragment = parser.parse();
}

void GlslFrontend::link()
{
	impl->draw_triangles_func = 0;
	impl->program.reset(new GlslProgram());

	GlslCodeGen vertex_codegen(*impl->program.get(), GlslCodeGen::vertex_shader);
	vertex_codegen.codegen(impl->ast_vertex.get());

	GlslCodeGen fragment_codegen(*impl->program.get(), GlslCodeGen::fragment_shader);
	fragment_codegen.codegen(impl->ast_fragment.get());

	GlslFixedFunction fixed_function(*impl->program.get(), vertex_codegen, fragment_codegen);
	fixed_function.codegen();

	impl->draw_triangles_func = reinterpret_cast<GlslFrontend_Impl::DrawTrianglesFunc*>(impl->program->compile("draw_triangles"));
}

void GlslFrontend::draw_triangles(
	int input_width, int input_height, const unsigned char *input_data,
	int output_width, int output_height, unsigned char *output_data,
	int viewport_x, int viewport_y, int viewport_width, int viewport_height,
	float *uniforms,
	int first_vertex, int num_vertices, float **vertex_attributes,
	int core, int num_cores)
{
	if (!impl)
		throw Exception("Impl is null!");
	if (impl->draw_triangles_func == 0)
		throw Exception("Program not linked!");

	impl->draw_triangles_func(input_width, input_height, input_data, output_width, output_height, output_data, viewport_x, viewport_y, viewport_width, viewport_height, uniforms, first_vertex, num_vertices, vertex_attributes, core, num_cores);
}
