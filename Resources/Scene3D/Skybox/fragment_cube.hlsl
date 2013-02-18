
struct PixelIn
{
	float4 Position : SV_Position;
	float3 CubeDirection : CubeDirection;
};

struct PixelOut
{
	float4 FragColor : SV_Target0;
};

TextureCube SkyboxTexture;
SamplerState SkyboxTextureSampler;

PixelOut main(PixelIn input)
{
	PixelOut output;
	output.FragColor = SkyboxTexture.Sample(SkyboxTextureSampler, input.CubeDirection);
	return output;
}
