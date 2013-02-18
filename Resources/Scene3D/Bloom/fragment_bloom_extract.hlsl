
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

PixelOut main(PixelIn input)
{
	PixelOut output;
	float4 color = FinalColors.Sample(FinalColorsSampler, input.TexCoord);
	const float blurBegin = 1.0;
	const float blurEnd = 2.0;
	output.FragColor = float4(clamp((color.rgb - blurBegin) / (blurEnd - blurBegin), 0, 1), 0);
	return output;
}
