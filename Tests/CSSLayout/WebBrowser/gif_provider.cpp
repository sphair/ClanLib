
#include "precomp.h"
#include "gif_provider.h"
#include "gif_lib.h"

using namespace clan;

PixelBuffer GIFProvider::load(IODevice &device)
{
	int result;
	int error;
	GifFileType *handle = DGifOpen(&device, &GIFProvider::on_read_input, &error);
	if (handle == 0)
		throw Exception("DGifOpen failed");

	result = DGifSlurp(handle);
	if (result == GIF_ERROR)
	{
		DGifCloseFile(handle);
		throw Exception("DGifSlurp failed");
	}

	int width = handle->SWidth;
	int height = handle->SHeight;

	PixelBuffer buffer(width, height, tf_bgra8);
	unsigned int *pixels = reinterpret_cast<unsigned int *>(buffer.get_data());

	if (handle->SBackGroundColor == 255) // transparent
	{
		for (int i = 0; i < width*height; i++)
			pixels[i] = 0x00000000;
	}
	else if (handle->SColorMap && handle->SColorMap->ColorCount > handle->SBackGroundColor)
	{
		GifColorType bgcolor = handle->SColorMap->Colors[handle->SBackGroundColor];
		unsigned int c = 0xff000000 + (((unsigned int)bgcolor.Red) << 16) + (((unsigned int)bgcolor.Green) << 8) + ((unsigned int)bgcolor.Blue);
		for (int i = 0; i < width*height; i++)
			pixels[i] = c;
	}
	else
	{
		for (int i = 0; i < width*height; i++)
			pixels[i] = 0xff000000;
	}

	bool is_transparent = false;
	unsigned char transparentcolor = 0;
	for(int j=0; j<handle->SavedImages[0].ExtensionBlockCount;j++)
	{
		if (handle->SavedImages[0].ExtensionBlocks[j].Function == GRAPHICS_EXT_FUNC_CODE)
		{
			if(handle->SavedImages[0].ExtensionBlocks[j].ByteCount==4)
			{
				if((handle->SavedImages[0].ExtensionBlocks[j].Bytes[0] & 0x01)>0)
				{
					is_transparent = true;
					transparentcolor = (unsigned char)handle->SavedImages[0].ExtensionBlocks[j].Bytes[3];
				}
			}
		}
	}

	if (handle->ImageCount > 0)
	{
		ColorMapObject *colormap = handle->SavedImages[0].ImageDesc.ColorMap;
		if (colormap == 0)
			colormap = handle->SColorMap;
		if (colormap != 0)
		{
	/*		if (handle->SavedImages[0].ImageDesc.Interlace)
			{
				for (int pass = 0; pass < 4; pass++)
				{
					for (int i = interlaced_offset[pass]; i < Height; i += interlaced_jumps[pass])
					{
					}
				}
			}
			else
			{
			}*/
			if (handle->SavedImages[0].ImageDesc.Left + handle->SavedImages[0].ImageDesc.Width <= width &&
				handle->SavedImages[0].ImageDesc.Left + handle->SavedImages[0].ImageDesc.Height <= height)
			{
				for (int y = 0; y < handle->SavedImages[0].ImageDesc.Height; y++)
				{
					for (int x = 0; x < handle->SavedImages[0].ImageDesc.Width; x++)
					{
						unsigned char v = handle->SavedImages[0].RasterBits[x+y*handle->SavedImages[0].ImageDesc.Width];
						unsigned int c = 0x00000000;
						if (is_transparent && transparentcolor == v)
						{
							c = 0x00000000;
						}
						else if (v != 255 && v < colormap->ColorCount)
						{
							GifColorType fgcolor = colormap->Colors[v];
							c = 0xff000000 + (((unsigned int)fgcolor.Red) << 16) + (((unsigned int)fgcolor.Green) << 8) + ((unsigned int)fgcolor.Blue);
						}

						int dx = x + handle->SavedImages[0].ImageDesc.Left;
						int dy = y + handle->SavedImages[0].ImageDesc.Top;
						pixels[dx+dy*width] = c;
					}
				}
			}
		}
	}

	DGifCloseFile(handle);
	return buffer;
}

int GIFProvider::on_read_input(GifFileType *filetype, GifByteType *buffer, int length)
{
	IODevice *iodevice = reinterpret_cast<IODevice *>(filetype->UserData);
	return iodevice->read(buffer, length, false);
}

//
//  Appendix E. Interlaced Images.
//
//  The rows of an Interlaced images are arranged in the following order:
//  
//        Group 1 : Every 8th. row, starting with row 0.              (Pass 1)
//        Group 2 : Every 8th. row, starting with row 4.              (Pass 2)
//        Group 3 : Every 4th. row, starting with row 2.              (Pass 3)
//        Group 4 : Every 2nd. row, starting with row 1.              (Pass 4)
//  
//const int GIFProvider::interlaced_offset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
//const int GIFProvider::interlaced_jumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */
