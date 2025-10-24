attribute vec4 in_position;
attribute vec3 in_normal;
uniform mat4 cl_ModelViewMatrix;
uniform mat4 cl_ModelViewProjectionMatrix;
uniform mat3 cl_NormalMatrix;

varying vec3 N;
varying vec3 v;
varying vec2 texcoord;

uniform vec4 area;

void main(void)
{
//	v = vec3(cl_ModelViewMatrix * in_position);
	v = vec3(cl_ModelViewMatrix * gl_Vertex);
	N = normalize(cl_NormalMatrix * in_normal);

	texcoord = (gl_Vertex.xz - area.xy) / area.zw;
	gl_Position = cl_ModelViewProjectionMatrix * gl_Vertex;
}
