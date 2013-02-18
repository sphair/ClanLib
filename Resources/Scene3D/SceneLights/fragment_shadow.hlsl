
struct PixelIn
{
	float4 ScreenPos : SV_Position;
	float4 PositionInEye : PositionInEye;
};

struct PixelOut
{
	float4 FragMoment : SV_Target0;
};

PixelOut main(PixelIn input)
{
	float distanceToLight = length(input.PositionInEye);
	PixelOut output;
	output.FragMoment = float4(distanceToLight, distanceToLight * distanceToLight, 0.0f, 0.0f);
	return output;
}
