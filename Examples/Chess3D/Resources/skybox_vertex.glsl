
attribute vec4 in_position;

varying vec3 v;

void main()
{
	v = vec3(in_position);
	gl_Position = gl_ModelViewProjectionMatrix * in_position;
}
