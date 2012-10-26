
struct VertexIn
{
	float4 PositionInProjection : PositionInProjection;
};

struct VertexOut
{
	float4 PositionInProjection : SV_Position;
};

VertexOut main(VertexIn input)
{
	VertexOut output;
	output.PositionInProjection = input.PositionInProjection;
	return output;
}
