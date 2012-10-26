
#pragma once

#include "ui_screen.h"

class GameScreen : public UIScreen
{
public:
	GameScreen(CL_GUIManager *gui);
	~GameScreen();

private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &rect);
	void load_skybox(CL_GraphicContext &gc);
	void render_skybox(CL_GraphicContext &gc);

	CL_ProgramObject skybox_program;
	CL_Texture skybox_texture;
	static CL_Vec3f skybox_positions[6*6];
};
