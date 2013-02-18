
struct PixelIn
{
	float4 Position : SV_Position;
	float3 CubeDirection : CubeDirection;
};

struct PixelOut
{
	float4 FragColor : SV_Target0;
};

Texture2D SkyboxTexture;
SamplerState SkyboxTextureSampler;

PixelOut main(PixelIn input)
{
	float2 uv = normalize(input.CubeDirection).xy * 0.5 + 0.5;

	PixelOut output;
	output.FragColor = SkyboxTexture.Sample(SkyboxTextureSampler, uv);
	return output;
}
