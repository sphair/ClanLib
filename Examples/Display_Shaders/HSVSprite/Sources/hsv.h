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
**    Magnus Norddahl
*/

#pragma once
class HSVSpriteBatch;
class HSVSprite;

class HSV : public clan::Application
{
public:
	HSV();
	bool update() override;
	int run();

private:
	void on_close();
	void on_input_up(const InputEvent &key);

	DisplayWindow window;
	Canvas canvas;
	SlotContainer sc;
	clan::Font font;

	std::shared_ptr<HSVSpriteBatch> sprite_batcher;
	std::shared_ptr<HSVSprite> car1;
	std::shared_ptr<HSVSprite> car2;

	uint64_t last_fps_update;
	uint64_t last_time;

	bool quit = false;
	int fps = 0;
	std::string fps_text;
	float hue_offset = 0.0;
};
