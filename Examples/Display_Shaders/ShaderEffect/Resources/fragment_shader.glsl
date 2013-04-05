#version 330

out vec4 FragColor;

layout(std140) uniform Uniforms
{
	uniform vec3 resolution;
	uniform float time;
};

void main(void)
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
