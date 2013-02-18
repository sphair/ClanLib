
struct VertexIn
{
	float4 Position : AttrPosition;
	uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
	float4 Position : SV_Position;
	float2 UV : PixelUV;
	float Distance : PixelDistance;
};

cbuffer Uniforms
{
	float4x4 ObjectToEye;
	float4x4 EyeToProjection;
}

Texture2D InstanceTexture;

VertexOut main(VertexIn input)
{
	float4 InstanceData = InstanceTexture.Load(uint3(input.InstanceId, 0, 0));
	float4 PositionInEye = mul(ObjectToEye, float4(InstanceData.xyz, 1));
	float Distance = length(PositionInEye);
	PositionInEye += float4(input.Position.xyz, 0) * InstanceData.w;

	VertexOut output;
	output.Position = mul(EyeToProjection, PositionInEye);
	output.UV = input.Position.xy * 0.5 + 0.5;
	output.Distance = (1 - Distance / 1000) * 0.7;
	return output;
}
