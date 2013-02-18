
cbuffer Uniforms
{
	float4x4 ObjectToEye;
	float4x4 EyeToProjection;
	float RcpDepthFadeDistance;
	uint InstanceVectorsOffset;
};

struct PixelIn
{
	float4 ScreenPos : SV_Position;
	float3 Texcoord : PixelTexcoord;
	float DepthInEye : PixelDepth;
	float3 Color : PixelColor;
};

struct PixelOut
{
	float4 FragColor : SV_Target0;
};

Texture3D ParticleTexture;
Texture2D NormalZTexture;

SamplerState ParticleSampler;

PixelOut main(PixelIn input)
{
	PixelOut output;

	float4 textureColor = ParticleTexture.Sample(ParticleSampler, input.Texcoord);
	float fragmentDepthInEye = NormalZTexture.Load(uint3(input.ScreenPos.xy, 0)).w;

	float depthDiff = fragmentDepthInEye - input.DepthInEye;
	float depthFade = saturate(depthDiff * RcpDepthFadeDistance);

	// Workaround since skybox fragments has 1 as depth value
	if (fragmentDepthInEye == 1)
		depthFade = 1;

	// To do: apply light on particles

	output.FragColor = textureColor * float4(input.Color, 1) * depthFade;
	return output;
}
