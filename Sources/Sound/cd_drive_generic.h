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

#ifndef header_cd_drive_generic
#define header_cd_drive_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>

class CL_CDDrive_Generic
{
//! Construction:
public:
	CL_CDDrive_Generic() { return; }
	
	virtual ~CL_CDDrive_Generic() { return; }

//! Attributes:
public:
	virtual std::string get_drive_path() = 0;

	virtual std::string get_cd_name() = 0;

	virtual int get_num_tracks() = 0;

	virtual bool is_playing() = 0;
	
	virtual int get_cur_track() = 0;

	virtual int get_cur_frame() = 0;

//! Operations:
public:
	virtual bool play_tracks(int track, int end_track) = 0;

	virtual bool play_frames(int frame, int end_frame) = 0;

	virtual bool play_track(int track) = 0;

	virtual void stop() = 0;

	virtual void pause() = 0;

	virtual void resume() = 0;

//! Implementation:
private:
};

#endif
