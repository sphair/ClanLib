struct PixelIn
{
	float4 screenpos : SV_Position;
	float4 color : PixelColor;
	float2 uv : PixelTexCoord;
	int texindex : PixelTexIndex;
};
struct PixelOut
{
	float4 color : SV_Target0;
};
Texture2D Texture0;
Texture2D Texture1;
Texture2D Texture2;
Texture2D Texture3;
SamplerState Sampler0;
SamplerState Sampler1;
SamplerState Sampler2;
SamplerState Sampler3;
PixelOut main(PixelIn input)
{
	int index = input.texindex;
	PixelOut output;
	if (index == 0)
		output.color = Texture0.Sample(Sampler0, input.uv) * input.color;
	else if (index == 1)
		output.color = Texture1.Sample(Sampler1, input.uv) * input.color;
	else if (index == 2)
		output.color = Texture2.Sample(Sampler2, input.uv) * input.color;
	else if (index == 3)
		output.color = Texture3.Sample(Sampler3, input.uv) * input.color;
	else
		output.color = input.color;
	return output;
}
