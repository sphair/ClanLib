        Name: CL_FlexibleMap
      Status: Windows(Y)
  Maintainer: Erik Ylvisaker   ylvisaker@ucdavis.edu
     Summary: Class which loads and displays FMP maps.
     License: See the License included with ClanLib
      
Features:

*	Reads FMP 0.5

*	Reads FMP 1.0 (needs more testing)

*	Rectangular and non-rectangular tiles implemented

*	Pillars for isometric maps implemented, including left and right pillar 
	flags, as well as continuation of pillars by chaining block structures.
	
*	Arbitrary sized maps

*	Individual Block layers (4 each, as in Mappy)

*	Each block layer is drawn with a separate function call, so sprites can
	be drawn in between background and foreground tiles.
	
*	Map layers, up to 8, as in Mappy

*	All storage graphics formats supported - 8-bit indexed, 15-bit, 16-bit,
	24-bit, 32-bit with alpha channel. 15 and 16 bit graphics may need more 
	testing.
	
*	Pixel based transparency supported, either through 32-bit alpha channel, 
	24-bit colorkey, or 8-bit transparent color as specified in FMP file
	
*	Looping, pingpong, and animate once tile animations implemented

*	Boundary conditions can be specified as either wrapping, or repetition 
	of a default tile (or nothing), as a per-layer setting.
	
*	Overall alpha transperency setting on each layer.

*   Clipped drawing supported, for the map to operate in a specific region 
	of your window.
	
*	Drawing position can be specified for individual layers, making parallax
	scrolling easy.
	
*	Binary access to chunks in FMP file which parser doesn't understand.

*	Line-by-line drawing for interleaving sprites on Isometric maps with pillars. (needs testing)

*	Frame-rate independent animation support.

Needed, if there is interest:

*	???? Loading from non-file sources, such as resources and zip
*	Support for objects from MappyWin32 Pro

Known Bugs:

none?  please send bug reports to the clanlib user mailing list or to my email address ylvisaker@ucdavis.edu

