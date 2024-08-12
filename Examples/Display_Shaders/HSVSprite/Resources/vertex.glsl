#version 430

in vec4 Position;
in float HueOffset0;
in vec2 TexCoord0;

out float HueOffset;
out vec2 TexCoord;

void main(void)
{
	gl_Position = Position;
	HueOffset = HueOffset0;
	TexCoord = TexCoord0;
}

