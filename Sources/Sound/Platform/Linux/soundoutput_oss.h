/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
*/

#pragma once

#include "../../soundoutput_impl.h"

namespace clan
{

class SoundOutput_OSS : public SoundOutput_Impl
{
/// \name Construction
/// \{
public:
	SoundOutput_OSS(int mixing_frequency, int mixing_latency);
	~SoundOutput_OSS();
/// \}

/// \name Attributes
/// \{
public:
	int dev_dsp_fd;
	int frag_size;
	bool has_sound;
/// \}

/// \name Operations
/// \{
public:
	/// \brief Called when we have no samples to play - and wants to tell the soundcard
	/// \brief about this possible event.
	virtual void silence() override;

	/// \brief Returns true if all fragments are filled with data.
	virtual bool is_full();

	/// \brief Returns the buffer size used by device (returned as num [stereo] samples).
	virtual int get_fragment_size() override;

	/// \brief Writes a fragment to the soundcard.
	virtual void write_fragment(float *data) override;

	/// \brief Waits until output source isn't full anymore.
	virtual void wait() override;
/// \}

/// \name Implementation
/// \{
private:
/// \}
};

}
