struct PixelIn
{
	float4 screenpos : SV_Position;
	float4 color : PixelColor;
};
struct PixelOut
{
	float4 color : SV_Target0;
};
PixelOut main(PixelIn input)
{
	PixelOut output;
	output.color = input.color;
	return output;
}
