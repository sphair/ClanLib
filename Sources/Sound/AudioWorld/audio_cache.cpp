
#include "Sound/precomp.h"
#include "API/Sound/AudioWorld/audio_cache.h"
#include "API/Sound/soundbuffer.h"
#include "audio_cache_impl.h"

namespace clan
{

AudioCache::AudioCache()
	: impl(new AudioCache_Impl())
{
}

SoundBuffer AudioCache::get_sound_buffer(const std::string &filename)
{
	std::unordered_map<std::string, SoundBuffer>::iterator it = impl->soundbuffers.find(filename);

	if (it == impl->soundbuffers.end())
	{
		impl->soundbuffers[filename] = SoundBuffer(filename);
		return impl->soundbuffers[filename];
	}
	else
	{
		return it->second;
	}
}

}
