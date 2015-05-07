in vec4 PositionInProjection;
out vec2 UV;

void main()
{
	gl_Position = PositionInProjection;
	UV = PositionInProjection.xy * 0.5 + 0.5;
	UV.y = 1 - UV.y;
}
