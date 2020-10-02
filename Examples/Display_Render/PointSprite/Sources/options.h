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
**    Mark Page
*/

#pragma once

#include "../../../ThemeAero/Sources/theme.h"

class Options : public clan::TextureWindow
{
public:
	Options(clan::Canvas &canvas);
	virtual ~Options();

	int num_particles;
	int point_size;

private:
	void update_all_slider_text();
	void slider_num_particles_changed();
	void slider_point_size_changed();
	std::shared_ptr<clan::LabelView> create_slider_label(int xpos, int ypos);
	std::shared_ptr<clan::SliderView> create_slider(int xpos, int ypos);
	float get_value(std::shared_ptr<clan::SliderView> slider);
private:
	std::shared_ptr<clan::SliderView> slider_num_particles;
	std::shared_ptr<clan::LabelView> label_num_particles;

	std::shared_ptr<clan::SliderView> slider_point_size;
	std::shared_ptr<clan::LabelView> label_point_size;


};

