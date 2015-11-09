
#pragma once

using namespace clan;

class Program : public clan::Application
{
public:
	Program();
	bool update() override;

	void exit_func();
	bool exit = false;

	static const int particle_count = 30;

	struct Uniforms
	{
		Vec3f resolution;
		float time;
		Vec4f mouse;

		Vec4f positions[particle_count];
		int particle_count;
	};

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	ShaderEffect effect;
	UniformVector<Uniforms> uniformVector;
	Uniforms uniforms;

}




