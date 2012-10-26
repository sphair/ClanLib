
kernel void main(global int *pixels, int width)
{
	int x = get_global_id(0);
	int y = get_global_id(1);
	pixels[x+y*width] = (x << 24) + (y << 16) + 0xff;
}
