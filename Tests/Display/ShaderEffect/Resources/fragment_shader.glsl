#version 330

in vec2 UV;
out vec4 FragColor;

void main()
{
	FragColor = vec4(UV, 0.0, 1.0);
}
