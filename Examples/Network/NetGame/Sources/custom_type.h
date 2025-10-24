#pragma once

#include <ClanLib/network.h>

class CustomType
{
public:
	CustomType(int x, int y, int z) : x(x), y(y), z(z) {};

	CustomType(CL_NetGameEventValue value)
	{
		if(value.is_complex() && value.get_member_count() == 3)
		{
			x = value.get_member(0);
			y = value.get_member(1);
			z = value.get_member(2);
		}
		else
			throw CL_Exception("Invalid CustomType CL_NetGameEventValue");
	}

	operator CL_NetGameEventValue() const 
	{ 
		CL_NetGameEventValue value(CL_NetGameEventValue::complex);
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
