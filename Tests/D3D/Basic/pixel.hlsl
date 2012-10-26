
struct PixelIn
{
	float4 screenpos : SV_Position;
	float4 color : PixelColor;
	float2 texcoord : PixelUV;
};

struct PixelOut
{
	float4 color : SV_Target0;
};

Texture2D TestTexture;
SamplerState TestSampler;

PixelOut main(PixelIn input)
{
	PixelOut output;
	output.color = TestTexture.Sample(TestSampler, input.texcoord) * input.color;
	return output;
}
