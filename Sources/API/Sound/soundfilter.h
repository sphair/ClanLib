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
**    Mark Page
*/

/// \addtogroup clanSound_Audio_Mixing clanSound Audio Mixing
/// \{

#pragma once

#include "api_sound.h"
#include "../Core/System/sharedptr.h"

class CL_SoundFilter_Impl;
class CL_SoundFilterProvider;

/// \brief Sound Filter Class
///
/// \xmlonly !group=Sound/Audio Mixing! !header=sound.h! \endxmlonly
class CL_API_SOUND CL_SoundFilter
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a NULL instance
	CL_SoundFilter() {};

	/// \brief Constructs a sound filter
	///
	/// \param provider = The provider
	CL_SoundFilter(CL_SoundFilterProvider *provider);

	~CL_SoundFilter();

/// \}
/// \name Operators
/// \{
public:
	/// \brief Equality operator
	bool operator==(const CL_SoundFilter &other) const
	{
		return impl==other.impl;
	}

	/// \brief Inequality operator
	bool operator!=(const CL_SoundFilter &other) const
	{
		return impl!=other.impl;
	}

/// \}
/// \name Operations
/// \{

public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Retrieves the provider.
	CL_SoundFilterProvider *get_provider() const;

	/// \brief Filter callback.
	/** <p>All sound data is passed through this function,
	    which modifies the sample data accordingly to the function of the
	    filter.</p>
	    <p>The format of the sample data is always 16 bit stereo. </p>*/
	void filter(float **sample_data, int num_samples, int channels);

/// \}
/// \name Implementation
/// \{

public:
	CL_SharedPtr<CL_SoundFilter_Impl> impl;
/// \}
};

/// \}
