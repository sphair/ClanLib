/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#pragma once

class App : public clan::Application
{
public:
	App();
	bool update() override;

	void window_close();

private:
	void render_shockwave(clan::Canvas &canvas, clan::Texture2D &source_texture, clan::ProgramObject &program_object);
	void on_input_up(const clan::InputEvent &key);
	void draw_texture(clan::GraphicContext &gc, const clan::Rectf &rect, const clan::Rectf &texture_unit1_coords);

	struct ProgramUniforms
	{
		clan::Mat4f cl_ModelViewProjectionMatrix;
		clan::Vec3f shockParams; 
		float time;
		clan::Vec2f center;
		float glow;
	};

	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::SlotContainer sc;
	clan::Font font;
	clan::ProgramObject shader;
	clan::Image background;
	clan::Image ball;
	clan::Canvas canvas_offscreen;
	clan::Texture2D texture_offscreen;

	clan::VertexArrayVector<clan::Vec2f> gpu_positions;
	clan::VertexArrayVector<clan::Vec2f> gpu_tex1_coords;
	clan::UniformVector<ProgramUniforms> gpu_uniforms;
	clan::PrimitivesArray gpu_primitives_array;

	ProgramUniforms uniforms;

	bool quit = false;
	float timer = 0.0f;
	float shockwave_start_time = 0.0f;
	float shockwave_rate = 1.0f;

	float amount = 0.0f;
	float scale = 1.0f;
	uint64_t startTime;


};

