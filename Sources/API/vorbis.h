/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

//: <p>Ogg Vorbis playback. With this library, you can play music with the same
//: quality as todays mp3, but with a completely open sound format (mp3 is
//: patented by some greedy companies that want money for each mp3 you use in a
//: commercial product).</p>
//! Global=Vorbis

#ifndef header_vorbis
#define header_vorbis

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "Vorbis/soundprovider_vorbis.h"
#include "Vorbis/setupvorbis.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanVorbis-static-mt.lib")
#pragma comment(lib, "libvorbis-static-mt.lib")
#pragma comment(lib, "libogg-static-mt.lib")
#else
#pragma comment(lib, "clanVorbis-static-mt-debug.lib")
#pragma comment(lib, "libvorbis-static-mt-debug.lib")
#pragma comment(lib, "libogg-static-mt-debug.lib")
#endif
#endif

#endif
