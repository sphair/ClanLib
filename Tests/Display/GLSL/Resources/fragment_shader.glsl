
uniform mat2 matrix2_a;
uniform mat2 matrix2_b;
uniform mat2 matrix2_result;
uniform vec2 vector2_a;
uniform vec2 vector2_result;

uniform mat3 matrix3_a;
uniform mat3 matrix3_b;
uniform mat3 matrix3_result;
uniform vec3 vector3_a;
uniform vec3 vector3_result;

uniform mat4 matrix4_a;
uniform mat4 matrix4_b;
uniform mat4 matrix4_result;
uniform vec4 vector4_a;
uniform vec4 vector4_result;

uniform int test_id;

void main() 
{
	if (test_id == 1)
	{
		if ((matrix2_a * matrix2_b) == matrix2_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 2)
	{
		if ((matrix3_a * matrix3_b) == matrix3_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 3)
	{
		if ((matrix4_a * matrix4_b) == matrix4_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 4)
	{
		if ((matrix2_a - matrix2_b) == matrix2_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 5)
	{
		if ((matrix3_a - matrix3_b) == matrix3_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 6)
	{
		if ((matrix4_a - matrix4_b) == matrix4_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 7)
	{
		if ((vector2_a * matrix2_a) == vector2_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 8)
	{
		if ((matrix2_a * vector2_a) == vector2_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 9)
	{
		if ((vector3_a * matrix3_a) == vector3_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 10)
	{
		if ((matrix3_a * vector3_a) == vector3_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 11)
	{
		if ((vector4_a * matrix4_a) == vector4_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}
	else if (test_id == 12)
	{
		if ((matrix4_a * vector4_a) == vector4_result)
		{
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
		else
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}	
	else
	{
			gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
	}

}
