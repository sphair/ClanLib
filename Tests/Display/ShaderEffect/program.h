
#pragma once

using namespace clan;

class Program : public clan::Application
{
public:
	Program();
	bool update() override;

	clan::SlotContainer sc;
	clan::DisplayWindow window;

	struct Uniforms
	{
		Vec3f resolution;
		float time;
		Vec4f mouse;
	};

	void exit_func();
	bool exit = false;
	Uniforms uniforms;
	ShaderEffect effect;
	UniformVector<Uniforms> uniformVector;

};
