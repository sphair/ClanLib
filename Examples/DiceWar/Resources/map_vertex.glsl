attribute vec4 in_position;
attribute vec3 in_normal;

varying vec3 N;
varying vec3 v;
varying vec2 texcoord;

uniform vec4 area;

void main(void)
{
//	v = vec3(gl_ModelViewMatrix * in_position);
	v = vec3(gl_ModelViewMatrix * gl_Vertex);
	N = normalize(gl_NormalMatrix * in_normal);

	texcoord = (gl_Vertex.xz - area.xy) / area.zw;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
