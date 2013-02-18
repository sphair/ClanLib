
#define TILE_SIZE 16

Texture2D<float4> in_final;
RWTexture2D<float4> out_final;

[numthreads(TILE_SIZE,TILE_SIZE,1)]
void main(uint3 globalThreadID : SV_DispatchThreadID)
{
	int2 pos = int2(globalThreadID.x, globalThreadID.y);
	float4 pixel = in_final.Load(pos);
	out_final[pos] = pixel;
}
