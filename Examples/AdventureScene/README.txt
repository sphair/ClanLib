         Name: Adventure Scene
       Status: Windows(Y), Linux(Y)
        Level: Advanced
   Maintainer: Core developers
      Summary: Examples some pixelbuffer manipulation.
     
This example show how a simple adventure-engine could be made. It loads
a few bitmaps and uses them to display depth-objects and scale moving
objects. 

Note, this is not a complete adventure game! It requires ALOT more
if you want a full engine. 

There are many other (and better) ways of creating a adventure type-engine,
but this is more an example of CL_PixelBuffer use than anything else.

The images (ex-bkg.tga, ex-mask.tga, ex-wasc.tga) are originally
from the adventure scene example in the MAD Adventure Game Engine 
(http://mad-project.sourceforge.net/manual/scene%20tutorial.html).

Thanks to the Hero6 team for the images (http://www.hero6.com).
They are copyrighted by and used with permission from the Hero6
Project. You may use these images only for example or demonstration
purposes. For other uses you must obtain permission from them.


Explanation of the format:
--------------------------

The format used in this example is a slightly modified version of the
one used in the MAD Adventure Game Engine.

ex-bkg.tga - The Background Screen:
This is just the background for the scene.

ex-wasc.tga - The Walk Scale Screen:
All areas of it that are painted with color 0 are designated as unwalkable,
and any other color is the percent to scale objects at that point in the
image. For example a color of 100 would draw the object at its original
size (100%), and a color of 200 would draw the object at twice its size
(or 200%). Depending upon how the background is drawn, you'll want to chose
a scale that makes the sprites fit in with it.

ex-mask.tga - The Mask Screen:
In the mask screen you'll need to draw solid colored masks of objects that
the player can walk behind. Each object is built up using RGB values.
The R and G colors combined into a 16 bit value are used to specify the
"depth" value of the object. The well in our example is placed at
y-location 209, but we want the player to be able to move a little bit
over it, so we set the depth to 205. It means the player can move to
y-position 205 and still be in front of the object. If the player is further
up on the screen than this, the player will be shown behind the object.
To make an object always show in front of the player, use 0 and 0 for the
R and G values. 

Each object needs to have its own color. Normally the R,G values gives an
unique value for each object, but if you have two objects with the same
depth value, use the B value to distinguish between them.



Again, thanks to the MAD Adventure Game Engine for their explanations,
this section was based on theirs.


Suggestions for improvements:
-----------------------------
1) Completely separate objects into separate files, and use some resource
file to place them around the scene. That way, objects can use proper
alpha to make them antialias better with players moving behind them.
This will also make the code alot more easier to understand, as you
won't need the "complex" object extractor.

2) It could be possible to put both the walk/scale map and the mask map
into one image. Use R,G for object z values, use B for unique object
identifier and use A for walk/scale info.

