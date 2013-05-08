/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Magnus Norddahl
**    Mark Page
*/
#include "precomp.h"
#include "chess.h"
#include "model.h"
#include "skybox.h"
#include "3D/camera.h"

int Chess::run(const std::vector<std::string> &args)
{
	DisplayWindowDescription windowdesc;
	windowdesc.set_title("Chess");
	windowdesc.set_depth_size(16);
	windowdesc.set_size(Size(1024, 768), true);
	windowdesc.set_allow_resize(true);
	windowdesc.set_multisampling(4);
	DisplayWindow window(windowdesc);
	GraphicContext gc = window.get_gc();

	Slot slot_quit = window.sig_window_close().connect(this, &Chess::on_window_close);

	Model building(gc, "Resources/Sponza/sponza.3ds", Model::read_from_file);
	Model chessboard(gc, "Resources/ChessSet/chessboard.3ds", Model::use_wood);
	Model bishop(gc, "Resources/ChessSet/bishop.3ds", Model::use_silver);
	Model king(gc, "Resources/ChessSet/king.3ds", Model::use_silver);
	Model knight(gc, "Resources/ChessSet/knight.3ds", Model::use_silver);
	Model pawn(gc, "Resources/ChessSet/pawn.3ds", Model::use_silver);
	Model queen(gc, "Resources/ChessSet/queen.3ds", Model::use_silver);
	Model rook(gc, "Resources/ChessSet/rook.3ds", Model::use_silver);

	building.set_scale(16.0f);
	queen.set_scale(0.22f);
	king.set_scale(0.065f);
	rook.set_scale(0.065f);
	bishop.set_scale(0.065f);
	pawn.set_scale(0.065f);
	knight.set_scale(0.065f);

	enum ModelIndex
	{
		king_index = 0,
		queen_index = 1,
		rook_index = 2,
		bishop_index = 3,
		knight_index = 4,
		pawn_index = 5
	};

	ChessPieceModel models[] =
	{
		&king, -24.2f, -27.0f, 5.0f,  // p.set_position(-24.2f+x*5, -27+y*5, 5.0f); // king
		&queen, -12.2f, -29.6f, 2.4f, // p.set_position(-14+x*5, -30+y*5, 1.7f); // queen
		&rook, -11.0f, -27.0f, 5.0f,  // p.set_position(-11+x*5, -27+y*5, 5.0f); // rook
		&bishop, -15.5f, -27.0f, 5.0f,// p.set_position(-15.5f+x*5, -27+y*5, 5.0f); // bishop
		&knight, -6.5f, -27.0f, 5.0f, // p.set_position(-6.5f+x*5, -27+y*5, 5.0f); // knight
		&pawn, -2.0f, -27.0f, 5.0f,   // p.set_position(-2+x*5, -27+y*5, 5.0f); // pawn
		0, 0.0f, 0.0f, 0.0f
	};

	Vec3f eye(0.0f, -22.0f, 15.0f);
	Vec3f center(0.0f, -10.0f, -2.0f);
	Vec3f up(0.0f, 1.0f, 0.0f);
	Position our_position(eye.x, eye.y, eye.z);
	Orientation our_orientation = Orientation::look_at(eye, center, up);
	our_orientation.rotate(180.0f, Vec3f(0.0f, 0.0f, 1.0f));
	our_orientation.rotate(30.0f, Vec3f(1.0f, 0.0f, 0.0f));

	Skybox skybox(gc);
	//Font font(gc, "Tahoma", -12);

	quit = false;
	while (!window.get_ic().get_keyboard().get_keycode(keycode_escape))
	{
		if (quit)
			break;

		float dx = 0.0f, dy = 0.0f, dz = 0.0f;
		float rx = 0.0f, ry = 0.0f, rz = 0.0f;
		handle_input(window, dz, dx, dy, ry, rx, rz);

		our_orientation.rotate(rx, Vec3f(1.0f, 0.0f, 0.0f));
		our_orientation.rotate(ry, Vec3f(0.0f, 1.0f, 0.0f));
		our_orientation.rotate(rz, Vec3f(0.0f, 0.0f, 1.0f));
		our_position.translate(our_orientation, dx, dy, dz);

		Camera camera(55.0f, gc.get_width()/(float)gc.get_height());
		camera.set_position(our_position);
		camera.set_orientation(our_orientation);

		gc.clear(Colorf(0.0f, 0.0f, 0.0f));
		skybox.render(gc, camera);
		camera.setup_gc(gc, 1.0f, 1000.0f);

		gc.enable_depth_test(true);
		gc.enable_depth_write(true);
		gc.clear_depth(1.0);

		LightModel lightmodel;
		LightSource light0;
		light0.constant_attenuation = 0.004f;
		light0.position = camera.get_modelview()*Vec4f(-60.0f, -125.0f, 350.0f, 1.0f);
		lightmodel.scene_ambient = Vec4f(0.4f, 0.4f, 0.4f, 1.0f);
		lightmodel.light_sources.push_back(light0);

		building.render(gc, lightmodel, Position(0.0f, 0.0f, -4.0f), Orientation());
		chessboard.render(gc, lightmodel, Position(), Orientation());

		for (int x=0; x<8; x++)
			render_model(gc, lightmodel, x, 6, models, pawn_index);

		render_model(gc, lightmodel, 0, 7, models, rook_index);
		render_model(gc, lightmodel, 1, 7, models, knight_index);
		render_model(gc, lightmodel, 2, 7, models, bishop_index);
		render_model(gc, lightmodel, 3, 7, models, king_index);
		render_model(gc, lightmodel, 4, 7, models, queen_index);
		render_model(gc, lightmodel, 5, 7, models, bishop_index);
		render_model(gc, lightmodel, 6, 7, models, knight_index);
		render_model(gc, lightmodel, 7, 7, models, rook_index);

		gc.reset_buffer_control();

		gc.set_projection(Mat4f::identity());
		gc.set_modelview(Mat4f::identity());
		gc.set_map_mode(map_2d_upper_left);
		//font.draw_text(gc, 10, 24, string_format("%1,%2,%3", our_position.get_x(), our_position.get_y(), our_position.get_z()));

		window.flip();
		KeepAlive::process();
	}
	
	return 0;
}

void Chess::render_model(GraphicContext &gc, LightModel &lightmodel, int x, int y, ChessPieceModel *models, int model_index)
{
	Position p;
	p.set_position(models[model_index].offset_x+x*5, models[model_index].offset_y+y*5, models[model_index].offset_z);
	models[model_index].model->render(gc, lightmodel, p, Orientation());
}

void Chess::handle_input(DisplayWindow &window, float &dz, float &dx, float &dy, float &ry, float &rx, float &rz)
{
	if (window.get_ic().get_keyboard().get_keycode(KEY_A))
		dz += 0.2f;
	else if (window.get_ic().get_keyboard().get_keycode(KEY_Z))
		dz -= 0.2f;

	if (window.get_ic().get_keyboard().get_keycode(KEY_MENU))
	{
		if (window.get_ic().get_keyboard().get_keycode(KEY_LEFT))
			dx += 0.2f;
		if (window.get_ic().get_keyboard().get_keycode(KEY_RIGHT))
			dx -= 0.2f;
		if (window.get_ic().get_keyboard().get_keycode(KEY_UP))
			dy += 0.2f;
		if (window.get_ic().get_keyboard().get_keycode(KEY_DOWN))
			dy -= 0.2f;
	}
	else
	{
		if (window.get_ic().get_keyboard().get_keycode(KEY_LEFT))
			ry -= 0.5f;
		if (window.get_ic().get_keyboard().get_keycode(KEY_RIGHT))
			ry += 0.5f;
		if (window.get_ic().get_keyboard().get_keycode(KEY_UP))
			rx -= 0.5f;
		if (window.get_ic().get_keyboard().get_keycode(KEY_DOWN))
			rx += 0.5f;
		if (window.get_ic().get_keyboard().get_keycode(KEY_Q))
			rz += 0.5f;
		if (window.get_ic().get_keyboard().get_keycode(KEY_E))
			rz -= 0.5f;
	}
}

void Chess::on_window_close()
{
	quit = true;
}
