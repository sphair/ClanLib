
struct VertexIn
{
	float4 PositionInObject : AttrPositionInObject;
	uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
	float4 PositionInProjection : SV_Position;
	float4 PositionInEye : PixelPositionInEye;
	float4 Color : PixelColor;
	float4 Range : PixelRange; // pow(attenuation_end, 2), pow(attenation_start, 2), 1/pow(attenuation_end-attenuation_start, 2), hotspot
	float4 SpotX : PixelSpotX;
	float4 SpotY : PixelSpotY;
	float4 SpotZ : PixelSpotZ;
};

cbuffer Uniforms
{
	float4x4 ObjectToEye;
	float4x4 EyeToProjection;
	float rcp_f;
	float rcp_f_div_aspect;
	float2 two_rcp_viewport_size;
	uint padding; // 16 byte boundary alignment
}

Texture1D InstanceTexture;

VertexOut main(VertexIn input)
{
	VertexOut output;

	uint LightOffset = input.InstanceId * 6;

	output.PositionInEye = InstanceTexture.Load(uint2(LightOffset  + 0, 0));
	output.Color = InstanceTexture.Load(uint2(LightOffset  + 1, 0));
	output.Range = InstanceTexture.Load(uint2(LightOffset  + 2, 0));
	output.SpotX = InstanceTexture.Load(uint2(LightOffset  + 3, 0));
	output.SpotY = InstanceTexture.Load(uint2(LightOffset  + 4, 0));
	output.SpotZ = InstanceTexture.Load(uint2(LightOffset  + 5, 0));

	output.PositionInProjection = mul(EyeToProjection, output.PositionInEye);

	return output;
}
