
#pragma once

class Program : public clan::Application
{
public:
	Program();
	bool update() override;
	
	bool exit = false;

	static const int particle_count = 30;
	struct Uniforms
	{
		clan::Vec3f resolution;
		float time;
		clan::Vec4f mouse;

		clan::Vec4f positions[particle_count];
		int xparticle_count;
	};

	void on_input_up(const clan::InputEvent &key);

	clan::DisplayWindow window;
	clan::SlotContainer sc;
	clan::GraphicContext gc;
	unsigned int start_time;
	Uniforms uniforms;
	clan::UniformVector<Uniforms> uniformVector;
	clan::ShaderEffect effect;


};
