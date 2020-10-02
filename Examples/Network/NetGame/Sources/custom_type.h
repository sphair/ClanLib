#pragma once

#include <ClanLib/network.h>

class CustomType
{
public:
	CustomType(int x, int y, int z) : x(x), y(y), z(z) {};

	CustomType(NetGameEventValue value)
	{
		if(value.is_complex() && value.get_member_count() == 3)
		{
			x = value.get_member(0);
			y = value.get_member(1);
			z = value.get_member(2);
		}
		else
			throw Exception("Invalid CustomType NetGameEventValue");
	}

	operator NetGameEventValue() const 
	{ 
		NetGameEventValue value(NetGameEventValue::Type::complex);
		value.add_member(x);
		value.add_member(y);
		value.add_member(z);
		return value; 
	}

	int get_x() const { return x; }
	int get_y() const { return y; }
	int get_z() const { return z; }

private:
	int x, y, z;
};
