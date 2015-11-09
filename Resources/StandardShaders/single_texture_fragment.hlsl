struct PixelIn
{
	float4 screenpos : SV_Position;
	float4 color : PixelColor;
	float2 uv : PixelTexCoord;
};
struct PixelOut
{
	float4 color : SV_Target0;
};
Texture2D Texture0;
SamplerState Sampler0;
PixelOut main(PixelIn input)
{
	PixelOut output;
	output.color = Texture0.Sample(Sampler0, input.uv) * input.color;
	return output;
}