
/*! \page SpriteResource Sprite Resource

Resource options

The sprite resource options is a plethora of possibilities to tweak a sprites looks and behaviours, but all of them have default values. In most cases you will only need to use the basic options.

Each sprite can have the following properties:

 
<sprite
	name="my_sprite"
	description="resource_containing_shared_description_data"
	pack_texture="[yes,no]"
	base_angle="angle"
	id="id">
 
	<!-- Frame image loading: -->
 
	<image 
		fileseq="filename.extension"  
		start_index="start_index" 
		skip_index="skip_index" 
		leading_zeroes="leading_zeroes" />
 
	<image file="filename" />
 
	<image file="filename">
		<grid
			pos="x,y"
			size="width,height"
			array="tiles_x,tiles_y"
			array_skipframes="skip_count"
			spacing="width,height" />
	</image>
 
	<image file="filename">
		<palette
			pos="x,y" />
	</image>
 
	<image file="image4.png">
		<alpha
			pos="x,y"
 			free="true"
			trans_limit="limit" />
	</image>
 
	<!-- Sprite render and animation states: -->
 
	<color
		red="red_component"
		green="green_component"
		blue="blue_component"
		alpha="alpha_component" />
 
	<animation
		speed="speed"
		loop="[yes,no]"
		pingpong="[yes,no]"
		direction="[backward,forward]"
		on_finish="[blank,last_frame,first_frame]" />
 
	<scale x="scale_x" y="scale_y" />
 
	<translation
		origin="[top_left, top_center, top_right,
		         center_left, center, center_right,
		         bottom_left, bottom_center, bottom_right]"
		x="offset_x"
		y="offset_y" />
 
	<rotation
		origin="[top_left, top_center, top_right,
		         center_left, center, center_right,
		         bottom_left, bottom_center, bottom_right]"
		x="offset_x"
		y="offset_y" />
 
	<frame
		nr="frame_number"
		speed="frame_delay"
		x="offset_x"
		y="offset_y" />
 
</sprite>

Only the name attribute of <sprite> and at least one <image> element is required to construct a sprite. The remaining elements and attributes are optional.
Using the <image> element

The first step in setting up a sprite is telling the resource loader where it should get the images for all the frames. This is done specifying one or more <image> elements. Each <image> element specify an image from where one or several frames should be extracted:

        If there is no child element in <image>, it will simply take the entire image and add it as one large frame. 

        If the child element is <grid> it will use the grid cutter in CL_SpriteDescription to extract a set of frames placed in a grid in the image file. 

        If the child is <alpha> the alpha cutter will be used instead. The alpha cutter uses the alpha channel to find frames separated with pure alpha (within trans_limit). 

        Then there is the <palette> child element. This method adds frames separated with palette-colours defining the boundaries. 

If you have many sprites that are using the same frames for its animation, you can use the description attribute on the <sprite> element to use the frames from an other sprite resource.
Setting up render and animation

The remaining elements <color>, <animation>, <scale>, <translation> and <rotation> alter the default values of the CL_Sprite render and animation properties.

<frame> sets up properties for a specific frame.
Sprite resource options reference
<sprite>

        Attribute name: Name of resource. 

        Valid values:
        Default value: None, MUST BE PRESENT. 

        Attribute description: Resource identifier of other resource to use as base for this sprite. 

        Valid values: "string" - Resource ID of other sprite resource
        Default value: Don't use any other sprite resource as base. 

        Attribute pack_texture: When pack_texture is enabled CL_Sprite will pack as many frames as it can into the same texture object. 

        Valid values: "yes, no" - Enable or disable texture packing
        Default value: yes 

        Attribute base_angle: Defines what direction the sprite is in. All other angles are relative to this one. 

        Valid values:
        Default value: "0" 

        Attribute id: Sets the sprite identify retrievable via CL_Sprite::get_id(). 

        Valid values:
        Default value: "0" 


<image>

        Attribute file: Image filename. 

        Valid values:
        Default value: None, MUST BE PRESENT. 

        Attribute fileseq: A sequence of images (Optional). 

        Valid values: "filename.ext" where ext is any of the supported ClanLib image types (for example png, jpg, tga, pcx)
        Default value: None. For non-sequenced images, use the file attribute. 

        Attribute start_index: The start index. (fileseq) 

        Valid values: "integer" - greater or equal to 0
        Default value: "0" 

        Attribute skip_index: How many images to skip per iteration. (fileseq) 

        Valid values: "integer" - greater or equal to 1
        Default value: "1" 

        Attribute leading_zeroes: Number of zeroes after the underscore. (fileseq) 

        Valid values: "integer" - greater or equal to 0
        Default value: "0" 


<grid>

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


<palette>

        Attribute pos: Position in image to start palette-cutting. 

        Valid values:
        Default value: "0, 0" 


<alpha>

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


<color>

        Attributes red, green, blue, alpha: Color. 

        Sets the red, green, blue and alpha color components of the sprite. 
        Valid values: "float" - between 0.0 and 1.0
        Default values: 1.0, 1.0, 1.0, 1.0 


<animation>

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


<scale>

        Attributes x and y: Scale. 

        Sets the x and y scale of the sprite. A value of 1.0 is the normal size, 
        2.0 is double the size, etc. 
        Valid values: (float, float)
        Default value: (1.0, 1.0) 


<translation>

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


<rotation>

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


<frame>

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
