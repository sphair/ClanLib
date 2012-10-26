
struct PixelIn
{
	float4 ScreenPos : SV_Position;
	float2 UV : TexCoord;
};

struct PixelOut
{
	float4 FragColor : SV_Target0;
};

Texture2D SourceTexture;

SamplerState SourceSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

PixelOut main(PixelIn input)
{
	PixelOut output;
	//output.FragColor = SourceTexture.Load(int3(input.ScreenPos.xy, 0));
	output.FragColor = SourceTexture.Sample(SourceSampler, input.UV);
	return output;
}
