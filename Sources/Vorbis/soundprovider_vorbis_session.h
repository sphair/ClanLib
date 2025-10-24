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

#ifndef header_sound_provider_vorbis_session
#define header_sound_provider_vorbis_session

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Sound/soundprovider_session.h"
#include "vorbis/codec.h"

class CL_SoundProvider_Vorbis_Generic;
class CL_InputSource;

class CL_SoundProvider_Vorbis_Session : public CL_SoundProvider_Session
{
//! Construction:
public:
	CL_SoundProvider_Vorbis_Session(CL_SoundProvider_Vorbis_Generic *data);

	~CL_SoundProvider_Vorbis_Session();

//! Attributes:
public:
	virtual int get_num_samples() const;

	virtual int get_frequency() const;

	virtual CL_SoundFormat get_format() const;

	virtual int get_num_channels() const;

	virtual int get_position() const;

//! Operations:
public:
	virtual bool eof() const;
	
	virtual void stop();

	virtual bool play();
		
	virtual bool set_position(int pos);
	
	virtual int get_data(void **data_ptr, int data_requested);

//! Implementation:
private:
	void stream_data();

	CL_SoundProvider_Vorbis_Generic *data;

	int num_samples;

	int position;

	CL_InputSource *input;

	bool stream_eof;

	//: Sync and verify incoming physical bitstream.
	ogg_sync_state oy;

	//: Take physical pages, weld into a logical stream of packets.
	ogg_stream_state os;

	//: One Ogg bitstream page.  Vorbis packets are inside.
	ogg_page og;

	//: One raw packet of data for decode.
	ogg_packet op;

	//: Struct that stores all the static vorbis bitstream settings.
	vorbis_info vi;

	//: Struct that stores all the bitstream user comments.
	vorbis_comment vc;

	//: Central working state for the packet->PCM decoder.
	vorbis_dsp_state vd;

	//: Local working space for packet->PCM decode.
	vorbis_block vb;
};

#endif
