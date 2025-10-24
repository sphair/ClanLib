/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#pragma once

#include "API/Sound/SoundProviders/soundprovider_session.h"
#include "API/Vorbis/soundprovider_vorbis.h"
#include "vorbis/codec.h"

class CL_IODevice;

class CL_SoundProvider_Vorbis_Session : public CL_SoundProvider_Session
{
/// \name Construction
/// \{

public:
	CL_SoundProvider_Vorbis_Session(CL_SoundProvider_Vorbis &source);
	~CL_SoundProvider_Vorbis_Session();

/// \}
/// \name Attributes
/// \{

public:
	int get_num_samples() const;
	int get_frequency() const;
	int get_num_channels() const;
	int get_position() const;

/// \}
/// \name Operations
/// \{

public:
	bool eof() const;
	void stop();
	bool play();
	bool set_position(int pos);
	bool set_end_position(int pos) { return false; }
	int get_data(float **data_ptr, int data_requested);

/// \}
/// \name Implementation
/// \{

private:
	void stream_data();

	CL_SoundProvider_Vorbis source;
	int num_samples;
	int position;
	CL_IODevice *input;
	bool stream_eof;

	/// \brief Sync and verify incoming physical bitstream.
	ogg_sync_state oy;

	/// \brief Take physical pages, weld into a logical stream of packets.
	ogg_stream_state os;

	/// \brief One Ogg bitstream page.  Vorbis packets are inside.
	ogg_page og;

	/// \brief One raw packet of data for decode.
	ogg_packet op;

	/// \brief Struct that stores all the static vorbis bitstream settings.
	vorbis_info vi;

	/// \brief Struct that stores all the bitstream user comments.
	vorbis_comment vc;

	/// \brief Central working state for the packet->PCM decoder.
	vorbis_dsp_state vd;

	/// \brief Local working space for packet->PCM decode.
	vorbis_block vb;
/// \}
};


