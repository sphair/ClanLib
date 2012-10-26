
#include "precomp.h"
#include "program.h"
#include "framerate_counter.h"
#include <ClanLib/application.h>

CL_ClanApplication clanapp(&Program::main);

bool Program::exit = false;

void Program::exit_func()
{
	exit = true;
}

int Program::main(const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupD3D setup_gl;
	CL_SetupCompute setup_compute;

	CL_ConsoleWindow console("Debug Output");
	CL_ConsoleLogger logger;
	logger.enable();

	try
	{
		CL_DisplayWindow window("Hello Direct3D", 800, 600, false, true);

		//	CL_Slot slot = window.sig_window_close().connect_functor([&exit] { exit = true; });
		CL_Slot slot = window.sig_window_close().connect(&Program::exit_func);

		test1(window);
	}
	catch (CL_Exception &e)
	{
		CL_Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
	}

	console.wait_for_key();
	return 0;
}

void Program::test1(CL_DisplayWindow &window)
{
	CL_DisplayWindowDescription desc;
	desc.set_size(CL_Size(640, 480), true);
	desc.set_visible(true);
	CL_DisplayWindow window2(desc);
	CL_GraphicContext gc2 = window2.get_gc();

	CL_GraphicContext gc = window.get_gc();
	CL_InputContext ic = window.get_ic();

	CL_PixelBuffer pb_cpu = CL_PixelBuffer("../../../Examples/Display/Basic2D/Resources/logo.png").to_format(cl_rgba8);
	CL_PixelBuffer pb_gpu(gc, pb_cpu);
	CL_Texture2D texture(gc, pb_gpu.get_width(), pb_gpu.get_height());
	texture.set_image(pb_gpu);

	CL_Image image2(gc2, "../../../Examples/Display/Basic2D/Resources/logo.png");

	FramerateCounter fps_counter;

	while (!exit)
	{
		//gc.clear(CL_Colorf::orangered);
		CL_Draw::gradient_fill(gc, 0.0f, 0.0f, (float)gc.get_width(), (float)gc.get_height(), CL_Gradient(CL_Colorf::coral, CL_Colorf::cornsilk));

		int margin = 5;
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 2; x++)
			{
				image2.draw(gc, x * (image2.get_width() + margin), y * (image2.get_height() + margin));
			}
		}

		gc2.clear();
		image2.draw(gc2, CL_Rectf(gc2.get_size()));

		window.flip(0);
		window2.flip(0);
		fps_counter.frame_rendered();
		CL_KeepAlive::process();
	}

	CL_Console::write_line("FPS: %1", fps_counter.get_fps());
}

void Program::test2(CL_DisplayWindow &window)
{
	CL_GraphicContext gc = window.get_gc();

	CL_ShaderObject vertex_shader1(gc, cl_shadertype_vertex, CL_File::read_text("vertex1.hlsl"));
	CL_ShaderObject pixel_shader1(gc, cl_shadertype_fragment, CL_File::read_text("pixel1.hlsl"));
	vertex_shader1.compile();
	pixel_shader1.compile();
	CL_ProgramObject program1(gc);
	program1.attach(vertex_shader1);
	program1.attach(pixel_shader1);
	program1.bind_attribute_location(0, "PositionInProjection");
	program1.link();

	CL_Console::write_line("Vertex compile log(1):");
	CL_Console::write_line(vertex_shader1.get_info_log());

	CL_Console::write_line("Pixel compile log(1):");
	CL_Console::write_line(pixel_shader1.get_info_log());

	CL_ShaderObject vertex_shader2(gc, cl_shadertype_vertex, CL_File::read_text("vertex2.hlsl"));
	CL_ShaderObject pixel_shader2(gc, cl_shadertype_fragment, CL_File::read_text("pixel2.hlsl"));
	vertex_shader2.compile();
	pixel_shader2.compile();
	CL_ProgramObject program2(gc);
	program2.attach(vertex_shader2);
	program2.attach(pixel_shader2);
	program2.bind_attribute_location(0, "PositionInProjection");
	program2.link();
	program2.set_uniform1i("SourceTexture", 4);

	CL_Console::write_line("Vertex compile log(2):");
	CL_Console::write_line(vertex_shader2.get_info_log());

	CL_Console::write_line("Pixel compile log(2):");
	CL_Console::write_line(pixel_shader2.get_info_log());

	CL_Texture2D offscreen(gc, gc.get_width(), gc.get_height(), cl_rgba16f);
	CL_FrameBuffer fb(gc);
	fb.attach_color(0, offscreen);

	CL_Vec4f positions[6] =
	{
		CL_Vec4f(-1.0f, -1.0f, 1.0f, 1.0f),
		CL_Vec4f( 1.0f, -1.0f, 1.0f, 1.0f),
		CL_Vec4f(-1.0f,  1.0f, 1.0f, 1.0f),
		CL_Vec4f( 1.0f, -1.0f, 1.0f, 1.0f),
		CL_Vec4f(-1.0f,  1.0f, 1.0f, 1.0f),
		CL_Vec4f( 1.0f,  1.0f, 1.0f, 1.0f)
	};
	CL_VertexArrayVector<CL_Vec4f> rect_positions(gc, positions, 6);
	CL_PrimitivesArray rect_primarray(gc);
	rect_primarray.set_attributes(0, rect_positions);

	CL_BlendStateDescription blend_desc;
	blend_desc.enable_blending(false);
	CL_BlendState no_blending(gc, blend_desc);

	FramerateCounter fps_counter;
	while (!exit)
	{
		gc.clear(CL_Colorf::gray);

		gc.set_frame_buffer(fb);
		gc.set_viewport(gc.get_size());
		gc.set_blend_state(no_blending);
		gc.set_program_object(program1);
		gc.draw_primitives(cl_triangles, 6, rect_primarray);
		gc.reset_program_object();
		gc.reset_frame_buffer();

		gc.set_viewport(gc.get_size());
		gc.set_blend_state(no_blending);
		gc.set_texture(4, offscreen);
		gc.set_program_object(program2);
		gc.draw_primitives(cl_triangles, 6, rect_primarray);
		gc.reset_program_object();
		gc.reset_texture(4);

		window.flip(0);
		fps_counter.frame_rendered();
		CL_KeepAlive::process();
	}

	CL_Console::write_line("FPS: %1", fps_counter.get_fps());
}

void Program::test3(CL_DisplayWindow &window)
{
	CL_GraphicContext gc = window.get_gc();
	CL_InputContext ic = window.get_ic();

	CL_ComputeContext cc(gc);
	CL_ComputeCommandQueue queue(cc);

	CL_ComputeProgram program(cc, CL_File::read_text("compute.opencl"));
	program.build();

	CL_ComputeBuffer buffer(cc, 128 * 128 * sizeof(int));

	CL_ComputeKernel kernel(program, "main");
	kernel.set_arg_buffer(0, buffer);
	kernel.set_arg_int(1, 128);

	CL_ComputeEvent run_event = queue.run_2d(kernel, 128, 128);

	CL_ComputeMemoryMap mapping = queue.map_buffer(buffer, cl_access_read_only, 0, 128 * sizeof(int), run_event);
	int *data = mapping.data<int>();
	CL_Console::write_line("Meaning of life: %1", data[42]);
	mapping.unmap();

	queue.finish();
}
