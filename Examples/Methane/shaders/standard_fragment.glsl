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
*/

// -------------------------
// Shandard Fragment Shader
// -------------------------

varying vec2 texCoord;

uniform sampler2D decalMap;
uniform float Lighting;

void main()
{
	vec4 decalCol = texture2D(decalMap, texCoord);
	
	if (decalCol.a == 1.0)
	{
		gl_FragColor.r = clamp(decalCol.r + Lighting, 0.0, 1.0);
		gl_FragColor.g = clamp(decalCol.g + Lighting, 0.0, 1.0);
		gl_FragColor.b = clamp(decalCol.b + Lighting, 0.0, 1.0);
		gl_FragColor.a = 1.0;
	}else
	{
		gl_FragColor.r = 1.0;
		gl_FragColor.g = 0.0;
		gl_FragColor.b = 0.0;
		gl_FragColor.a = 0.0;
	}
}

