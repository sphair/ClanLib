
struct VertexIn
{
	float4 position : VertexPosition;
	float4 color : VertexColor;
};
struct VertexOut
{
	float4 position : SV_Position;
	float4 color : PixelColor;
};
VertexOut main(VertexIn input)
{
	VertexOut output;
	output.position = input.position;
	output.color = input.color;
	return output;
}
