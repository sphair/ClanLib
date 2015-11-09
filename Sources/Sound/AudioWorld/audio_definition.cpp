
#include "Sound/precomp.h"
#include "API/Sound/AudioWorld/audio_definition.h"
#include "audio_definition_impl.h"

namespace clan
{
	AudioDefinition::AudioDefinition()
		: impl(std::make_shared<AudioDefinition_Impl>())
	{
	}

	float AudioDefinition::get_attenuation_begin() const
	{
		return impl->attenuation_begin;
	}

	float AudioDefinition::get_attenuation_end() const
	{
		return impl->attenuation_end;
	}

	float AudioDefinition::get_volume() const
	{
		return impl->volume;
	}

	bool AudioDefinition::is_looping() const
	{
		return impl->looping;
	}

	bool AudioDefinition::is_ambience() const
	{
		return impl->ambience;
	}

	std::string AudioDefinition::get_sound_id() const
	{
		return impl->sound_id;
	}

	void AudioDefinition::set_attenuation_begin(float distance)
	{
		impl->attenuation_begin = distance;
	}

	void AudioDefinition::set_attenuation_end(float distance)
	{
		impl->attenuation_end = distance;
	}

	void AudioDefinition::set_volume(float volume)
	{
		impl->volume = volume;
	}

	void AudioDefinition::set_sound_id(const std::string &id)
	{
		impl->sound_id = id;
	}

	void AudioDefinition::set_looping(bool loop)
	{
		impl->looping = loop;
	}

	void AudioDefinition::set_ambience(bool ambience)
	{
		impl->ambience = ambience;
	}

	/////////////////////////////////////////////////////////////////////////////

	AudioDefinition_Impl::AudioDefinition_Impl()
		: attenuation_begin(0.0f), attenuation_end(0.0f), volume(1.0f), looping(false), ambience(false)
	{
	}
}
