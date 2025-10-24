/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Mark Page

This theme was created for clanlib using GIMP

Color's used

RGB Color_A = 38,96,127    (dark blue for inner button graphics)
RGB Color_B = 77,194,255   (buttons and window blue)
RGB Color_C = 128,193,254  (buttons and window lighter blue)

Gradients used:
Gradient_AB = Color_A to Color_C radial  (For radio buttons)
Gradient_BC = Color_B to Color_C linear (Color_C at top)  (For component pressed background)

States:

Pressed = Bevel(1)
Normal = Bevel(1). Outer [ HSV: Light(95), Sat(-100) ]  Inner [ HSV: Light(50), Sat(-50) ]
Hot = Bevel(1).  Outer [ HSV: Light(95) ]  Inner [ HSV: Light(50) ]
Disabled = No Bevel. Outer [ HSV: Light(100)+Light(100), Sat(-100) ]  Inner [ HSV: Light(100) + Light(50), Sat(-100) ]

Note: buttons, rounded corners = 3
Note: Scrollbar buttons, rounded corners = 4
