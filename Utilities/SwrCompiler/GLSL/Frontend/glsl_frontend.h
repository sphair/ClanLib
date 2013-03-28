
#pragma once

class GlslFrontend_Impl;

class GlslFrontend
{
public:
	GlslFrontend();

	void compile_vertex_shader(const std::string &source_code);
	void compile_tesselation_control_shader(const std::string &source_code);
	void compile_tesselation_evaluation_shader(const std::string &source_code);
	void compile_geometry_shader(const std::string &source_code);
	void compile_fragment_shader(const std::string &source_code);
	void link();

	void draw_triangles(
		int input_width, int input_height, const unsigned char *input_data,
		int output_width, int output_height, unsigned char *output_data,
		int viewport_x, int viewport_y, int viewport_width, int viewport_height,
		float *uniforms,
		int first_vertex, int num_vertices, float **vertex_attributes,
		int core, int num_cores);

private:
	std::shared_ptr<GlslFrontend_Impl> impl;
};
