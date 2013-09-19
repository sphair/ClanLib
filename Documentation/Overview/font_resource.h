
/*! \page FontResource Font Resource

Resource options

These are constructed from a resource file, using <font name="ClanFont">. Inside the <font> tag you need a <bitmap> tag that contains the following attributes: 
• <glyphs> - The sprite description containing the glyphs. See  Sprites Resources 
• <letters> - The glyphs contained in <glyphs> 
• <monospace> - If "true", then all letters have equal width 
• <spacelen> - The width of unknown glyphs. If unset, this defaults to the average of the font glyphs. If monospace is used, then this always equals the widest glyph 
• The font metrics can be set using: (value = floating point value, bool = true or false) 
• <height="value"> 
• <ascent="value"> 
• <descent="value"> 
• <internal_leading="value"> 
• <external_leading="value"> 
• <average_character_width="value"> 
• <max_character_width="value"> 
• <weight="value"> 
• <overhang="value"> 
• <digitized_aspect_x="value"> 
• <digitized_aspect_y="value"> 
• <italic="bool"> 
• <underlined="bool"> 
• <struck_out="bool"> 
• <fixed_pitch="bool"> 

*/
