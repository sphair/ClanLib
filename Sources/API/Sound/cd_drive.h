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

//! clanSound="CD Audio"
//! header=sound.h

#ifndef header_cd_drive
#define header_cd_drive

#ifdef CL_API_DLL
#ifdef CL_SOUND_EXPORT
#define CL_API_SOUND __declspec(dllexport)
#else
#define CL_API_SOUND __declspec(dllimport)
#endif
#else
#define CL_API_SOUND
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Core/System/sharedptr.h"
#include <string>
#include <vector>

class CL_CDDrive_Generic;

//: CD drive audio interface.
//- !group=Sound/CD Audio!
//- !header=sound.h!
//- <p>Use this class to control the CDROM drive in your computer.</p>
class CL_API_SOUND CL_CDDrive
{
//! Construction:
public:
	//: Constructs a CD drive object.
	CL_CDDrive();

	CL_CDDrive(const CL_CDDrive &copy);

	virtual ~CL_CDDrive();

//! Attributes:
public:
	//: Returns the amount of CD drives available on the system.
	static std::vector<CL_CDDrive> &get_drives();

	//: Get the path of the CD drive.
	std::string get_drive_path();

	//: Get the name of the CD.
	std::string get_cd_name();

	//: Get the number of tracks available on the cd.
	int get_num_tracks();

	//: Returns true if the CD drive is playing.
	bool is_playing();
	
	//: Returns the current track.
	int get_cur_track();

	//: Returns the current frame on the track.
	int get_cur_frame();

//! Operations:
public:
	//: Play from track 'track' til track 'end_track'.
	//param track: Begin track.
	//param end_track: End track.
	bool play_tracks(int track, int end_track=0);

	//: Play from frame 'frame' to 'end_frame'.
	//param frame: Begin frame.
	//param end_frame: End frame.
	bool play_frames(int frame, int end_frame=0);

	//: Play track 'track'. Returns true if successful.
	bool play_track(int track);

	//: Stop playing.
	void stop();

	//: Pause the playback.
	void pause();

	//: Resume the playback.
	void resume();

//! Implementation:
private:
	CL_SharedPtr<CL_CDDrive_Generic> impl;
};

#endif
