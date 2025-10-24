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

//: <p>Sound support. If you want sound in your application, you defiantly
//: want to link to this library.</p>
//! Global=Sound

#ifndef header_sound_api
#define header_sound_api

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "Sound/setupsound.h"
#include "Sound/sound.h"
#include "Sound/soundoutput.h"
#include "Sound/soundoutput_description.h"
#include "Sound/soundformat.h"
#include "Sound/soundprovider.h"
#include "Sound/soundprovider_session.h"
#include "Sound/soundbuffer.h"
#include "Sound/soundbuffer_session.h"
#include "Sound/soundfilter.h"
#include "Sound/cd_drive.h"

#include "Sound/SoundProviders/soundprovider_wave.h"
#include "Sound/SoundProviders/soundprovider_raw.h"
#include "Sound/SoundProviders/soundprovider_recorder.h"

#include "Sound/SoundFilters/echofilter.h"
#include "Sound/SoundFilters/inverse_echofilter.h"
#include "Sound/SoundFilters/fadefilter.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanSound-static-mt.lib")
#else
#pragma comment(lib, "clanSound-static-mt-debug.lib")
#endif
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#endif

#endif
