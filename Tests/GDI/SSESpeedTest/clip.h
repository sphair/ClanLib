
void clipScanline(Scanline *d, int clipx1, int clipx2)
{
	float xscr1 = d->x1 * d->half_viewport_width + d->viewport_center;
	float xscr2 = d->x2 * d->half_viewport_width + d->viewport_center;
	int x1 = (int)(xscr1+0.5f);
	int x2 = ((int)(xscr2-0.5f))+1;
	x1 = cl_max(x1, clipx1);
	x2 = cl_min(x2, clipx2);
	d->startx = x1;
	d->endx = x2;
}
