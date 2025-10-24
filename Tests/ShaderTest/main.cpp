
#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/GL/opengl_wrap.h>

/////////////////////////////////////////////////////////////////////////////
// Application class

class Application : public CL_ClanApplication
{
public:
	int main(int argc, char** argv)
	{
		CL_ConsoleWindow debug_console("Debug console");
		debug_console.redirect_stdio();
	
		try
		{
			CL_SetupCore setupCore;
			CL_SetupDisplay setupDisplay;
			CL_SetupGL setupGL;

			CL_DisplayWindowDescription desc;
			desc.set_title("ClanLib Shader Test");
			desc.set_size(CL_Size(640, 480));
			desc.set_bpp(32);
			desc.set_fullscreen(false);
			CL_DisplayWindow window(desc);

			CL_ResourceManager resources("resources.xml");
			
			CL_OpenGLState state(window.get_gc());
			state.set_active();
			state.setup_2d();

			std::cout << "Renderer: " << (const char *) clGetString(CL_RENDERER) << std::endl;
			std::cout << "Vendor:   " << (const char *) clGetString(CL_VENDOR) << std::endl;
			std::cout << "Version:  " << (const char *) clGetString(CL_VERSION) << std::endl;

			std::string extensions = (const char *) clGetString(CL_EXTENSIONS);
			if (extensions.find("GL_ARB_shader_objects") == std::string::npos)
				std::cout << "No shader support by this card." << std::endl;

			CL_ShaderObject shader("white_shader", &resources);
			std::cout << "Shader status: " << (shader.get_compile_status() ? "true" : "false") << std::endl;
			std::cout << "Shader log: " << shader.get_info_log() << std::endl;
			std::cout << "Shader handle: " << shader.get_handle() << std::endl;

//			CL_ProgramObject program("program", &resources);
			CL_ProgramObject program;
			program.attach(shader);
			program.link();
			std::cout << "Program status: " << (program.get_link_status() ? "true" : "false") << std::endl;
			std::cout << "Program log: " << program.get_info_log() << std::endl;
			std::cout << "Program handle: " << program.get_handle() << std::endl;

			program.use();
			program.validate();
			std::cout << "Program validate status: " << (program.get_validate_status() ? "true" : "false") << std::endl;
			std::cout << "Program log: " << program.get_info_log() << std::endl;

			// print available uniform variables
			std::cout << "Uniform variables:" << std::endl;
			std::vector<CL_ProgramUniform> uniforms = program.get_uniforms();
			std::vector<CL_ProgramUniform>::iterator it;

			for(it = uniforms.begin(); it != uniforms.end(); ++it )
			{
				std::cout << CL_String::format( "name: %1, type: %2", (*it).name, get_uniform_type_str((*it).type)) << std::endl;
			}

			if( program.has_uniform("color", CL_FLOAT_VEC4) ) 
				program.set_uniform("color", 0.15f, 0.73f, 0.83f, 1.0f);

			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE)) 
			{
				window.get_gc()->clear(CL_Color::blue);
				state.set_active();
				clBegin(CL_QUADS);
				clColor3f(1.0, 0.0, 0.0);
				clVertex2i(100,100);
				clVertex2i(200,100);
				clVertex2i(200,200);
				clVertex2i(100,200);
				clEnd();

				window.flip();
				CL_System::keep_alive(20);
			}

			state.set_active();
			program.disable();
		}
		catch (CL_Error error)
		{
			std::cout << "Uncaught error: " << error.message.c_str() << std::endl;
		}

		debug_console.wait_for_key();
		return 0;
	}

	std::string get_uniform_type_str(int type)
	{
		static std::map<int, std::string> names;
		static bool first_time = true;

		if( first_time )
		{
			names[CL_FLOAT] = "CL_FLOAT";
			names[CL_FLOAT_VEC2] = "CL_FLOAT_VEC2";
			names[CL_FLOAT_VEC3] = "CL_FLOAT_VEC3";
			names[CL_FLOAT_VEC4] = "CL_FLOAT_VEC4";
			names[CL_INT] = "CL_INT";
			names[CL_INT_VEC2] = "CL_INT_VEC2";
			names[CL_INT_VEC3] = "CL_INT_VEC3";
			names[CL_INT_VEC4] = "CL_INT_VEC4";
			names[CL_BOOL] = "CL_BOOL";
			names[CL_BOOL_VEC2] = "CL_BOOL_VEC2";
			names[CL_BOOL_VEC3] = "CL_BOOL_VEC3";
			names[CL_BOOL_VEC4] = "CL_BOOL_VEC4";
			names[CL_FLOAT_MAT2] = "CL_FLOAT_MAT2";
			names[CL_FLOAT_MAT3] = "CL_FLOAT_MAT3";
			names[CL_FLOAT_MAT4] = "CL_FLOAT_MAT4";
			names[CL_SAMPLER_1D] = "CL_SAMPLER_1D";
			names[CL_SAMPLER_2D] = "CL_SAMPLER_2D";
			names[CL_SAMPLER_3D] = "CL_SAMPLER_3D";
			names[CL_SAMPLER_CUBE] = "CL_SAMPLER_CUBE";
			names[CL_SAMPLER_1D_SHADOW] = "CL_SAMPLER_1D_SHADOW";
			names[CL_SAMPLER_2D_SHADOW] = "CL_SAMPLER_2D_SHADOW";

			first_time = false;
		}

		return names[type];
	}
} app;
