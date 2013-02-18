
cbuffer Uniforms
{
	float4x4 ObjectToEye;
	float4x4 EyeToProjection;
	float RcpDepthFadeDistance;
	uint InstanceVectorsOffset;
}

struct VertexIn
{
	float4 Position : AttrPosition;
	uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
	float4 Position : SV_Position;
	float3 Texcoord : PixelTexcoord;
	float DepthInEye : PixelDepth;
	float3 Color : PixelColor;
};

Texture2D InstanceTexture;

Texture2D ColorGradientTexture;
SamplerState ColorGradientSampler;

VertexOut main(VertexIn input)
{
	float4 InstanceData0 = InstanceTexture.Load(uint3(InstanceVectorsOffset + input.InstanceId * 2, 0, 0));
	float4 InstanceData1 = InstanceTexture.Load(uint3(InstanceVectorsOffset + input.InstanceId * 2 + 1, 0, 0));
	float4 positionInObject = float4(InstanceData0.xyz, 1);
	float sizeInEye = InstanceData0.w;
	float life = InstanceData1.x;

	VertexOut output;
	if (life >= 0)
	{
		float4 positionInEye = mul(ObjectToEye, positionInObject);
		positionInEye += float4(input.Position.xyz, 0) * sizeInEye;

		output.Position = mul(EyeToProjection, positionInEye);
		output.Texcoord = float3(input.Position.xy * 0.5 + 0.5, life);
		output.DepthInEye = positionInEye.z;
		output.Color = ColorGradientTexture.SampleLevel(ColorGradientSampler, float2(life, 0), 0);
	}
	else
	{
		output.Position = float4(0,0,0,0);
	}
	return output;
}
