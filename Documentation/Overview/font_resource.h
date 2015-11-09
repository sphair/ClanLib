
/*! \page FontResource Font Resource

\ref clan::Font can be constructed from a resource file.

<h2>Using the \<font\> element</h2>

The font element must contain either the \<sprite\>  or \<ttf\> element

<ul>
<li>Attribute: <b>name</b><br/>
Resource identifier to use as base for this font. This is specified in the font family name of \ref clan::FontFamily <br/>
Default value: None, MUST BE PRESENT. 
</li>
<li>Element: <b>\<sprite\> </b><br/>
Describes a sprite font<br/>
</li>
<li>Element: <b>\<ttf\>  </b><br/>
Describes a system font<br/>
</li>
</ul>

<h2>Using the \<sprite\> element</h2>

<ul>
<li>Attribute: <b>glyphs</b><br/>
The sprite description containing the glyphs. See \ref SpriteResource <br/>
Default value: None, MUST BE PRESENT. 
</li>
<li>Attribute: <b>letters</b><br/>
The glyphs contained in /<glyphs\>  <br/>
Default value: None, MUST BE PRESENT. 
</li>
<li>Attribute: <b>monospace</b> (Optional) <br/>
If "true", then all letters have equal width 
</li>
<li>Attribute: <b>spacelen</b> (Optional) <br/>
The width of unknown glyphs. If unset, this defaults to the average of the font glyphs. If monospace is used, then this always equals the widest glyph 
</li>
<li>Attribute: <b>height</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>ascent</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>descent</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>internal_leading</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>external_leading</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>average_character_width</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>max_character_width</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>weight</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>overhang</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>digitized_aspect_x</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>digitized_aspect_y</b> (Optional) <br/>floating point value 
</li>
<li>Attribute: <b>italic</b> (Optional) <br/>true or false 
</li>
<li>Attribute: <b>underlined</b> (Optional) <br/>true or false 
</li>
<li>Attribute: <b>struck_out</b> (Optional) <br/>true or false 
</li>
<li>Attribute: <b>fixed_pitch</b> (Optional) <br/>true or false 
</li>
</ul>

<h2>Using the \<ttf\> element</h2>

<ul>
<li>Attribute: <b>file</b> (Optional) <br/>
Use this filename that contains the font.<br/>
</li>
<li>Attribute: <b>typeface</b><br/>
Use this to change the typeface name of the font. i.e. The typeface name inside the font file.<br/>
Default value: None, MUST BE PRESENT.
</li>
<li>Attribute: <b>height</b> (Optional) <br/>floating point value
</li>
<li>Attribute: <b>average_width</b> (Optional) <br/>floating point value
</li>
<li>Attribute: <b>anti_alias</b><br/>true or false 
</li>
<li>Attribute: <b>subpixel</b><br/>true or false 
</li>
</ul>

*/
