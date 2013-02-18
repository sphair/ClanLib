
struct PixelIn
{
	float4 ScreenPos : SV_Position;
	float2 TexCoord : TexCoord;
};

struct PixelOut
{
	float4 FragColor : SV_Target0;
};

Texture2D FinalColors;
SamplerState FinalColorsSampler;

Texture2D BloomColors;
SamplerState BloomColorsSampler;

float4 sRGB(float4 c);

PixelOut main(PixelIn input)
{
	PixelOut output;
	float4 color = FinalColors.Sample(FinalColorsSampler, input.TexCoord);
	float4 bloom = BloomColors.Sample(BloomColorsSampler, input.TexCoord);
	color.xyz /= max(max(color.x, max(color.y, color.z)), 1);
	output.FragColor = sRGB(float4(color.rgb, 1) + bloom);
	return output;
}

float4 sRGB(float4 c)
{
	float gamma = 1.0 / 2.2;
	return float4(pow(c.rgb, (float3)gamma), c.a);
	// return float4(sqrt(c.rgb), c.a); // cheaper, but assuming gamma of 2.0 instead of 2.2
}
