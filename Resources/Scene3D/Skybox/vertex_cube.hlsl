
struct VertexIn
{
	float4 Position : AttrPosition;
	uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
	float4 Position : SV_Position;
	float3 CubeDirection : CubeDirection;
};

cbuffer Uniforms
{
	float4x4 ObjectToEye;
	float4x4 EyeToProjection;
}

VertexOut main(VertexIn input)
{
	VertexOut output;
	output.Position = mul(EyeToProjection, mul(ObjectToEye, input.Position));
	output.CubeDirection = input.Position;
	return output;
}
