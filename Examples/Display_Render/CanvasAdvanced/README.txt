         Name: CanvasAdvanced
       Status: Windows(Y)
        Level: Intermediate
   Maintainer: Core Developers
      Summary: Canvas, Lights, destructable terrain
      
Demonstrates how to modify surface data for lighting effects using ClanLibs 
drawing functions.

A random colored light will follow the mouse pointer around.
Press the left mouse button to create random lights.


Methodology
-----------

This effect is achieved by masking the viewport with a semi-transparent 
texture.  We use the framebuffer to "cut" out lights each cycle. For colors, 
we simply draw an alpha-blended image/texture at the same location as the cut 
outs (in the main canvas, not the frame buffer).  I've provided a simple container 
class and some methods for scaling and changing colors of the lights.

Day and night cycling is possible by simply interpolating the alpha value of 
the "mask."  If you wanted to get fancy, you could adjust the red/blue color 
components as well, to give sunset and dawn effects or a nice blue moonlit 
evening.


Scalability
-----------

There's two ways to handle the masking.  The first is simplier, but a little 
slower (or significantly slower on older hardware).  This method is to simply 
use a "mask" that is the size of the destination viewport.  Unfortunately, that
makes multi-resolution targets trickier, since you'll have to have a mask for 
each resolution. If you have a fixed resolution game, it's probably a moot 
point, and this method might be desired.

The scaling method is slightly more complex, but it allows for easy scaling 
across multiple resolutions and lower end hardware will probably perform better 
with it (it's about 5% faster on my setup, which is fairly stout - 
SLI 260 core 216s; gains may be larger on lower end hardware).  You scale a 
smaller texture (256x256 in this case) to fill the screen, and then do some 
translating of your coordinates so the lights appear in the proper spot.

Most people will probably find the scaling method more useful, though it's 
difficult to say if the translations might get complicated further down the 
development path.  I can't see how, but I thought I'd throw it out there... :)


See the documentation at www.clanlib.org for further information.

