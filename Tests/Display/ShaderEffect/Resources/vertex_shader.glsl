#version 330

in vec4 PositionInProjection;
in vec2 AttrUV;
out vec2 UV;

void main()
{
	gl_Position = PositionInProjection;
	UV = AttrUV;
}
