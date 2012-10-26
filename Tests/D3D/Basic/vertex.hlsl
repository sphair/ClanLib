
cbuffer Global
{
	uniform float4x4 objectToProjection;
	uniform float4 ccolor;
}

struct VertexIn
{
	float4 position : Position;
	float4 color : Color;
	float2 texcoord : UV;
};

struct VertexOut
{
	float4 position : SV_Position;
	float4 color : PixelColor;
	float2 texcoord : PixelUV;
};

VertexOut main(VertexIn input)
{
	VertexOut output;
	output.position = mul(objectToProjection, input.position);
	output.color = lerp(ccolor, input.color, 0.8f);
	output.texcoord = input.texcoord;
	return output;
}
