
#pragma once

class Model;
class LightModel;

class Program
{
public:
	static int main(const std::vector<CL_String> &args);
	int run(const std::vector<CL_String> &args);

	struct ChessPieceModel
	{
		Model *model;
		float offset_x, offset_y, offset_z;
	};
	void render_model(CL_GraphicContext &gc, LightModel &lightmodel, int x, int y, ChessPieceModel *models, int model_index);
	void handle_input(CL_DisplayWindow &window, float &dz, float &dx, float &dy, float &ry, float &rx, float &rz);

private:
	void on_window_close();
	bool quit;
};
