/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

/// \addtogroup clanSound_CD_Audio clanSound CD Audio
/// \{

#pragma once

#include "api_sound.h"
#include "../Core/System/sharedptr.h"
#include "../Core/Text/string_types.h"
#include <vector>

class CL_CDDrive_Impl;

/// \brief CD drive audio interface.
///
/// Use this class to control the CDROM drive in your computer.
/// \xmlonly !group=Sound/CD Audio! !header=sound.h! \endxmlonly
class CL_API_SOUND CL_CDDrive
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance
	///
	/// Use get_drives() instead
	CL_CDDrive();

	virtual ~CL_CDDrive();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the amount of CD drives available on the system.
	static std::vector<CL_CDDrive> &get_drives();

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get the path of the CD drive.
	CL_String8 get_drive_path();

	/// \brief Get the name of the CD.
	CL_String8 get_cd_name();

	/// \brief Get the number of tracks available on the cd.
	int get_num_tracks();

	/// \brief Returns true if the CD drive is playing.
	bool is_playing();

	/// \brief Returns the current track.
	int get_cur_track();

	/// \brief Returns the current frame on the track.
	int get_cur_frame();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Play from track 'track' til track 'end_track'.
	///
	/// \param track Begin track.
	/// \param end_track End track.
	bool play_tracks(int track, int end_track=0);

	/// \brief Play from frame 'frame' to 'end_frame'.
	///
	/// \param frame Begin frame.
	/// \param end_frame End frame.
	bool play_frames(int frame, int end_frame=0);

	/// \brief Play track 'track'. Returns true if successful.
	bool play_track(int track);

	/// \brief Stop playing.
	void stop();

	/// \brief Pause the playback.
	void pause();

	/// \brief Resume the playback.
	void resume();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_CDDrive_Impl> impl;
/// \}
};

/// \}
