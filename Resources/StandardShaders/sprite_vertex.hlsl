struct VertexIn
{
	float4 position : VertexPosition;
	float4 color : VertexColor;
	float2 uv : VertexTexCoord;
	int texindex : VertexTexIndex;
};
struct VertexOut
{
	float4 position : SV_Position;
	float4 color : PixelColor;
	float2 uv : PixelTexCoord;
	int texindex : PixelTexIndex;
};
VertexOut main(VertexIn input)
{
	VertexOut output;
	output.position = input.position;
	output.color = input.color;
	output.uv = input.uv;
	output.texindex = input.texindex;
	return output;
}