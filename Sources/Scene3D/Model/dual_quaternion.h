/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
namespace clan
{

template<typename Type>
class DualQuaternionx
{
public:
	DualQuaternionx();
	DualQuaternionx(const clan::Vec3f translate, const clan::Quaternionf orientation);
	clan::Mat4<Type> to_matrix();

	clan::Quaternionx<Type> first;
	clan::Quaternionx<Type> second;
};

template<typename Type>
DualQuaternionx<Type>::DualQuaternionx()
{
}

template<typename Type>
DualQuaternionx<Type>::DualQuaternionx(const clan::Vec3f translate, const clan::Quaternionf orientation)
{
	first = orientation;
	second.x = Type( 0.5) * ( translate.x * orientation.w + translate.y * orientation.z - translate.z * orientation.y);
	second.y = Type( 0.5) * (-translate.x * orientation.z + translate.y * orientation.w + translate.z * orientation.x);
	second.z = Type( 0.5) * ( translate.x * orientation.y - translate.y * orientation.x + translate.z * orientation.w);
	second.w = Type(-0.5) * ( translate.x * orientation.x + translate.y * orientation.y + translate.z * orientation.z);
}

template<typename Type>
clan::Mat4<Type> DualQuaternionx<Type>::to_matrix()
{
	Type length = first.magnitude();
	Type x = first.x;
	Type y = first.y;
	Type z = first.z;
	Type w = first.w;
	Type t1 = second.x;
	Type t2 = second.y;
	Type t3 = second.z;
	Type t0 = second.w;

	clan::Mat4<Type> result;
	result.m[0][0] = w*w + x*x - y*y - z*z; 
	result.m[1][0] = 2*x*y - 2*w*z;
	result.m[2][0] = 2*x*z + 2*w*y;
	result.m[0][1] = 2*x*y + 2*w*z;
	result.m[1][1] = w*w + y*y - x*x - z*z;
	result.m[2][1] = 2*y*z - 2*w*x;
	result.m[0][2] = 2*x*z - 2*w*y;
	result.m[1][2] = 2*y*z + 2*w*x;
	result.m[2][2] = w*w + z*z - x*x - y*y;

	result.m[3][0] = -2*t0*x + 2*t1*w - 2*t2*z + 2*t3*y;
	result.m[3][1] = -2*t0*y + 2*t1*z + 2*t2*w - 2*t3*x;
	result.m[3][2] = -2*t0*z - 2*t1*y + 2*t2*x + 2*t3*w;

	result.m[0][3] = Type(0);
	result.m[1][3] = Type(0);
	result.m[2][3] = Type(0);
	result.m[3][3] = Type(1);

	result /= length;
	return result;
}

typedef DualQuaternionx<float> DualQuaternionf;
typedef DualQuaternionx<double> DualQuaterniond;

}

