/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
*/

#include "precomp.h"
#include "app.h"

int App::start(const std::vector<CL_String> &args)
{
	CL_OpenGLWindowDescription description;
	description.set_title("GLSL Test");
	description.set_size(CL_Size(1024, 768), true);

	CL_DisplayWindow window(description);
	CL_InputDevice keyboard = window.get_ic().get_keyboard();
	CL_GraphicContext gc = window.get_gc();

	CL_Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Load and link shaders
	CL_ProgramObject shader = CL_ProgramObject::load(gc, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	if (!shader.link())
		throw CL_Exception("Unable to link shader program: Error:" + shader.get_info_log());

	quit = false;

	while (!keyboard.get_keycode(CL_KEY_ESCAPE) && !quit )
	{
		gc.clear();
		gc.set_program_object(shader, cl_program_matrix_modelview_projection);
		//shader.set_uniform1i("SourceTexture", 0);

		int xpos = 0;
		int size = 16;
		int gap = size + 2;

		//-------- Test 1
		CL_Mat2f matrix2_a = CL_Mat2f(3, 1, 2, 4);
		CL_Mat2f matrix2_b = CL_Mat2f(-3, 7, 2, 5);
		CL_Mat2f matrix2_result = CL_Mat2f::multiply(matrix2_a, matrix2_b);

		CL_Mat2f matrix2_a_temp = matrix2_a;
		if (CL_Mat2i(matrix2_result) != CL_Mat2i(matrix2_a * matrix2_b))
			throw CL_Exception("Failure");

		shader.set_uniform1i("test_id", 1);
		shader.set_uniform_matrix("matrix2_a", matrix2_a);
		shader.set_uniform_matrix("matrix2_b", matrix2_b);
		shader.set_uniform_matrix("matrix2_result", matrix2_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;

		//-------- Test 2
		CL_Mat3f matrix3_a = CL_Mat3f(3, 1, 2, 4, 5 ,6, 4, 2, 1);
		CL_Mat3f matrix3_b = CL_Mat3f(4, 7, 2, 5, 3, 5, 2, 9, 3);
		CL_Mat3f matrix3_result = CL_Mat3f::multiply(matrix3_a, matrix3_b);

		CL_Mat3f matrix3_a_temp = matrix3_a;
		if (CL_Mat3i(matrix3_result) != CL_Mat3i(matrix3_a * matrix3_b))
			throw CL_Exception("Failure");

		shader.set_uniform1i("test_id", 2);
		shader.set_uniform_matrix("matrix3_a", matrix3_a);
		shader.set_uniform_matrix("matrix3_b", matrix3_b);
		shader.set_uniform_matrix("matrix3_result", matrix3_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;

		//-------- Test 3
		static float test_a_values[] = {3, 1, 2, 4, 5 ,6, 4, 2, 1, 4, 6, 7, 6, 3, 7, 2};
		static float test_b_values[] = {4, 7, 2, 5, 3, 5, 2, 9, 3, 3, 6, 9, 2, 4, 6, 2};

		CL_Mat4f matrix4_a = CL_Mat4f(test_a_values);
		CL_Mat4f matrix4_b = CL_Mat4f(test_b_values);
		CL_Mat4f matrix4_result = CL_Mat4f::multiply(matrix4_a, matrix4_b);

		CL_Mat4f matrix4_a_temp = matrix4_a;
		if (CL_Mat4i(matrix4_result) != CL_Mat4i(matrix4_a * matrix4_b))
			throw CL_Exception("Failure");

		shader.set_uniform1i("test_id", 3);
		shader.set_uniform_matrix("matrix4_a", matrix4_a);
		shader.set_uniform_matrix("matrix4_b", matrix4_b);
		shader.set_uniform_matrix("matrix4_result", matrix4_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;

		//-------- Test 4
		matrix2_result = CL_Mat2f::subtract(matrix2_a, matrix2_b);

		matrix2_a_temp = matrix2_a;
		if (CL_Mat2i(matrix2_result) != CL_Mat2i(matrix2_a - matrix2_b))
			throw CL_Exception("Failure");

		shader.set_uniform1i("test_id", 4);
		shader.set_uniform_matrix("matrix2_a", matrix2_a);
		shader.set_uniform_matrix("matrix2_b", matrix2_b);
		shader.set_uniform_matrix("matrix2_result", matrix2_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;

		//-------- Test 5
		matrix3_result = CL_Mat3f::subtract(matrix3_a, matrix3_b);

		matrix3_a_temp = matrix3_a;
		if (CL_Mat3i(matrix3_result) != CL_Mat3i(matrix3_a - matrix3_b))
			throw CL_Exception("Failure");

		shader.set_uniform1i("test_id", 5);
		shader.set_uniform_matrix("matrix3_a", matrix3_a);
		shader.set_uniform_matrix("matrix3_b", matrix3_b);
		shader.set_uniform_matrix("matrix3_result", matrix3_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;

		//-------- Test 6

		matrix4_result = CL_Mat4f::subtract(matrix4_a, matrix4_b);

		matrix4_a_temp = matrix4_a;
		if (CL_Mat4i(matrix4_result) != CL_Mat4i(matrix4_a - matrix4_b))
			throw CL_Exception("Failure");

		shader.set_uniform1i("test_id", 6);
		shader.set_uniform_matrix("matrix4_a", matrix4_a);
		shader.set_uniform_matrix("matrix4_b", matrix4_b);
		shader.set_uniform_matrix("matrix4_result", matrix4_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;

		//-------- Test 7
		CL_Vec2f vector2_a(2,3);
		CL_Vec2f vector2_result = vector2_a * matrix2_a;

		shader.set_uniform1i("test_id", 7);
		shader.set_uniform_matrix("matrix2_a", matrix2_a);
		shader.set_uniform2f("vector2_a", vector2_a);
		shader.set_uniform2f("vector2_result", vector2_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;

		//-------- Test 8
		vector2_result = matrix2_a * vector2_a;

		shader.set_uniform1i("test_id", 8);
		shader.set_uniform_matrix("matrix2_a", matrix2_a);
		shader.set_uniform2f("vector2_a", vector2_a);
		shader.set_uniform2f("vector2_result", vector2_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;


		//-------- Test 9
		CL_Vec3f vector3_a(3,5,6);
		CL_Vec3f vector3_result = vector3_a * matrix3_a;

		shader.set_uniform1i("test_id", 9);
		shader.set_uniform_matrix("matrix3_a", matrix3_a);
		shader.set_uniform3f("vector3_a", vector3_a);
		shader.set_uniform3f("vector3_result", vector3_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;

		//-------- Test 10
		vector3_result = matrix3_a * vector3_a;

		shader.set_uniform1i("test_id", 10);
		shader.set_uniform_matrix("matrix3_a", matrix3_a);
		shader.set_uniform3f("vector3_a", vector3_a);
		shader.set_uniform3f("vector3_result", vector3_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;
		//-------- Test 11
		CL_Vec4f vector4_a(4,3,5,6);
		CL_Vec4f vector4_result = vector4_a * matrix4_a;

		shader.set_uniform1i("test_id", 11);
		shader.set_uniform_matrix("matrix4_a", matrix4_a);
		shader.set_uniform4f("vector4_a", vector4_a);
		shader.set_uniform4f("vector4_result", vector4_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;

		//-------- Test 12
		vector4_result = matrix4_a * vector4_a;

		shader.set_uniform1i("test_id", 12);
		shader.set_uniform_matrix("matrix4_a", matrix4_a);
		shader.set_uniform4f("vector4_a", vector4_a);
		shader.set_uniform4f("vector4_result", vector4_result);
		draw(gc, CL_Rect(xpos, 0, CL_Size(size, size)));
		xpos+=gap;


		gc.reset_program_object();
		window.flip();

		CL_System::sleep(10);

		CL_KeepAlive::process();
	}

	return 0;
}

void App::draw(CL_GraphicContext &gc, const CL_Rectf &rect)
{
	CL_Vec2f positions[6] =
	{
		CL_Vec2f(rect.left, rect.top),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.bottom)
	};

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	gc.draw_primitives(cl_triangles, 6, prim_array);
}

void App::window_close()
{
	quit = true;
}
