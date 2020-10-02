/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
	float compute_gaussian(float n, float theta);
	void render_gaussian_blur(clan::Canvas &canvas, float blur_amount, clan::Texture2D &source_texture, clan::ProgramObject &program_object, float dx, float dy);
	void on_input_up(const clan::InputEvent &key);
	void draw_texture(clan::GraphicContext &gc, const clan::Rectf &rect, const clan::Rectf &texture_unit1_coords);

	static const int sampleCount = 15;

	struct Sample
	{
		float offset_x;
		float offset_y;
		float weight;
		float padding;
	};


	struct ProgramUniforms
	{
		clan::Mat4f cl_ModelViewProjectionMatrix;
		Sample sample[sampleCount];		// Each array element must be aligned to a vec4 (4 floats) in GLSL
	};
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::SlotContainer sc;
	clan::ProgramObject shader;
	clan::Font font;
	clan::Image background;
	clan::Image ball;
	clan::Canvas canvas_offscreen;
	clan::Canvas canvas_offscreen2;
	clan::FrameBuffer framebuffer_offscreen;
	clan::FrameBuffer framebuffer_offscreen2;
	clan::Texture2D texture_offscreen;
	clan::Texture2D texture_offscreen2;

	clan::VertexArrayVector<clan::Vec2f> gpu_positions;
	clan::VertexArrayVector<clan::Vec2f> gpu_tex1_coords;
	clan::UniformVector<ProgramUniforms> gpu_uniforms;
	clan::PrimitivesArray gpu_primitives_array;

	ProgramUniforms uniforms;

	uint64_t startTime;

	float amount = 0.0f;
	float timer = 0.0f;
	float scale = 1.0f;

	float blur = 1.0f;
	bool quit = false;
};

