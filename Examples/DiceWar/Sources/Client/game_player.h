
#pragma once

class GamePlayer
{
public:
	GamePlayer(CL_String &name, int id, CL_Color &color);

	CL_String name;
	int id;
	CL_Color color;
};
