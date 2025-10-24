/*
**  CL_FlexibleMap
**  Copyright (c) 2005  Erik Ylvisaker
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Erik Ylvisaker
**    (if your name is missing here, please add it)
*/


#include "CL_FlexibleMap.h"

#include <algorithm>

CL_FlexibleMap::CL_FlexibleMap(void)
{
	block_data = NULL;
	anim_data = NULL;

	bgfx_data = NULL;

	tiles = NULL;

	palette = NULL;

	last_error = CL_FlexibleMap::ok;

	use_stagger = false;

	current_layer = 0;

	clear();

}

CL_FlexibleMap::~CL_FlexibleMap(void)
{
	clear();
}

void CL_FlexibleMap::clear()
{
	delete [] block_data;
	block_data = NULL;

	delete [] anim_data;
	anim_data = NULL;
	anim_data_count = 0;

	delete [] anim_frames;
	anim_frames = NULL;

	delete [] bgfx_data;
	bgfx_data = NULL;

	delete [] tiles;
	tiles = NULL;

	delete [] palette;
	palette = NULL;
	palette_size = 0;

	last_error = ok;

	map_version = 0;
	current_layer = 0;

	use_stagger = false;		
	map_source = NULL;


	last_anim_update = 0;
	frame_rate_independent_anims = false;

	memset(&MPHD, 0, sizeof(MPHD));

	for (int i = 0; i < (signed) layers.size(); i++)
	{
		delete layers[i];
	}

	layers.clear();
 
	// free extra chunks
	for (std::map<std::string, CL_FlexibleMapChunk*>::iterator i = unknown_chunks.begin();
		i != unknown_chunks.end(); i++)
	{
		delete i->second;
	}

	unknown_chunks.clear();


	for (int i = 0; i < 4; i++)
		author_info[i] = "";



}

void CL_FlexibleMap::read_chars(std::string &dest, int count)
{
	dest = "";

	for (int i = 0; i < count; i++)
		dest += map_source->read_char8();
}

int CL_FlexibleMap::read_zero_terminated_string(std::string &dest, int maxlen)
{
	dest = "";

	for (int i = 0; i < maxlen; i++)
	{
		char c = map_source->read_char8();

		dest += c;

		if (c == 0)
			break;
	}

	return i;
}


bool CL_FlexibleMap::load(const std::string &filename)
{
	clear();

	CL_InputSource_File file(filename);

	map_source = &file;

	bool retval = _load();

	map_source = NULL;

	return retval;

}



bool CL_FlexibleMap::_load()
{

	std::string buffer;
	cl_uint32 filesize;				// size of the file - header (which is 8 bytes)

	// mappy stores the first part in motorola format
	map_source->set_big_endian_mode();		

	// read beginning of file 
	read_chars(buffer, 4);

	if (buffer != "FORM")
	{
		last_error = notfmap;
		return false;
	}

	// Size of data is next
	filesize = map_source->read_uint32();

	// now we have FMAP
	read_chars(buffer, 4);

	if (buffer != "FMAP")
	{
		last_error = notfmap;
		return false;
	}


	// ok, now we read in the chunks
	// we probably should do something where we read in the MPHD chunk first.

	
	while (map_source->tell() < map_source->size())
	{
		bool chunk_check;

		read_chars(buffer, 4);		// read in chunk name
		unsigned int size = map_source->read_uint32();

		// sometimes mappy will store data in little-endian mode.  However,
		// chunk sizes will still be in big-endian mode.  So take that into account.
		if (map_source->is_little_endian())
		{
			SWAP_IF_LITTLE(size);
		}


		// store beginning of chunk
		int pos = map_source->tell();

		// parse reading of chunk
		if (buffer == "ATHR")					{ chunk_check = parse_ATHR(buffer, size); }
		else if (buffer == "MPHD")				{ chunk_check = parse_MPHD(buffer, size); }
		else if (buffer == "EDHD")				{ chunk_check = parse_EDHD(buffer, size); }
		else if (buffer == "CMAP")				{ chunk_check = parse_CMAP(buffer, size); }
		else if (buffer == "BKDT")				{ chunk_check = parse_BKDT(buffer, size); }
		else if (buffer == "ANDT")				{ chunk_check = parse_ANDT(buffer, size); }
		else if (buffer == "BGFX")				{ chunk_check = parse_BGFX(buffer, size); }
		else if (buffer == "BODY")				{ chunk_check = parse_BODY_LYR(buffer, size); }
		else if (buffer.substr(0, 3) == "LYR")	{ chunk_check = parse_BODY_LYR(buffer, size); }
		else									{ chunk_check = parse_custom_chunk(buffer, size); }

		if (!chunk_check)
			return false;

		// navigate to next chunk
		map_source->seek(pos + size, CL_InputSource::seek_set);
        
	}

	// from the mappy 1.2 docs:
	//These are the chunks as of V1.2:
	//ATHR - Up to 4 ASCII strings of author information, separated by 0 values, always an even size.
	//MPHD - Map header, see struct in the editor source download
	//EDHD - Editor information, see struct in mappy.c
	//CMAP - Colour palette for 8bit maps, red byte, green byte, blue byte for however many colours are needed (so usually 256*3 bytes).
	//BKDT - Block data. Contains BLKSTR structures for however many block structures were made.
	//ANDT - Animation data. Contains ANISTR structures for however many animation structures were made, and also animation data.
	//BGFX - The raw graphics in whatever format the map is in. Examples: 8bit: mapwidth*mapheight bytes per block, in forward format *numblocks 16bit: mapwidth*mapheight*2 bytes per block, each word contains 5 bits red, 6 bits green, 5 bits blue.
	//BODY - An array of short ints containing positive offsets into BKDT, and negative offsets into ANDT.
	//LYR? - Where ? is an ASCII number form 1 to 7. These are the same size and format as BODY, and allow object layers to be used.
	//You can add your own chunks to a map file, if you load it into mappy, when you save it, those additional chunks will be saved in the file, but not necessarily in the same place as before.


	/////////////////////
	//
	//  Ok, all the chunk data is loaded.  Now we need to convert the BGFX data
	//  into an actual ClanLib surface.

	if (!load_tiles())
		return false;

	// sort layers
	std::sort(layers.begin(), layers.end(), CL_Layer::compare);

	// set the animation time
	last_anim_update = CL_System::get_time();

	return true;

}

int CL_FlexibleMap::get_FMP0_5_bytes_per_pixel()
{
	int pixelbytes;

	switch(MPHD.blockdepth)
	{
		
		// 8-bit paletted maps must be converted to non-paletted.
		case 8:		pixelbytes = 1;					        break;

		case 15:   	pixelbytes = 2;							break;
		case 16:    pixelbytes = 2;							break;
		case 24:    pixelbytes = 3;							break;
		case 32:    pixelbytes = 4;							break;

		default:	last_error = could_not_convert_tiles;				return 0;

	}

	return pixelbytes * MPHD.blockheight * MPHD.blockwidth;
}


bool CL_FlexibleMap::load_tiles()
{
	CL_PixelFormat format;
	
	int pitch;
	int width = MPHD.blockwidth;
	int height = MPHD.blockheight;
	bool palette8 = false;

	if (CL_Endian::is_system_big())
		format = CL_PixelFormat::rgba8888;
	else
		format = CL_PixelFormat::abgr8888;

	pitch = 4 * width;


	// ClanLib doesn't support 8-bit paletted data, for obvious reasons.
	// It also doesn't seem to implement color-keying right.  The best thing to do
	// is manually convert all input images to rgba8888 format.
	unsigned int *data = new unsigned int[MPHD.blockwidth * MPHD.blockheight];

	// calculate how to write colors to the memory
	int redshift, blueshift, greenshift, alphashift;

	redshift   = get_color_shift_from_mask(format.get_red_mask());
	blueshift  = get_color_shift_from_mask(format.get_blue_mask());
	greenshift = get_color_shift_from_mask(format.get_green_mask());
	alphashift = get_color_shift_from_mask(format.get_alpha_mask());

	// allocate the surfaces
	tiles = new CL_Surface[MPHD.numblockgfx];

	if (MPHD.blockdepth == 8)	// we need to convert 8-bit
	{
		for (int n = 0; n < MPHD.numblockgfx; n++)
		{
			unsigned char *tile_data = &bgfx_data[n * MPHD.blockwidth * MPHD.blockheight];

			for (int i = 0; i < MPHD.blockwidth * MPHD.blockheight; i++)
			{
				unsigned int color = palette[tile_data[i]];
				unsigned int result;

				if (tile_data[i] == MPHD.ckey8bit)// && map_version > 0.35) 
					// colorkeying was apparently added in version 0.4
					// but some 0.3 maps seem to need it.  How to handle this?????
				{
					result = 0;
				}
				else
				{
					result  = ((color & 0x000000FF)      ) << blueshift;
					result |= ((color & 0x0000FF00) >>  8) << greenshift;
					result |= ((color & 0x00FF0000) >> 16) << redshift;
					result |= 0xFF << alphashift;
				}
				

				data[i] = result;
			}

			CL_PixelBuffer px(MPHD.blockwidth, MPHD.blockheight, pitch, format, data);
	
			tiles[n] = CL_Surface(px.to_format(format), CL_Surface::flag_none);

		}


	}
	else if (MPHD.blockdepth == 15 || MPHD.blockdepth == 16)
	{
/*		for (int i = 0; i < MPHD.blockwidth * MPHD.blockheight * MPHD.numblockgfx; i++)
		{
			unsigned short color = ((short*)bgfx_data)[i];


		}
*/
		unsigned int color_key = get_map_colorkey();

		unsigned short red_mask, green_mask, blue_mask;

		int src_red_shift, src_green_shift, src_blue_shift;
		int dest_red_shift = 0, dest_green_shift = 0, dest_blue_shift = 0;

		
 		if (MPHD.blockdepth == 15)
		{
			red_mask   = 0x7C00;
			green_mask = 0x03E0;
			blue_mask  = 0x001F;

			src_red_shift   = 10;    dest_red_shift = 3;
			src_green_shift = 5;	 dest_green_shift = 3;
			src_blue_shift  = 0;	 dest_blue_shift = 3;

		}
		else if (MPHD.blockdepth == 16)
		{
			red_mask   = 0xF800;
			green_mask = 0x07E0;
			blue_mask  = 0x001F;

			src_red_shift   = 11;    dest_red_shift   = 3;
			src_green_shift = 5;	 dest_green_shift = 2;
			src_blue_shift  = 0;	 dest_blue_shift  = 3;

		}


		for (int n = 0; n < MPHD.numblockgfx; n++)
		{
			unsigned char *tile_data = &bgfx_data[n * 2 * MPHD.blockwidth * MPHD.blockheight];

			for (int i = 0; i < MPHD.blockwidth * MPHD.blockheight; i++)
			{
				unsigned short src_color = ((short*)tile_data)[i];

				// I'm not sure about this...
				if (!CL_Endian::is_system_big())
				{
					unsigned int lo = src_color & 0xff;
					unsigned int hi = (src_color & 0xff00) >> 8;

					src_color = (lo << 8) | hi;
				}

				unsigned int red   = (((src_color & red_mask)   >> src_red_shift)   << dest_red_shift);
				unsigned int green = (((src_color & green_mask) >> src_green_shift) << dest_green_shift);
				unsigned int blue  = (((src_color & blue_mask)  >> src_blue_shift)  << dest_blue_shift);

				if (red > 255 || green > 255 || blue > 255)
					int j = 4;

				unsigned int color =
					red   << redshift   |
					green << greenshift |
					blue  << blueshift    ;

				if (color != color_key)
					color |= 0xFF << alphashift;

				data[i] = color;

			}

			CL_PixelBuffer px(MPHD.blockwidth, MPHD.blockheight, pitch, format, data);
	
			tiles[n] = CL_Surface(px.to_format(format), CL_Surface::flag_none);

		}
	}
	else if (MPHD.blockdepth == 24)
	{
		unsigned int color_key = get_map_colorkey();

		for (int n = 0; n < MPHD.numblockgfx; n++)
		{
			unsigned char *tile_data = &bgfx_data[n * 3 * MPHD.blockwidth * MPHD.blockheight];

			for (int i = 0; i < MPHD.blockwidth * MPHD.blockheight; i++)
			{
				unsigned int color = 
					tile_data[i*3+0] << redshift   |
					tile_data[i*3+1] << greenshift |
					tile_data[i*3+2] << blueshift    ;

				if (color != color_key)
					color |= 0xFF << alphashift;

				data[i] = color;

			}

			CL_PixelBuffer px(MPHD.blockwidth, MPHD.blockheight, pitch, format, data);
	
			tiles[n] = CL_Surface(px.to_format(format), CL_Surface::flag_none);

		}
	}
	else if (MPHD.blockdepth == 32)
	{
		unsigned int color_key = get_map_colorkey();

		unsigned int alpha_mask = 0xff << redshift | 
								  0xff << blueshift |
								  0xff << greenshift;

		for (int n = 0; n < MPHD.numblockgfx; n++)
		{
			unsigned char *tile_data = &bgfx_data[n * 4 * MPHD.blockwidth * MPHD.blockheight];

			for (int i = 0; i < MPHD.blockwidth * MPHD.blockheight; i++)
			{
				unsigned int color = 
					tile_data[i*4+0] << alphashift |
					tile_data[i*4+1] << redshift   |
					tile_data[i*4+2] << greenshift |
					tile_data[i*4+3] << blueshift    ;

				if ((color & alpha_mask) != color_key)
					data[i] = color;
				else
					data[i] = 0;

			}

			CL_PixelBuffer px(MPHD.blockwidth, MPHD.blockheight, pitch, format, data);
	
			tiles[n] = CL_Surface(px.to_format(format), CL_Surface::flag_none);

		}
	}
	else
	{
		last_error = could_not_convert_tiles;
		return false;
	
	}

	// now transfer the data
	delete [] bgfx_data;
	bgfx_data = NULL;
	

	delete [] data;

	// create the surface from the data we found in the BGFX chunk.
	//tile_pixels = CL_PixelBuffer(width, height, pitch, format, bgfx_data);

	// convert to display format
	//tile_pixels = tile_pixels.to_format(CL_Display::get_current_window()->get_back_buffer().get_format());


	//tiles = new CL_Surface(tile_pixels, CL_Surface::flag_keep_pixelbuffer);

	
	if (tiles)
	{
		

		return true;
	}
	else
	{
		last_error = could_not_convert_tiles;				
		
		return false;
	}
}

int CL_FlexibleMap::get_color_shift_from_mask(unsigned int color_mask)
{
	for (int sh = 0; sh < 32; sh += 8)
	{
		if (color_mask & (255 << sh))
		{
			return sh;
		}
	}
	
	return 0;
}


unsigned int CL_FlexibleMap::get_map_colorkey()
{

	unsigned int colorkey;

	
	colorkey = MPHD.ckeyred   << 16 |
			   MPHD.ckeygreen <<  8 |
			   MPHD.ckeyblue          ;

	return colorkey;
}

bool CL_FlexibleMap::parse_ATHR(const std::string& blockname, int size)
{
	int bytes_read = 0;

	for (int i = 0; i < 4; i++)
	{
		bytes_read += read_zero_terminated_string(author_info[i], size - bytes_read);
	}

	// make sure we didn't go over
	if (bytes_read <= size)
		return true;	
	else
	{
		last_error = bad_ATHR;

		return false;
	}

}

bool CL_FlexibleMap::parse_MPHD(const std::string& blockname, int size)
{ 

	//char mapverhigh;		/* map version number to left of . (ie X.0). */
	//char mapverlow;			/* map version number to right of . (ie 0.X). */
	//char lsb;				/* if 1, data stored LSB first, otherwise MSB first. */
	//char reserved;
	//short int mapwidth;		/* width in blocks. */
	//short int mapheight;	/* height in blocks. */
	//short int reserved1;
	//short int reserved2;
	//short int blockwidth;	/* width of a block (tile) in pixels. */
	//short int blockheight;	/* height of a block (tile) in pixels. */
	//short int blockdepth;	/* depth of a block (tile) in planes (ie. 256 colours is 8) */
	//short int blockstrsize;	/* size of a block data structure */
	//short int numblockstr;	/* Number of block structures in BKDT */
	//short int numblockgfx;	/* Number of 'blocks' in graphics (BODY) */

	MPHD.mapverhigh = map_source->read_char8();
	MPHD.mapverlow = map_source->read_char8();
	MPHD.lsb = map_source->read_char8();

	// exactly one of these should be uncommented.
	//MPHD.reserved = map_source->read_char8();
	MPHD.maptype = map_source->read_char8();


	// check to see if data is little-endian format
	if (MPHD.lsb == 1)
		map_source->set_little_endian_mode();

	MPHD.mapwidth = map_source->read_int16();
	MPHD.mapheight = map_source->read_int16();
	MPHD.reserved1 = map_source->read_int16();
	MPHD.reserved2 = map_source->read_int16();
	MPHD.blockwidth = map_source->read_int16();
	MPHD.blockheight = map_source->read_int16();
	MPHD.blockdepth = map_source->read_int16();
	MPHD.blockstrsize = map_source->read_int16();
	MPHD.numblockstr = map_source->read_int16();
	MPHD.numblockgfx = map_source->read_int16();

	map_version = MPHD.mapverhigh + MPHD.mapverlow / 10.0;

	// version 0.4
	if (size > 24)
	{
		MPHD.ckey8bit = map_source->read_char8();
		MPHD.ckeyred = map_source->read_char8();
		MPHD.ckeygreen = map_source->read_char8();
		MPHD.ckeyblue = map_source->read_char8();
	}
	else
	{
		MPHD.ckey8bit = 0;
		MPHD.ckeyblue = 0;
		MPHD.ckeygreen = 0;
		MPHD.ckeyred = 0;
	}


	
	// version 0.5
	if (size > 28) 
	{
		MPHD.blockgapx = map_source->read_int16();
		MPHD.blockgapy = map_source->read_int16();
		MPHD.blockstaggerx = map_source->read_int16();
		MPHD.blockstaggery = map_source->read_int16();
	}
	else 
	{
		MPHD.blockgapx = MPHD.blockwidth;
		MPHD.blockgapy = MPHD.blockheight;
		MPHD.blockstaggerx = 0;
		MPHD.blockstaggery = 0;
	}

	if (size > 36)
	{
		MPHD.clickmask = map_source->read_int16();
		MPHD.pillar = map_source->read_int16();
	}
	else
	{
		MPHD.clickmask = 0;
		MPHD.pillar = 0;
	}

	if (MPHD.blockstaggerx || MPHD.blockstaggery)
		use_stagger = true;

	return true;
}

bool CL_FlexibleMap::parse_EDHD(const std::string& blockname, int size)
{
	// I think we can skip reading this, because this class is not for using the editor.
	return true; 
}

bool CL_FlexibleMap::parse_CMAP(const std::string& blockname, int size)
{ 
	// here we would read in the color map for 8-bit displays.
	palette_size = size / 3;

	palette = new unsigned int[palette_size];

	for (int i = 0; i < palette_size; i++)
	{
		unsigned char red   = map_source->read_uchar8();
		unsigned char green = map_source->read_uchar8();
		unsigned char blue  = map_source->read_uchar8();

		palette[i] = (red   << 16) |
					 (green <<  8) |
					 (blue       )   ;
}

	return true; 
}


bool CL_FlexibleMap::parse_BKDT(const std::string& blockname, int size)
{ 
	// read in block data stuff.  This assumes the MPHD has been read in
	block_data = new CL_BlockData[MPHD.numblockstr];

	if (sizeof(CL_BlockData) < MPHD.blockstrsize)
	{
		// we've got a problem here.  Either the map version is newer, or
		// the map header is corrupted.  Bail out.
		last_error = CL_FlexibleMap::bad_MPHD;

		return false;
	}


	for (int i = 0; i < MPHD.numblockstr; i++)
	{
		CL_BlockData &current = block_data[i];

		//map_source->read(&current, MPHD.blockstrsize );
		current.bg_offset = map_source->read_int32();
		current.fg_offset = map_source->read_int32();
		current.fg2_offset = map_source->read_int32();
		current.fg3_offset = map_source->read_int32();

		current.user1 = map_source->read_uint32();
		current.user2 = map_source->read_uint32();
		
		current.user3 = map_source->read_uint16();
		current.user4 = map_source->read_uint16();

		current.user5 = map_source->read_char8();
		current.user6 = map_source->read_char8();
		current.user7 = map_source->read_char8();

		// if the mappy version is low, convert offsets to indices
		if (MPHD.mapverhigh == 0)
		{
			int pixel_size = get_FMP0_5_bytes_per_pixel();

			current.bg_offset /= pixel_size;
			current.fg_offset /= pixel_size;
			current.fg2_offset /= pixel_size;
			current.fg3_offset /= pixel_size;

		}


		// interpret the bit flags;
		unsigned char bit = map_source->read_uchar8();




		current.tl =  (bit & 0x01) ? 1 : 0;
		current.tr =  (bit & 0x02) ? 1 : 0;
		current.bl =  (bit & 0x04) ? 1 : 0;
		current.br =  (bit & 0x08) ? 1 : 0;

		current.trigger =		  (bit & 0x10) ? 1 : 0;
		current.u1_attach_next  = (bit & 0x20) ? 1 : 0;
		current.u2_left_pillar  = (bit & 0x40) ? 1 : 0;
		current.u3_right_pillar = (bit & 0x80) ? 1 : 0;



		//long int bgoff, fgoff;	/* offsets from start of graphic blocks */
		//long int fgoff2, fgoff3; /* more overlay blocks */
		//unsigned long int user1, user2;	/* user long data */
		//unsigned short int user3, user4;	/* user short data */
		//unsigned char user5, user6, user7;	/* user byte data */
		//unsigned char tl : 1;	/* bits for collision detection */
		//unsigned char tr : 1;
		//unsigned char bl : 1;
		//unsigned char br : 1;
		//unsigned char trigger : 1;	/* bit to trigger an event */
		//unsigned char unused1 : 1;
		//unsigned char unused2 : 1;
		//unsigned char unused3 : 1;


	}

	
	return true; 
}


bool CL_FlexibleMap::parse_ANDT(const std::string& blockname, int size)
{ 
	// read in animation data stuff.  This assumes the MPHD has been read in
	// apparently mappy has a bizarre way of storing this information.
	unsigned char *data = new unsigned char[size];

	map_source->read(data, size);

	// we need to count the animation structures present, and they are last.
	CL_AnimData *end_pt = reinterpret_cast<CL_AnimData*>(data + size);
	CL_AnimData *anim_ptr = end_pt;


	do
	{
		anim_ptr--;
		anim_data_count++;

	} while (anim_ptr->anim_type != CL_FlexibleMap::anim_end);

	// no need to store the end structure
	anim_data_count--;

	anim_data = new CL_AnimData[anim_data_count];

	// allocate memory for the actual frames
	int frame_count = (size - anim_data_count * anim_data_size) / sizeof(int);
	anim_frames = new int[frame_count];

	memcpy(anim_frames, data, sizeof(int) * frame_count);
	
	if (MPHD.mapverhigh == 0)
	{
		// these need to be divided by block sizes
		for (int i = 0; i < frame_count; i++)
			anim_frames[i] /= block_data_size;
	}


	// ok, read in the control structures.
	anim_ptr = end_pt;

	for (int i = 0; i < anim_data_count; i++)
	{
		anim_ptr--;

		anim_data[i] = *anim_ptr;

		// make sure once through animations are frozen to begin with
		if (anim_data[i].anim_type == anim_once)
			anim_data[i].anim_type = anim_onces;

		if (anim_data[i].anim_type == anim_onceh)
			anim_data[i].anim_type = anim_oncehs;

		// some of the fields need to be modified based on maptype
		if (MPHD.mapverhigh == 0)
		{
			anim_data[i].anim_current_offset += size;
			anim_data[i].anim_start_offset += size;
			anim_data[i].anim_end_offset += size;
		}
		else
		{
			// do nothing?
			/*
			anim_data[i].anim_current_offset += size;
			anim_data[i].anim_start_offset += size;
			anim_data[i].anim_end_offset += size;
			*/
		}

		if (MPHD.mapverhigh == 0)
		{
			// anim offsets need to be divided by 4 or sizeof(int)
			anim_data[i].anim_start_offset /= 4;
			anim_data[i].anim_current_offset /= 4;
			anim_data[i].anim_end_offset /= 4;
		}

		int j = 4;
	}

	return true;
}


bool CL_FlexibleMap::parse_BGFX(const std::string& blockname, int size)
{ 
	// here we need to actually read in the graphics image
	// graphics data stored here is raw?
	
	bgfx_data = new unsigned char[size];

	if (!bgfx_data)
	{
		last_error = CL_FlexibleMap::out_of_memory;
		return false;
	}

	// read the raw data
	if (map_source->read(bgfx_data, size) == size)
		return true;
	else
	{
		last_error = CL_FlexibleMap::bad_BGFX;
		return false;
	}

}

bool CL_FlexibleMap::parse_BODY_LYR(const std::string& blockname, int size)
{
	int layer_id = 0;
	bool retval = false;

	// assign appropriate ID
	if (blockname.substr(0, 3) == "LYR")
		layer_id = atoi(blockname.substr(3, -1).c_str());

	CL_Layer *current = new CL_Layer(this);

	current->layer_id = layer_id;

	current->set_display_area(
		CL_Rect(0, 0, 
		CL_Display::get_current_window()->get_width(),
		CL_Display::get_current_window()->get_height()  ) );

	// allocate memory for it
	short int *data = new short int[size / sizeof(short int)];

	if (map_source->read(data, size) != size)
	{
		last_error = CL_FlexibleMap::bad_BODY_LYR;

		delete [] data;
		delete current;

		return false;
	}



	// allocate the final storage
	current->size = MPHD.mapheight * MPHD.mapwidth;
	current->data = new short int[current->size];

	// make sure allocation was successful
	if (!current->data)
	{
		last_error = CL_FlexibleMap::out_of_memory;

		delete [] data;
		delete current;

		return false;
	}

	// well, as an undocumented feature, mappy apparently will store the maptype in a field
	// normally called "reserved", and the actual storage of map data will be arranged differently
	// depending on that field.
	
	if (MPHD.maptype == 0 || MPHD.maptype == 1)
	{
		// both of these store the tile data directly.
		delete [] current->data;
		current->data = data;

		data = NULL;

		// The oldest versions wrote out actuall byte offsets instead of indices.
		// So convert offsets into tile indices
		if (MPHD.mapverhigh == 0)
		{
			for (int i = 0; i < (signed) current->size; i++)
			{
				if (current->data[i] > 0)
					current->data[i] /= MPHD.blockstrsize;
				else if (current->data[i] < 0)
					current->data[i] /= CL_FlexibleMap::anim_data_size;
			}
		}



		retval = true;

	}
	else if (MPHD.maptype == 2)
	{
		// this seems to be the format of hexagonal and isometric maps
		int d_index = 0;
		int i = 0;

		while (i < (signed) current->size)
		{
			// we have a short indicating how many times to repeat the
			// next tile
			int repeat = data[d_index];
			d_index++;

			if (repeat > 0)
			{
				// in this case, we actually read tiles until we're done
				for (int n = 0; n < repeat; n++)
				{
					current->data[i] = data[d_index];

					d_index++;
					i++;

				}
			}
			else if (repeat < 0)
			{
				// here, we read the next tile and repeat it (-repeat) times.
				short int tile = data[d_index];

				d_index++;

				for (int n = 0; n < -repeat; n++, i++)
					current->data[i] = tile;
			}


		}
			
		retval = true;


	}
	else
	{
		last_error = CL_FlexibleMap::unsupported_map_type;
		delete [] current->data;

		retval = false;
	}

	delete [] data;

	// save the layer
	if (retval)
		layers.push_back(current);

	return retval;

}


bool CL_FlexibleMap::parse_custom_chunk(const std::string &blockname, int size)
{
	CL_FlexibleMapChunk *chunk = new CL_FlexibleMapChunk;

	chunk->name = blockname;
	chunk->size = size;

	chunk->data = new unsigned char[size];

	if (map_source->read(chunk->data, size) == size)
	{
		unknown_chunks[blockname] = chunk;

		return true;
	}
	else
	{
		delete [] chunk->data;
		delete chunk;

		return false;
	}

}

int CL_FlexibleMap::get_extra_chunk_count()
{
	return (signed) unknown_chunks.size();
}


CL_FlexibleMapChunk* CL_FlexibleMap::get_chunk(const std::string &identifier)
{
	if (unknown_chunks.find(identifier) != unknown_chunks.end())
		return unknown_chunks[identifier];
	else
		return NULL;
}

CL_FlexibleMapChunk* CL_FlexibleMap::get_chunk(int index)
{
	if (index < (signed) unknown_chunks.size() && index > 0)
	{
		std::map<std::string, CL_FlexibleMapChunk* >::iterator i = unknown_chunks.begin();

		// iterate through
		for (int j = 0; j < index; j++, i++);

		return i->second;
	}
	else
		return NULL;

}


////////////////////////////////
//  Drawing functions
bool CL_FlexibleMap::done_drawing(CL_FlexibleMapDrawParams &params)
{
	if (params.draw.y < params.start.y + params.draw_size.height)
		return false;
	else
		return true;
}


void CL_FlexibleMap::draw_block_layer(int block_layer)
{
	CL_FlexibleMapDrawParams params(get_current_layer(), block_layer);

	begin_draw_block_layer(params);


	// begin drawing loop
	while (!done_drawing(params))
	{

		draw_next_line(params);

	}

	end_draw_block_layer(params);

}

void CL_FlexibleMap::draw_next_line(CL_FlexibleMapDrawParams &params)
{

	int eff_draw_y = params.draw.y;

	// whether or not the row is staggered.
	if (params.tile.y % 2 != 0 && use_stagger)
		params.stagger_this_row = true;
	else
		params.stagger_this_row = false;

	// store drawing location
	params.last_row_start.x = params.draw.x;
	params.last_row_start.y = params.draw.y;

	params.last_row_start_tile.x = params.tile.x;
	params.last_row_start_tile.y = params.tile.y;

	if (params.stagger_this_row)
	{
		params.last_row_start.x = params.draw.x + MPHD.blockstaggerx;
		params.last_row_start.y = params.draw.y + MPHD.blockstaggery;
	}



	while (params.draw.x < params.start.x + params.draw_size.width)
	{
		int eff_draw_x = params.draw.x;

		// use_stagger is true for non-rectangular tiles
		if (params.stagger_this_row)
		{
			eff_draw_y = params.draw.y + MPHD.blockstaggery;
			eff_draw_x = params.draw.x + MPHD.blockstaggerx;
		}

		// read from the map 
		int block_index = params.layer.get_map_data(params.tile.x, params.tile.y);

		// draw the block in the correctly calculated place
		draw_block(eff_draw_x, eff_draw_y, params.layer.get_alpha(), 
					block_index, params.block_layer, params.draw_attached);

		// increment
		params.draw.x += MPHD.blockgapx;
		params.tile.x++;

	}

	// start the next row
	params.draw.x = params.start.x;

	// the gap is only added for stagger maps every other row.  The
	// stagger variable will alternate each row.
	if (params.stagger_this_row || !use_stagger)
		params.draw.y += MPHD.blockgapy;

	params.tile.x = params.start_tile.x;
	params.tile.y++;

}



void CL_FlexibleMap::begin_draw_block_layer(CL_FlexibleMapDrawParams &params)
{
	CL_Layer &layer = params.layer;

	params.stagger_this_row = false;


	// save the clipping region, and set the layer clip region
	params.old_clip_rect = CL_Display::get_cliprect();
	params.clip_rect = layer.get_display_area();

	// for debugging the clip_rect stuff.. comment out top line and add bottom
	//CL_Display::draw_rect(params.clip_rect, CL_Color::white);
	CL_Display::set_cliprect(params.clip_rect);

	params.eff_stagger.x = use_stagger ? MPHD.blockstaggerx : 0;
	params.eff_stagger.y = use_stagger ? MPHD.blockstaggery : 0;

	// pixel point at which to use for the top left of the display
	params.source.x = layer.draw_point.x - params.clip_rect.left;
	params.source.y = layer.draw_point.y - params.clip_rect.top;

	// include the choice of origin
	CL_Point origin = calc_origin(layer.origin, params.clip_rect.get_size());
	
	params.source.x -= origin.x;
	params.source.y -= origin.y;

	// tile at which to start drawing
	params.start_tile.x = (layer.draw_point.x - origin.x) / MPHD.blockgapx - params.block_border;
	params.start_tile.y = (layer.draw_point.y - origin.y) / MPHD.blockgapy - params.block_border;

	// incorporate smooth scrolling
	params.offset.x = (layer.draw_point.x - origin.x) % MPHD.blockgapx;
	params.offset.y = (layer.draw_point.y - origin.y) % MPHD.blockgapy;

	// height and width minimum to draw
	params.draw_size.width  = params.clip_rect.get_width()  
		+ (MPHD.blockgapx + params.eff_stagger.x) * params.block_border * 2 + abs(params.offset.x) * 2;

	params.draw_size.height = params.clip_rect.get_height() 
		+ (MPHD.blockgapy + params.eff_stagger.y) * params.block_border * 2 + abs(params.offset.y) * 2;

	// for non-rectangular tiles
	if (use_stagger)
	{
		params.start_tile.y = (params.source.y / (MPHD.blockgapy)) * 2 - 2;
		params.offset.y	    = (params.source.y % (MPHD.blockgapy));

	}

	if (params.block_layer > 1 && MPHD.pillar)
        params.offset.y += MPHD.blockheight * (params.block_layer - 1);

	// point at which to start drawing
  	params.start.x = -MPHD.blockgapx * params.block_border - params.offset.x + params.clip_rect.left;
	params.start.y = -MPHD.blockgapy * params.block_border - params.offset.y + params.clip_rect.top;

	// hack for strange bug where one tile too few is drawn on the top and left
	// of hexagonal maps which are wrapping.  Turn this off and look at hextst.fmp
	// to see what I mean.
	if (params.source.x < MPHD.blockgapx + MPHD.blockstaggerx)
	{
		params.start.x -= MPHD.blockgapx;
		params.start_tile.x -= 1;
	}
	if (params.source.y < MPHD.blockgapy + MPHD.blockstaggery)
	{
		params.start.y -= MPHD.blockgapy;
		params.start_tile.y -= 1;
	}

	// iteration variables
	params.draw.x = params.start.x;
	params.draw.y = params.start.y;

	params.tile.x = params.start_tile.x;
	params.tile.y = params.start_tile.y;

	// whether or not to draw isometric pillars
	params.draw_attached = false;

	if (MPHD.pillar && params.block_layer == 3)
		params.draw_attached = true;

	if (MPHD.pillar)
	{
		// extend drawing past bottom to make sure that
		// tall pillars that are below the map show up
		params.draw_size.height += MPHD.blockgapy * 4;
	}

	// draw all tiles, for debugging purposes
	//for(int i = 0; i < MPHD.numblockgfx; i++)
	//	tiles[i].draw(MPHD.blockwidth * (i % 10), MPHD.blockwidth * (i / 10));
	//
	//return;

	src_rect = get_src_rect(0);



}


void CL_FlexibleMap::end_draw_block_layer(CL_FlexibleMapDrawParams &params)
{

	// restore the clipping area
	CL_Display::set_cliprect(params.old_clip_rect);


}


void CL_FlexibleMap::draw_block(int draw_x, int draw_y, int alpha, int block_index, 
						int block_layer, bool do_attached, bool is_attached)
{

	int display_index = 0;
	bool has_attached = false;
	bool left_pillar = false, 
		 right_pillar = false;

	// look for animations first
	if (block_index < 0)
	{
		const CL_AnimData *anim = get_anim(block_index);

		if (anim)
		{
			block_index = anim_frames[anim->anim_current_offset];
		}
	}

	// now look for static blocks
	if (block_index > 0)
	{
		const CL_BlockData *tile = get_block(block_index);

		if (MPHD.pillar)
		{
			if (tile->u1_attach_next)				has_attached = true;
			if (tile->u2_left_pillar)				left_pillar = true;
			if (tile->u3_right_pillar)				right_pillar = true;

			// if both are true, it acts as if both are false.
			if (left_pillar && right_pillar)		left_pillar = right_pillar = false;

			// we draw the whole tile if it's the very bottom one.
			if (block_layer == 0 && !is_attached)	left_pillar = right_pillar = false;

		}
		// make sure we have a structure
		if (tile)
		{
			switch(block_layer)
			{
				case 0:		display_index = tile->bg_offset;		break;
				case 1:		display_index = tile->fg_offset;		break;
				case 2:		display_index = tile->fg2_offset;		break;
				case 3:		display_index = tile->fg3_offset;		break;
					
				default:	int j = 4;
			}					
		
		}


	}
	


	if (display_index)  // zero tile is transparent
	{
		// calculate up the source and destination rects
		CL_Rect eff_src_rect = src_rect;
		CL_Rect dest_rect  (draw_x, 
							draw_y, 
							draw_x + MPHD.blockwidth, 
							draw_y + MPHD.blockheight);

		// they're different if we're doing isometric pillars
		if (left_pillar)
		{
			dest_rect = CL_Rect(draw_x,
								draw_y,
								draw_x + MPHD.blockwidth / 2,
								draw_y + MPHD.blockheight);

			eff_src_rect = CL_Rect(0, 0, MPHD.blockwidth / 2, MPHD.blockheight);
		}
		else if (right_pillar)
		{
			dest_rect = CL_Rect(draw_x + MPHD.blockwidth / 2,
								draw_y,
								draw_x + MPHD.blockwidth,
								draw_y + MPHD.blockheight);

			eff_src_rect = CL_Rect(MPHD.blockwidth / 2, 0,
								   MPHD.blockwidth,     MPHD.blockheight);
		}
		
		// give the appropriate alpha blending value
		tiles[display_index].set_alpha(alpha / 255.0f);

		// actually do the drawing
		tiles[display_index].draw(eff_src_rect, dest_rect);

	}

	// check for attached blocks
 	if (has_attached && do_attached && block_layer == 3)
	{ 
		draw_block(draw_x, draw_y - MPHD.blockheight*1, alpha, block_index + 1, 0, do_attached, true);
		draw_block(draw_x, draw_y - MPHD.blockheight*2, alpha, block_index + 1, 1, do_attached, true);
		draw_block(draw_x, draw_y - MPHD.blockheight*3, alpha, block_index + 1, 2, do_attached, true);
		draw_block(draw_x, draw_y - MPHD.blockheight*4, alpha, block_index + 1, 3, do_attached, true);

	}
}

short int CL_FlexibleMap::get_map_data(int x, int y, int layer)
{
	if (layer < 0 || layer >= get_layer_count())
		layer = current_layer;

	CL_Layer &current = get_current_layer();

	return current.get_map_data(x, y);
}


CL_Rect CL_FlexibleMap::get_src_rect(int tile)
{
	return CL_Rect(0, 0, MPHD.blockwidth, MPHD.blockheight);
}

void CL_FlexibleMap::set_frame_independent_animation(int increment_time, bool use_fia)
{
	frame_rate_independent_anims = use_fia;

	if (frame_rate_independent_anims)
		frame_increment_time = increment_time;
	else
		frame_increment_time = 0;
}
		
int CL_FlexibleMap::get_frame_increment_time()
{
	if (frame_rate_independent_anims)
		return frame_increment_time;

	else
		return 0;


}

void CL_FlexibleMap::start_animate_once(int anim_index, bool start_now)
{
	std::vector<int> anim_indices;

	// determine which animations to take care of
	if (anim_index != 0)
	{
		if (anim_index < 0)  anim_index = -anim_index;

		anim_indices.push_back(anim_index - 1);
	}
	else
	{
		for (int i = 0; i < anim_data_count; i++)
			anim_indices.push_back(i);
	}

	// cycle through the tiles to update
	for (int i = 0; i < (signed)anim_indices.size(); i++)
	{
		anim_index = anim_indices[i];

		CL_AnimData &anim = anim_data[anim_index];

		bool update = false;
		bool allow_skip = true;

		// check if we need to reset this one
		switch(anim.anim_type)
		{
			case anim_once:
			case anim_onces:

				anim.anim_type = anim_once;
				update = true;

				break;

			case anim_onceh:
			case anim_oncehs:

				anim.anim_type = anim_onceh;
				update = true;

				// don't skip the first tile if we are starting this animation over
				if (anim.anim_current_offset != anim.anim_start_offset)
					allow_skip = false;

				break;
			
		}


		if (update)
		{
			// reset the timer
			anim.anim_count = anim.anim_delay;

			// start the animation over
			anim.anim_current_offset = anim.anim_start_offset;

			if (start_now && allow_skip)
				anim.anim_current_offset++;

		}

	}

}


void CL_FlexibleMap::update_anim()
{
	unsigned int delta_t;
	unsigned int current_t = CL_System::get_time();
	int decrement_size = 1;

	delta_t = current_t - last_anim_update;
	
	if (get_frame_independent_animation())
	{
		// definitely want integer arithmetic on this one
		decrement_size = delta_t / get_frame_increment_time();

		// ok, we're not there yet, so don't do any updating.
		if (delta_t == 0)
			return;
		
		// now only update to the time when the last decrement should have occured
		last_anim_update += decrement_size * get_frame_increment_time();
	}

	
	for (int i = 0; i < anim_data_count; i++)
	{
		anim_data[i].anim_count -= decrement_size;


		if (anim_data[i].anim_count <= 0)
		{
			// reset the counter
			anim_data[i].anim_count = anim_data[i].anim_delay;

			// make sure there is more than one frame to the animation before we start changing stuff
			if (anim_data[i].anim_start_offset == anim_data[i].anim_end_offset - 1)
				continue;

			switch(anim_data[i].anim_type)
			{
				case anim_loopf:
					// increment forward
					anim_data[i].anim_current_offset++;

					if (anim_data[i].anim_current_offset >= anim_data[i].anim_end_offset)
					{
						anim_data[i].anim_current_offset = anim_data[i].anim_start_offset;
					}

					break;

				case anim_loopr:
					// increment backwards
					anim_data[i].anim_current_offset--;

					if (anim_data[i].anim_current_offset < anim_data[i].anim_start_offset)
					{
						anim_data[i].anim_current_offset = anim_data[i].anim_end_offset - 1;
					}

					break;

				case anim_ppff:
				case anim_ppfr:
					// increment forward
					anim_data[i].anim_current_offset++;

					if (anim_data[i].anim_current_offset >= anim_data[i].anim_end_offset)
					{
						// switch type to move backwards
						anim_data[i].anim_type = anim_pprf;
						anim_data[i].anim_current_offset = anim_data[i].anim_end_offset - 2;
					}

					break;
				case anim_pprr:
				case anim_pprf:
					// increment backwards
					anim_data[i].anim_current_offset--;

					if (anim_data[i].anim_current_offset < anim_data[i].anim_start_offset)
					{
						// switch type to move forwards
						anim_data[i].anim_type = anim_ppfr;
						anim_data[i].anim_current_offset = anim_data[i].anim_start_offset + 1;
					}

					break;
				case anim_once:
					// increment forward
					anim_data[i].anim_current_offset++;

					if (anim_data[i].anim_current_offset >= anim_data[i].anim_end_offset)
					{
						anim_data[i].anim_type = anim_onces;
						anim_data[i].anim_current_offset = anim_data[i].anim_start_offset;
					}

					break;

				case anim_onceh:
					// increment forward
					anim_data[i].anim_current_offset++;

					if (anim_data[i].anim_current_offset >= anim_data[i].anim_end_offset)
					{
						anim_data[i].anim_type = anim_oncehs;
						anim_data[i].anim_current_offset = anim_data[i].anim_end_offset - 1;
					}

					break;

				case anim_onces:
				case anim_oncehs:
					// do nothing here
					break;

				case anim_end:
				case anim_none:
				default:

					// do nothing
					break;

			}
		}
	}

}


CL_FlexibleMap::CL_BlockData *CL_FlexibleMap::get_block(short index) 
{

	if (index > 0)
		return &block_data[index];
	else if (index < 0)
		return NULL;
	else
		return NULL;
}


CL_FlexibleMap::CL_AnimData *CL_FlexibleMap::get_anim(short index)
{ 

	if (index < 0)
		return &anim_data[(-index) - 1];
	else
		return NULL;

}



bool CL_FlexibleMap::set_current_layer(int layer)
{
	if (layer < 0 || layer >= get_layer_count())
		return false;

	current_layer = layer;

	return true;
}



int CL_FlexibleMap::get_layer_count() const
{
	return (signed)layers.size(); 
}


void CL_FlexibleMap::set_display_area(const CL_Rect &area)
{
	for (int i = 0; i < (signed) layers.size(); i++)
		layers[i]->set_display_area(area);
}

const CL_Rect &CL_FlexibleMap::get_display_area(int layer_id)
{
	if (layer_id < 0 || layer_id >= (signed) layers.size())
		layer_id = current_layer;

	return layers[current_layer]->get_display_area();
}



void CL_FlexibleMap::set_anchor_point(const CL_Origin &origin)
{
	for (int i = 0; i < (signed) layers.size(); i++)
		layers[i]->set_anchor_point(origin);
}

void CL_FlexibleMap::get_anchor_point(CL_Origin &origin, int layer_id)
{
	if (layer_id < 0 || layer_id >= (signed) layers.size())
		layer_id = current_layer;

	layers[current_layer]->set_anchor_point(origin);
}



///////////////////////////////////////////////////////////////////////////
//
//  CL_FlexibleMap::CL_Layer functions
//
CL_FlexibleMap::CL_Layer::CL_Layer(CL_FlexibleMap *mappy_owner) :
	owner(*mappy_owner), size(0), data(NULL), draw_point(0, 0),
	layer_id(-1), alpha(255), default_tile(0), border_style(border_none),
	origin(origin_top_left)
{

}

CL_FlexibleMap::CL_Layer::CL_Layer(const CL_Layer &copy) : owner(copy.owner)
{
	size = copy.size;
	draw_point = copy.draw_point;
	layer_id = copy.layer_id;
	alpha = copy.alpha;
	default_tile = copy.default_tile;
	border_style = copy.border_style;
	origin = copy.origin;
	display_area = copy.display_area;

	data = new short[size];

	memcpy(data, copy.data, size * sizeof(short));
}

CL_FlexibleMap::CL_Layer::~CL_Layer() 
{ 
	delete [] data; 
	data = NULL; 
}


void CL_FlexibleMap::CL_Layer::set_draw_point(int x, int y)
{
	draw_point.x = x;
	draw_point.y = y;
}

void CL_FlexibleMap::CL_Layer::set_draw_point(const CL_Point &pt)
{
	draw_point = pt;
}

void CL_FlexibleMap::CL_Layer::set_alpha(int alp)
{
	if (alp < 0)
		alp = 0;
	else if (alp > 255)
		alp = 255;

	alpha = alp;
}

void CL_FlexibleMap::CL_Layer::set_border_style(CL_FlexibleMap::BorderStyle value)
{
	if (value >= 0 && value < 4)
		border_style = value;
}

short int CL_FlexibleMap::CL_Layer::get_map_data(int x, int y)
{
	int width = owner.MPHD.mapwidth;
	

	if (x < 0 || y < 0 || x >= owner.MPHD.mapwidth || y >= owner.MPHD.mapheight)
	{
		// now we need to see how the layer should handle it
		switch(get_border_style())
		{
			case border_none:   
				return NULL;

			case border_wrap:
				{
					while (x < 0)						x += owner.MPHD.mapwidth;
					while (x >= owner.MPHD.mapwidth)	x -= owner.MPHD.mapwidth;
					while (y < 0)						y += owner.MPHD.mapheight;
					while (y >= owner.MPHD.mapheight)	y -= owner.MPHD.mapheight;
				}

				break;

			case border_default_tile:
				return default_tile;

		}


	}

	return data[y * width + x];
}

void CL_FlexibleMap::CL_Layer::set_map_data(int x, int y, int value)
{
	int width = owner.MPHD.mapwidth;

	data[y * width + x] = value;
}

void CL_FlexibleMap::CL_Layer::set_display_area(const CL_Rect &area)
{
	display_area = area;
}


void CL_FlexibleMap::CL_Layer::set_anchor_point(const CL_Origin &origin)
{
	this->origin = origin;
}
