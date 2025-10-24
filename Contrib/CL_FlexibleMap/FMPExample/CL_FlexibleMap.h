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



#include <string>

#include <clanlib/core.h>
#include <clanlib/display.h>
#include <clanlib/gl.h>

#include <vector>

struct CL_FlexibleMapDrawParams;

//: Structure for unknown chunks of the file.
struct CL_FlexibleMapChunk
{
	//! Construction:
	public:
		CL_FlexibleMapChunk() : data(NULL), size(0) {}
		~CL_FlexibleMapChunk() { free(); }

		void free() { delete [] data; data = NULL; size = 0; }

	//! Implementation:
	public:

		//: Four-byte chunk identifier
		std::string name;

		//: Indicates the number of bytes stored in data
		int size;

		//: Pointer to the actual data.  Freed when the structure is deleted.
		unsigned char *data;

};


//: Class for loading and displaying a .FMP file
//- <p>This class is designed to load in files produced by the program Mappy,
//- It will automatically generate CL_Surface data for the tile graphics data
//- stored in the FMP file.  It supports non-square tiles.  So far it works with
//- all the sample maps that come with Mappy.</p>
class CL_FlexibleMap
{
	//! Enums:
	public:

		//: Error enumeration to give idea of what the last error was.
		enum CL_FlexibleMapError
		{
			ok			= 0,
			notfmap		= 1,

			bad_ATHR,
			bad_MPHD,
			bad_EDHD,
			bad_CMAP,
			bad_BKDT,
			bad_ANDT,
			bad_BGFX,
			bad_BODY_LYR,

			out_of_memory,
			could_not_convert_tiles,
			unsupported_map_type,


		};

		//: Enum for layers indicating how tiles outside the map rectangle should be handled.
		enum BorderStyle
		{
			border_none = 0,
			border_wrap,
			border_default_tile,
		};

		//: Enum for the various animation types that can be selected.
		enum AnimType
		{
			anim_end    = -1,
			anim_none   = 0,
			anim_loopf  = 1,
			anim_loopr  = 2,
			anim_once   = 3,
			anim_onceh  = 4,
			anim_ppff   = 5,
			anim_pprr   = 6,
			anim_pprf   = 7,
			anim_ppfr   = 8,
			anim_onces  = 9,
			anim_oncehs = 10,
		};


	//! Construction:
	public:
		//: Constructs a single Mappy reader object
		CL_FlexibleMap(void);
		CL_FlexibleMap(const CL_FlexibleMap& copy);

		~CL_FlexibleMap(void);

		//: Frees all memory and erases everything that has to do with the current map.
		void clear();

	//! Implementation:
	public:

		//: Class for storing information about a layer
		class CL_Layer
		{
			public:
				CL_Layer(CL_FlexibleMap *mappy_owner); 
				CL_Layer(const CL_Layer &copy);

				~CL_Layer();



				//: Comparison operator for ordering layers.  Compares layer_id data.
				static compare(const CL_Layer *left, const CL_Layer *right) { return left->layer_id < right->layer_id; }

				//: Sets the drawing point for this layer
				void set_draw_point(int x, int y);
				void set_draw_point(const CL_Point& pt);

				//: Fills the passed parameters with the current draw point.
				void get_draw_point(/*out*/ int &x, /*out*/ int &y) const { x = draw_point.x; y = draw_point.y; }
				void get_draw_point(/*out*/ CL_Point &pt) const { pt = draw_point; }


				//: Sets transparency for the layer
				void set_alpha(int alp);

				//: Returns the overall transparency for this layer.
				int get_alpha() const { return alpha; }


				//: Indicates how we handle tiles that are picked outside the rectangle making up the map
				BorderStyle get_border_style() { return border_style; }

				//: Sets how we handle tiles that are picked outside the rectangle making up the map
				void set_border_style(BorderStyle value); 

				//: Sets the default tile for when border_style == border_default_tile
				void set_default_tile(short tile) { default_tile = tile; }

				//: returns map data at the specified point
				short int get_map_data(int x, int y);
				
				//: sets map data at a specified point
				void set_map_data(int x, int y, int value);

				//: Sets the region of the window in which things are actually drawn for this layer.
				//- It's not recommended to change this for each layer individually, unless you really
				//- know what you're doing.
				void set_display_area(const CL_Rect &area);

				//: Returns the region of the window in which things are actually drawn for this layer.
				const CL_Rect &get_display_area() { return display_area; }

				//: Sets the point within the display area that the drawpoint is interpreted to be
				//- <p>This function will set the origin draw point for this layer within the 
				//- display area.  That is, if you call set_anchor_point(CL_Origin::origin_center), then
				//- call set_draw_point(0, 0), the pixel point (0, 0) will be drawn in the center
				//- of the area the map is drawn in.  This can be set on each layer
				//- individually, but you probably want to use CL_FlexibleMap::set_anchor_point.
				void set_anchor_point(const CL_Origin &origin);

				//: Returns the point within the display area that the drawpoint is interpreted to be
				void get_anchor_point(CL_Origin &origin) { origin = this->origin; }

			private:

				friend class CL_FlexibleMap;
				CL_FlexibleMap &owner;

				//: Size of map in tiles
				unsigned int size;

				//: Data containing indices into BlockData (positive) or AnimData (negative)
				short int *data;

				//: ID of this layer in the FMP
				int layer_id;

				//: Position to draw the layer
				CL_Point draw_point;
				
				//: Alpha blending of the layer.
				int alpha;

				//: Handling requests outside of the map
				BorderStyle border_style;

				//: The block structure (positive) or animation structure(negative) for drawing outside of the map
				short int default_tile; 

				//: The display area in the window in which this layer's contents will be drawn.
				CL_Rect	display_area;

				CL_Origin origin;

		};
	
		//:  Structure for data blocks
		class CL_BlockData 
		{							
			public:

				//: Offsets from start of graphic blocks for the pixels that are actually drawn
				long int bg_offset,					
						 fg_offset,
						 fg2_offset, 
						 fg3_offset;
				
				//: 32-bit user data
				unsigned long int user1, user2;			

				//: 16-bit user data
				unsigned short int user3, user4;

				//: 8-bit user data
				unsigned char user5, user6, user7;

				//: bits for collision detection
				unsigned char tl;				
				unsigned char tr;
				unsigned char bl;
				unsigned char br;

				//: User bit
				//- Called trigger (or bg transparency) in mappy, but it's not used internally by this class
				//- So it's really a user bit
				unsigned char trigger;			
				
				//: More user bits
				//- these are often called unused1,2,3 in the mappy source.  They are user bits, unless
				//- you are working with an isometric map with pillars.  Then they are used to indicate how
				//- pillars should be displayed.
				unsigned char u1_attach_next;				// Attach next
				unsigned char u2_left_pillar;				// left pillar
				unsigned char u3_right_pillar;				// right pillar

		};

		//: Structure for animation blocks
		class CL_AnimData			
		{ 
			// unfortunately, mappy doesn't output the size of these structures used, as it does
			// for the BLKSTR or CL_BlockData structures, so for 
			// now I am using a hard-coded 16 bytes in the variable anim_data_size to figure out 
			// how many structures to read in.
			public:
				signed char anim_type;			/* Type of anim, AN_? */
				signed char anim_delay;			/* Frames to go before next frame */
				signed char anim_count;			/* Counter, decs each frame, till 0, then resets to andelay */
				signed char user;				/* User info */
				long int anim_current_offset;	/* Points to current offset in list */
				long int anim_start_offset;		/* Points to start of blkstr offsets list, AFTER ref. blkstr offset */
				long int anim_end_offset;		/* Points to end of blkstr offsets list */
			


		};

		
		//: Size of block data structure used in Mappy
		static const int block_data_size = 32; //sizeof(CL_BlockData);

		//: Size of animation structure used in Mappy
		static const int anim_data_size = 16;  //sizeof(CL_AnimData);


	//! Operations:
	public:
		//: Begins the drawing loop
		void begin_draw_block_layer(CL_FlexibleMapDrawParams &params);

		//: Draws a single horizontal line of tiles
		void draw_next_line(CL_FlexibleMapDrawParams &params);

		//: Returns true when all lines have been drawn
		bool done_drawing(CL_FlexibleMapDrawParams &params);

		//; Ends drawing
		void end_draw_block_layer(CL_FlexibleMapDrawParams &params);



		//: Draws the background layer
		//- <p>Tiles have four layers.  This function will draw the very bottom
		//- one for all the visible tiles.  There are multiple of these functions
		//- in order to allow drawing of sprites between background and foreground.</p>
		void draw_background () { draw_block_layer(0); }

		//: Draws the foreground layer
		//- <p>Tiles have four layers.  This function will draw the second
		//- one for all the visible tiles.</p>
		void draw_foreground () { draw_block_layer(1); }

		//: Draws the foreground layer
		//- <p>Tiles have four layers.  This function will draw the third
		//- one for all the visible tiles.</p>
		void draw_foreground2() { draw_block_layer(2); }

		//: Draws the foreground layer
		//- <p>Tiles have four layers.  This function will draw the fourth
		//- one for all the visible tiles.</p>
		void draw_foreground3() { draw_block_layer(3); }

		//: Draws a given block layer on the screen
		//- An alternative to the draw_background(), draw_foreground(), etc. functions,
		//- which can be used in a loop.
		void draw_block_layer(int block_layer);

		//: Draws a single block, at the specified screen point.
		//- param draw_x: Back buffer x-position to draw at
		//- param draw_y: Back buffer y-position to draw at
		//- param alpha: Alpha blending value to use, in the range of (0-255)
		//- param block_index: Which block structure (positive) or animation structure (negative) to draw.
		//- param block_layer: 0-3 are valid inputs only.  Selects background(0) or one of the three foreground layers
		//- param do_attached: For pillars, if we are to draw blocks attached above the pillar (only for block_layer == 3)
		//- param is_attached: Used internally for recursive calls to the function.  Leave as the default.
		void draw_block(int draw_x, int draw_y, int alpha, int block_index, 
					int block_layer, bool do_attached, bool is_attached = false);


		//: Updates all the animation structures
		void update_anim();

		//: Load the FMP data
		//- Returns true if loading of the map was successful.  Otherwise, check
		//- get_last_error().
		//- param filename: Relative path and file name of FMP file to load
		bool load(const std::string& filename);

	//! Attributes:
	public:
		//: If the load method returns false, this will give a basic description of the error.
		CL_FlexibleMapError get_last_error() { return last_error; }

		//: Sets the current layer for drawing
		//- Returns false if failed (ie. parameter is >= get_layer_count())
		bool set_current_layer(int layer);

		//: Returns the index of the current layer 
		int get_current_layer_index() const { return current_layer; }


		//: Returns a reference to the current layer
		//- If you are storing this in a variable, make sure it is a reference or pointer
		//- variable to avoid invoking a costly copy constructor.
		CL_Layer &get_current_layer() { return *layers[current_layer]; }

		
		//: Returns the total number of layers in the map
		int get_layer_count() const;


		//: Returns the tile indicated at the position on the map
		//- This function will return the actual map data.  If it is positive or zero, it indicates
		//- an index in the CL_BlockData array.  If it's negative, it's an index into the CL_AnimData array.
		//- param x: x position in tiles on the map (between 0 and map_width - 1)
		//- param y: y position in tiles on the map (between 0 and map_height - 1)
		//- param layer: a shortcut to using layer pointers.  The default value will look up the tile on the current layer.
		short int get_map_data(int x, int y, int layer = -1);


		//: Sets frame-rate independent animations
		//- This function will set a flag indicating to the map to use frame-rate independent tile
		//- animation.  
		//- param increment_time: The number of milliseconds to pass for each value of anim_delay that is set in Mappy.
		//- param use_fia: flag to indicate whether to use frame-rate independent animation or not
		void set_frame_independent_animation(int increment_time, bool use_fia = true);

		//: Returns the characteristic time length for frame-rate independent animations.
		//- Returns zero if frame-rate independent animations are off.  Otherwise, returns the number
		//- of milliseconds between each decrement of the anim_delay values.
		int get_frame_increment_time();

		//: Returns the flag indicating whether or not frame-rate independent animation is in use.
		bool get_frame_independent_animation() { return frame_rate_independent_anims; }

		//: Start up animate once tiles
		//- Starts up tiles which run their animation once.
		//- param anim_index: The index (either positive or negative) of the animation structure to run.  The default value of zero will do all once run tiles.
		//- param start_now: Indicates whether to immediately change the visible frame to the next one in sequence (true), or wait for the count to decrease (false).
		void start_animate_once(int anim_index = 0, bool start_now = true);

		//: Returns a CL_BlockData structure from the index given.
		//- param index: A non-negative number indicating the CL_BlockData structure to return.
		//- The zero tile is always transparent.
		CL_BlockData *get_block(short index);

		//: Returns a CL_AnimData structure from the index given.
		//- param index: A negative number indicating the CL_BlockData structure to return.
		CL_AnimData *get_anim(short index);


		//: Gets the color key that is provided by the map.
		//- On loading the map, colors matching the color key are automatically replaced with
		//- a color with zero alpha.
		unsigned int get_map_colorkey();


		//: Returns the source rectangle for drawing a particular tile.
		//- This should return (0, 0, MPHD.blockwidth, MPHD.blockheight)
		CL_Rect get_src_rect(int tile);


		//: Sets the region of the window in which things are actually drawn for all layers.
		void set_display_area(const CL_Rect &area);

		
		//: Returns the region of the window in which things are actually drawn for this layer.
		//- param layer_id: a shortcut to using layer pointers.  The default value will return the value for the current layer.
		const CL_Rect &get_display_area(int layer_id = -1);


		//: Sets the point within the display area that the drawpoint is interpreted to be
		//- <p>This function will set the origin draw point for all layers within the 
		//- display area.  That is, if you call set_anchor_point(CL_Origin::origin_center), then
		//- call set_draw_point(0, 0), the pixel point (0, 0) will be drawn in the center
		//- of the area the map is drawn in.  
		void set_anchor_point(const CL_Origin &origin);

		//: Returns the point within the display area that the drawpoint is interpreted to be
		void get_anchor_point(CL_Origin &origin, int layer_id = -1);


		//: Returns the number of extra chunks in the file, that load didn't parse.
		int get_extra_chunk_count();

		//: Returns the specified chunk from Mappy
		//- param identifier: String identifing which chunk to get (prefered access method)
		//- param index: Integer from 0 to (get_extra_chunk_count() - 1) indexing which chunk to return.  Good for enumeration, but chunks will not be in a specified order in a given file.
		CL_FlexibleMapChunk* get_chunk(const std::string &identifier);
		CL_FlexibleMapChunk* get_chunk(int index);


	private:


		CL_InputSource *map_source;			// Source of map data
		CL_FlexibleMapError last_error;		// record of the last error 

		CL_Surface *tiles;					// surface of image data for drawing tiles.

		// actually does the reading, once the input source is selected
		bool _load();

		//: Returns how many bits to shift for a particular mask (888 type colors only)
		//- This is included to help endian compatibility, but it needs to be tested
		//- in a big-endian environment.
		static int get_color_shift_from_mask(unsigned int color_mask);

		//: Converts the raw image data in BGFX to a CL_Surface
		bool load_tiles();

		//: Reads a specified number of chars (usually 4)
		void read_chars(std::string& dest, int count);

		//: Reads a null terminated string.  returns number of characters read.
		int read_zero_terminated_string(std::string &dest, int maxlen);

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

		bool parse_ATHR(const std::string& blockname, int size);
		bool parse_MPHD(const std::string& blockname, int size);
		bool parse_EDHD(const std::string& blockname, int size);
		bool parse_CMAP(const std::string& blockname, int size);
		bool parse_BKDT(const std::string& blockname, int size);
		bool parse_ANDT(const std::string& blockname, int size);
		bool parse_BGFX(const std::string& blockname, int size);
		bool parse_BODY_LYR(const std::string& blockname, int size);
		bool parse_custom_chunk(const std::string& blockname, int size);


		///////////////////////////////////////		
		//: ATHR data
		std::string			author_info[4];


		///////////////////////////////////////
		// MPHD data - This struct is taken directly from the mappy source code,
		//             which is copyright  Robin Burrows 1997-2005
		//
		struct {			/* Map header structure */
			/* char M,P,H,D;		4 byte chunk identification. */
			/* long int mphdsize;	size of map header. */
			char mapverhigh;		/* map version number to left of . (ie X.0). */
			char mapverlow;			/* map version number to right of . (ie 0.X). */
			char lsb;				/* if 1, data stored LSB first, otherwise MSB first. */

			// documentation calls this reserved.. however a look at the playback
			// library suggests this indicates 2D, Hex, or Isometric.
			//char reserved;
			char maptype;

			short int mapwidth;		/* width in blocks. */
			short int mapheight;	/* height in blocks. */
			short int reserved1;
			short int reserved2;
			short int blockwidth;	/* width of a block (tile) in pixels. */
			short int blockheight;	/* height of a block (tile) in pixels. */
			short int blockdepth;	/* depth of a block (tile) in planes (ie. 256 colours is 8) */
			short int blockstrsize;	/* size of a block data structure */
			short int numblockstr;	/* Number of block structures in BKDT */
			short int numblockgfx;	/* Number of 'blocks' in graphics (BODY) */

			// added FMP0.4
			unsigned char ckey8bit, ckeyred, ckeygreen, ckeyblue; /* colour key values added FMP0.4*/

			// added FMP0.5
			short int blockgapx;
			short int blockgapy;
			short int blockstaggerx;
			short int blockstaggery;

			short int clickmask;
			short int pillar;

		} MPHD;

		//: Indicates whether or not to pay attention to the blockstaggerx and blockstaggery fields
		bool use_stagger;

		//: Version number of map loaded in
		double map_version;

		//: Returns the number of bytes per tile stored in FMP version 0.5
		//- FMP 0.5, the default file format produced by Mappy, uses units of byte offsets in everything
		//- rather than storing structure indices.
		int get_FMP0_5_bytes_per_pixel();


		///////////////////////////////////////
		// BKDT data

		//: Stores all the BlockData structures for drawing on the screen.
		//- To get how many total there are, see MPHD.numblockstr
		CL_BlockData		*block_data;


		///////////////////////////////////////
		// ANDT data
		CL_AnimData			*anim_data;
		int					anim_data_count;
		int					*anim_frames;

		unsigned int		last_anim_update;
		bool				frame_rate_independent_anims;
		unsigned int		frame_increment_time;


		///////////////////////////////////////
		// CMAP data
		unsigned int		*palette;
		int					palette_size;


		///////////////////////////////////////
		// BGFX data
		unsigned char 		*bgfx_data;

		
		///////////////////////////////////////
		// BODY, LYR? data
		std::vector<CL_Layer*>	layers;

		int						current_layer;

		CL_Rect					src_rect;	// source rect for drawing.. hardly ever changes


		///////////////////////////////////////
		// Other data
		std::map<std::string, CL_FlexibleMapChunk*> 		unknown_chunks;

};



//: Structure passed around in drawing the map.
struct CL_FlexibleMapDrawParams
{
	//! Construction:
	public:
		CL_FlexibleMapDrawParams(CL_FlexibleMap::CL_Layer &source_layer, int blocklayer) 
			: layer(source_layer), block_border(1), block_layer(blocklayer) {}
		~CL_FlexibleMapDrawParams() {}

	//! Implementation
	public:

		CL_FlexibleMap::CL_Layer &layer;

		//: Which block layer to draw
		int block_layer;

		//: Blocks to draw outside the clipping rectangle to insure smooth scrolling
		int block_border;

		//: Indicates whether or not the row drawn was staggered
		bool stagger_this_row;

		//: Number of pixels to stagger drawing
		CL_Point eff_stagger;

		//: pixel point on the layer at which to use for the top left of the display
		CL_Point source;

		//: tile at which to start drawing
		CL_Point start_tile;

		//: pixel point in window at which to start drawing
  		CL_Point start;

		//: pixel offset to incorporate smooth scrolling
		CL_Point offset;

		//: height and width minimum to draw
		CL_Size draw_size;


		//: Draw point iterated through
		CL_Point draw;

		//: tiles iterated through
		CL_Point tile;

		//: whether or not to draw isometric pillars
		bool draw_attached;

		//: The on-screen point at which drawing of the last row started
		CL_Point last_row_start;

		//; The point in tiles on the map at which drawing of the last row started
		CL_Point last_row_start_tile;



	private:
		//: The old saved clipping region
		CL_Rect old_clip_rect;

		//: The clip region being used.
		CL_Rect clip_rect;


		friend class CL_FlexibleMap;

};
