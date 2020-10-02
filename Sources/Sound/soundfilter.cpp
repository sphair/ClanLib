/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "Sound/precomp.h"
#include "API/Sound/soundfilter.h"
#include "API/Sound/SoundProviders/soundfilter_provider.h"

namespace clan
{
	class SoundFilter_Impl
	{
	public:
		SoundFilter_Impl()
			: provider(nullptr)
		{
		}

		~SoundFilter_Impl()
		{
			if (provider)
				delete provider;
		}

		SoundFilterProvider *provider;
	};

	SoundFilter::SoundFilter(SoundFilterProvider *provider)
		: impl(std::make_shared<SoundFilter_Impl>())
	{
		impl->provider = provider;
	}

	SoundFilter::~SoundFilter()
	{
	}

	void SoundFilter::throw_if_null() const
	{
		if (!impl)
			throw Exception("SoundFilter is null");
	}

	SoundFilterProvider *SoundFilter::get_provider() const
	{
		if (!impl)
			return nullptr;
		return impl->provider;
	}

	void SoundFilter::filter(float **sample_data, int num_samples, int channels)
	{
		impl->provider->filter(sample_data, num_samples, channels);
	}
}
