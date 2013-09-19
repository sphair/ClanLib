
/*! \page SpriteResource Sprite Resources


The \ref clan::Sprite resource options are a plethora of possibilities to tweak a sprites looks and behaviours, but all of them have default values. In most cases you will only need to use the basic options.

Only the name attribute of \<sprite\> and at least one \<image\> element is required to construct a sprite. The remaining elements and attributes are optional.

<h2>Using the \<sprite\> element</h2>

<ul><li>Attribute: <b>name</b><br/>
Resource identifier to use as base for this sprite<br/>
Default value: None, MUST BE PRESENT. 
</li></ul>

<ul><li>Attribute: <b>base_angle</b><br/>
Defines what direction the sprite is in. All other angles are relative to this one.<br/>
Default value: "0" 
</li></ul>

<ul><li>Attribute: <b>id</b><br/>
Sets the sprite identify retrievable via \ref clan::Sprite::get_id().<br/>
Default value: "0" 
</li></ul>

<ul><li>Element: <b>\<image\> </b><br/>
Description of the sprite frames.<br/>
You can specify any number of \<image\> elements
</li></ul>

<ul><li>Element: <b>\<color\>  </b><br/>
Description of the sprite color.<br/>
You can specify any number of \<color\> elements
</li></ul>

<ul><li>Element: <b>\<frame\> </b><br/>
Description of the sprite frames.<br/>
You can specify any number of \<frame\> elements
</li></ul>


<ul><li>Element: <b>\<animation\>  </b><br/>
Description of the sprite animation.<br/>
You can specify any number of \<animation\> elements
</li></ul>


<ul><li>Element: <b>\<scale\>  </b><br/>
Description of the sprite scale.<br/>
You can specify any number of \<scale\> elements
</li></ul>


<ul><li>Element: <b>\<translation\> </b><br/>
Description of the sprite translation.<br/>
You can specify any number of \<translation\> elements
</li></ul>


<ul><li>Element: <b>\<rotation\>  </b><br/>
Description of the sprite rotation.<br/>
You can specify any number of \<rotation\> elements
</li></ul>


<h2>Using the \<image\> element</h2>

The first step in setting up a sprite is telling the resource loader where it should get the images for all the frames. This is done specifying one or more \<image\> elements. Each \<image\> element specify an image from where one or several frames should be extracted:

<ul><li>If there is no child element in \<image\>, it will simply take the entire image and add it as one large frame. </li></ul>
<ul><li>If the child element is \<grid\> it will use the grid cutter to extract a set of frames placed in a grid in the image file. </li></ul>
<ul><li>If the child is \<alpha\> the alpha cutter will be used instead. The alpha cutter uses the alpha channel to find frames separated with pure alpha (within trans_limit). </li></ul>

<ul><li>Attribute: <b>file</b> (Optional) <br/>
Image filename.<br/>
</li></ul>
 
<ul><li>Element: <b>\<grid\> </b> (Optional - Only when file attribute is set)<br/>
Use the grid cutter to extract a set of frames placed in a grid in the image file<br/>
You can specify any number of \<grid\> elements
</li></ul>

<ul><li>Element: <b>\<alpha\> </b> (Optional - Only when file attribute is set)<br/>
The alpha cutter uses the alpha channel to find frames separated with pure alpha<br/>
You can specify any number of \<alpha\> elements
</li></ul>

<ul><li>Attribute: <b>fileseq</b> (Optional) <br/>
A sequence of images<br/>
Valid values: "filename.ext" where ext is any of the supported ClanLib image types (for example png, jpg) so that it will be translated into "filename0001.ext"<br/>
Default value: None. For non-sequenced images, use the file attribute.<br/>
</li></ul>

<ul><li>Attribute: <b>start_index</b>  (Optional - Used when fileseq attribute is set) <br/>
The start index.<br/>
Valid values: "integer" - greater or equal to 0<br/>
Default value: "0" 
</li></ul>

<ul><li>Attribute: <b>skip_index</b>  (Optional - Used when fileseq attribute is set) <br/>
How many images to skip per iteration. (fileseq) <br/>
Valid values: "integer" - greater or equal to 1<br/>
Default value: "1"<br/>
</li></ul>

<ul><li>Attribute: <b>leading_zeroes </b>  (Optional - Used when fileseq attribute is set) <br/>
Number of zeroes before the extension. <br/>
Valid values: "integer" - greater or equal to 0<br/>
Default value: "0" <br/>
</li></ul>

<h2>Using the \<grid\> element</h2>


        Attribute pos: Position in image to start grid-cutting. 

        Valid values: "integer, integer" - x-position, y-position
        Default value: "0, 0" 

        Attribute size: Size of each grid-tile. 

        Valid values: "integer, integer" - width, height
        Default value: "1, 1" 

        Attribute array: Grid-size. 

        Valid values: "integer, integer" - width, height
        Default value: None, MUST BE PRESENT. 

        Attribute array_skipframes: How many frames to skip at end of last gridline. 

        Valid values: "integer" - frames to skip
        Default value: "0" 

        Attribute spacing: Space between each grid-tile. 

        Valid values: "integer, integer" - x-spacing, y-spacing
        Default value: "0, 0" 

<h2>Using the \<alpha\> element</h2>


        Attribute pos: Position in image to start alpha-cutting. 

        Valid values: (integer,integer)
        Default value: "0, 0" 

        Attribute free: Use the "Free Alpha Cutter". 

        The default alpha cutter finds columns of sprites, all of which have the 
        same height and variable width. The "Free Cutter" identifies all rectangular 
        non alpha blocks of pixels and puts them on a single frame. The algorithms 
        starts at the top left corner (or the specified position) and scans the 
        image line by line, from top to bottom. 
        Valid values: blank, true
        Default value: blank 

        Attribute trans_limit: Transparency limit. 

        Valid values: "float" - between 0.0 and 1.0
        Default value: "0.05" 


<h2>Using the \<color\> element</h2>

        Attributes red, green, blue, alpha: Color. 

        Sets the red, green, blue and alpha color components of the sprite. 
        Valid values: "float" - between 0.0 and 1.0
        Default values: 1.0, 1.0, 1.0, 1.0 


<h2>Using the \<animation\> element</h2>

        Attribute speed: Default frame delay. 

        This sets the delay between each frame. You can override separate frames using frameX_speed (see below). 
        Value is in milliseconds. 
        Valid values: integer
        Default value: 60 

        Attribute loop: Loop the animation. 

        Set it to loop if you want the animation to loop after it has reached end of 
        of the animation. 
        Valid values: "yes, no" - Enable or disable looping.
        Default value: "yes" 

        Attribute pingpong: Pingpong the animation. 

        Set it to pingpong if you want the animation to play back to start once it has reached 
        the end of the animation. 
        Valid values: "yes, no" - Enable or disable pingpong.
        Default value: "no" 

        Attribute direction: Direction of animation. 

        Set it to backwards if you want the animation to play backwards - starts at 
        last frame, and plays forward to first frame. 
        Valid values: "backward, forward" - Play the animation backwards or forwards.
        Default value: "forward" 

        Attribute on_finish: What to show when animation is finished. 

        Specify what is shown when the animation is finished. Blank shows nothing, 
        last_frame shows last frame in animation, first_frame shows first frame in animation. 
        If you use looping, this option has no effect. 
        Valid values: blank, last_frame, first_frame - one of the 3 options
        Default value: blank 


<h2>Using the \<scale\> element</h2>


        Attributes x and y: Scale. 

        Sets the x and y scale of the sprite. A value of 1.0 is the normal size, 
        2.0 is double the size, etc. 
        Valid values: (float, float)
        Default value: (1.0, 1.0) 


<h2>Using the \<translation\> element</h2>

        Attribute origin: Hotspot/alignment for translation operations. 

        This is which pixel will go at the location specified with CL_Sprite::draw. 
        So if your sprite has origin=top_left (the default), and you call, say, 
        my_sprite.draw(50, 100), the very top left hand pixel will be placed at (50, 100) 
        and the rest of the sprite drawn around that. If you have origin=center then the 
        centermost pixel will be placed at (50, 100) and the rest of the picture drawn 
        around that. 

        Valid values: top_left, top_center, top_right, center_left, center, center_right, bottom_left, bottom_center, bottom_right
        Default value: top_left 

        Attributes x and y: This is how far from the origin the picture should be placed. 

        x=50 will draw the picture 50 pixels right of where it would normally go. Note that this does take into account the value of origin. 
        Valid values: integer
        Default value: 0 

<h2>Using the \<rotation\> element</h2>

        Attribute origin: Hotspot/alignment for rotation operations. 

        This is the pixel that will stay where it is when you rotate the image. 
        By default this is the center, so any rotations will leave the center pixel 
        where it is and rotate the rest of the image around it. If you want to rotate 
        about the top left corner set this to top_left, etc. 
        Valid values: top_left, top_center, top_right, center_left, center, center_right, bottom_left, bottom_center, bottom_right
        Default value: center 

        Attributes x and y: This is how far to offset the hotspot from the origin. 

        Valid values: integer
        Default value: 0 

<h2>Using the \<frame\> element</h2>


        Attribute speed: Override default speed for this specific frame. 

        Value is in milliseconds. 
        Valid values: integer
        Default value: play_speed 

        Attributes x and y: Offset to display this frame. 

        This let you set a position-offset on a frame, finetuning its 
        position relative to the other frames. 
        Valid values: (integer, integer) - x-position, y-position
        Default value: (0, 0) 

*/
