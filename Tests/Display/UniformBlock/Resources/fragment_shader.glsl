#version 140

uniform TestBlock
{
	vec3 src_color;
};

//uniform vec3 src_color;

void main()
{
	gl_FragColor.rgb = src_color;
	gl_FragColor.a = 1.0;
}

