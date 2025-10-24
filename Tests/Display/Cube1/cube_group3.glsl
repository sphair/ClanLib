/*
	cube_group3, GLSL vertex shader.
	
	Executes the following logic by using a vertex shader:

	Translated(-11.0,-8.0,0.0);
	Rotated(25.0,0.0,0.0,1.0);
	Rotated(-50.0,0.0,1.0,0.0);
	Translated(spinner0*4,2.0,-20.0);
	Rotated(spinner1*100,400.0,0.0,0.0);
	Rotated(sin0 * 25.0,1.0,0.0,0.0);
	Rotated(k * 15.0,1.0,0.0,0.0);
	Rotated(180.0,1.0,0.0,0.0);
	Translated(k*-4.0,0.0,-1.0);
	Translated(7.0,0.0,0.0);
*/

varying vec3 N;
varying vec3 v;
//varying vec2 gl_TexCoord[1];

uniform float spinner0;
uniform float spinner1;
uniform float sin0;
attribute float k;

mat4x4 translate(mat4x4 matrix, float x, float y, float z);
mat4x4 rotate(mat4x4 matrix, float angle_degrees, float x, float y, float z);

void main()
{
	mat4x4 matrix;
	mat3x3 normal_matrix;
	vec4 transformed_normal4;
	vec3 transformed_normal3;
	
	matrix = mat4x4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);

	matrix = translate(matrix, -11.0,-8.0,0.0);
	matrix = rotate(matrix, 25.0,0.0,0.0,1.0);
	matrix = rotate(matrix, -50.0,0.0,1.0,0.0);
	matrix = translate(matrix, spinner0*4,2.0,-20.0);
	matrix = rotate(matrix, spinner1*100,1.0,0.0,0.0);
	matrix = rotate(matrix, sin0 * 25.0,1.0,0.0,0.0);
	matrix = rotate(matrix, k * 15.0,1.0,0.0,0.0);
	matrix = rotate(matrix, 180.0,1.0,0.0,0.0);
	matrix = translate(matrix, k*-4.0,0.0,-1.0);
	matrix = translate(matrix, 7.0,0.0,0.0);
	
	transformed_normal4 = matrix * vec4(gl_Normal.x, gl_Normal.y, gl_Normal.z, 1.0);
	transformed_normal3 = vec3(transformed_normal4.x, transformed_normal4.y, transformed_normal4.z);
	
	//normal_matrix = transpose(inverse(mat3x3(matrix)));
	normal_matrix = gl_NormalMatrix;

	v = vec3(gl_ModelViewMatrix * matrix * gl_Vertex);
	N = normalize(normal_matrix * transformed_normal3);

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * matrix * gl_Vertex;
	gl_TexCoord[0].st = gl_MultiTexCoord0.st;
}

mat4x4 rotate(mat4x4 matrix, float angle_degrees, float x, float y, float z)
{
	float angle_rad = radians(angle_degrees);
	float c = cos(angle_rad);
	float s = sin(angle_rad);
	
	return matrix * transpose(mat4x4(
		x*x*(1.0 - c) + c,
		x*y*(1.0 - c) - z*s,
		x*z*(1.0 - c) + y*s,
		0.0,
		
		y*x*(1.0 - c) + z*s,
		y*y*(1.0 - c) + c,
		y*z*(1.0 - c) - x*s,
		0.0,
		
		x*z*(1.0 - c) - y*s,
		y*z*(1.0 - c) + x*s,
		z*z*(1.0 - c) + c,
		0.0,
		
		0.0,
		0.0,
		0.0,
		1.0));
}

mat4x4 translate(mat4x4 matrix, float x, float y, float z)
{
	return matrix * mat4x4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		  x,   y,   z, 1.0);
}
