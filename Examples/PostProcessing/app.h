
#pragma once

class App
{
public:
	int start(const std::vector<CL_String> &args);
	void draw_texture(CL_GraphicContext &gc, const CL_Rectf &rect, const CL_Colorf &color = CL_Colorf::white, const CL_Rectf &texture_unit1_coords = CL_Rectf(0.0f,0.0f,1.0f,1.0f));
	void window_close();

private:
	bool quit;
};

