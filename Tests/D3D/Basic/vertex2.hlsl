
struct VertexIn
{
	float4 PositionInProjection : PositionInProjection;
};

struct VertexOut
{
	float4 PositionInProjection : SV_Position;
	float2 UV : TexCoord;
};

VertexOut main(VertexIn input)
{
	VertexOut output;
	output.PositionInProjection = input.PositionInProjection;
	output.UV = input.PositionInProjection.xy * 0.5 + 0.5;
	return output;
}
