#version 330

in vec2 UV;
out vec4 FragColor;

layout(std140) uniform Uniforms
{
	float Blueness;
};

void main()
{
	FragColor = vec4(UV, Blueness, 1.0);
}
