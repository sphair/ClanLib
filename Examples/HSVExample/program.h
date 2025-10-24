
#pragma once

class Program
{
public:
	static int main(const std::vector<CL_String> &args);

	Program();
	int run();

private:
	void on_close();
	CL_ProgramObject create_shader_program(CL_GraphicContext &gc);
	CL_Texture create_texture(CL_GraphicContext &gc);
	void render_texture(CL_GraphicContext &gc, CL_ProgramObject &program, CL_Texture &texture, float hue_offset);

	bool quit;
};
