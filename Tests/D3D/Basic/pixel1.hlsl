
struct PixelIn
{
	float4 ScreenPos : SV_Position;
};

struct PixelOut
{
	float4 FragColor : SV_Target0;
};

PixelOut main(PixelIn input)
{
	PixelOut output;
	output.FragColor = float4(input.ScreenPos.x / 640, input.ScreenPos.y / 480, 0, 1);
	return output;
}
