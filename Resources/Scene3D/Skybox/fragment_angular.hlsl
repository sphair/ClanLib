
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
	float3 n = normalize(input.CubeDirection);
	float len = length(n.xy);
	float r = len != 0 ? 0.159154943 * acos(n.z) / len : 0;
	float2 uv = 0.5 + n.xy * r;

	PixelOut output;
	output.FragColor = SkyboxTexture.Sample(SkyboxTextureSampler, uv);
	return output;
}
