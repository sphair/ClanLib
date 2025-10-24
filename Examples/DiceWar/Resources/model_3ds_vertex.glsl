
attribute vec4 in_position;
attribute vec3 in_normal;
attribute vec2 in_texcoord;

varying vec3 N;
varying vec3 v;
varying vec2 texcoord;

void main(void)
{
	v = vec3(gl_ModelViewMatrix * in_position);
	N = normalize(gl_NormalMatrix * in_normal);
	texcoord = in_texcoord;
	gl_Position = gl_ModelViewProjectionMatrix * in_position;
}
