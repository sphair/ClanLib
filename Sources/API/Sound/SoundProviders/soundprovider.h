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

#include <memory>

namespace clan
{
	/// \addtogroup clanSound_Audio_Mixing clanSound Audio Mixing
	/// \{

	class SoundProvider_Session;
	class SoundProvider_Impl;

	/// \brief Sound provider.
	///
	/// Interface to sound sources. Used by the SoundBuffer class to start and stop playback.
	class SoundProvider
	{
	public:
		SoundProvider();
		virtual ~SoundProvider();

		/// \brief Called by SoundBuffer when a new session starts.
		/** \return The soundbuffer session to be attached to the newly started session.*/
		virtual SoundProvider_Session *begin_session() = 0;

		/// \brief Called by SoundBuffer when a session has finished. After this call,
		/** <p>SoundBuffer will not access the session anymore. It can safely be deleted
			here (and in most cases should be delete here).</p>*/
		virtual void end_session(SoundProvider_Session *session) = 0;

	private:
		std::shared_ptr<SoundProvider_Impl> impl;
	};

	/// \}
}
