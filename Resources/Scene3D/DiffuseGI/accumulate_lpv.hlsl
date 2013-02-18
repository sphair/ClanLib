
#define TILE_SIZE 16

[numthreads(TILE_SIZE,TILE_SIZE,1)]
void main(uint3 globalThreadID : SV_DispatchThreadID)
{
}
