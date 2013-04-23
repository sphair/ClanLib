
#include "Sound/precomp.h"
#include "API/Sound/AudioWorld/audio_world.h"
#include "API/Sound/AudioWorld/audio_object.h"
#include "API/Sound/soundbuffer.h"
#include "API/Core/Math/cl_math.h"
#include "audio_world_impl.h"
#include "audio_object_impl.h"

namespace clan
{

AudioWorld::AudioWorld()
: impl(new AudioWorld_Impl())
{
}

void AudioWorld::set_listener(const Vec3f &position, const Quaternionf &orientation)
{
	impl->listener_position = position;
	impl->listener_orientation = orientation;
}

bool AudioWorld::is_ambience_enabled() const
{
	return impl->play_ambience;
}

void AudioWorld::enable_reverse_stereo(bool enable)
{
	impl->reverse_stereo = enable;
}

bool AudioWorld::is_reverse_stereo_enabled() const
{
	return impl->reverse_stereo;
}

void AudioWorld::update()
{
	for (std::list<AudioObject_Impl *>::iterator it = impl->objects.begin(); it != impl->objects.end(); ++it)
	{
		impl->update_session(*it);
	}
}

/////////////////////////////////////////////////////////////////////////////

AudioWorld_Impl::AudioWorld_Impl()
: play_ambience(true), reverse_stereo(false)
{
}

AudioWorld_Impl::~AudioWorld_Impl()
{
}

void AudioWorld_Impl::update_session(AudioObject_Impl *obj)
{
	if (obj->attenuation_begin != obj->attenuation_end)
	{
		// Calculate volume from distance
		float distance = obj->position.distance(listener_position);
		float t = 1.0f - clamp((distance - obj->attenuation_begin) / (obj->attenuation_end - obj->attenuation_begin), 0.0f, 1.0f);

		// Calculate pan from ear angle
		Vec3f sound_direction = Vec3f::normalize(obj->position - listener_position);
		Vec3f ear_vector = listener_orientation.rotate_vector(Vec3f(1.0f, 0.0f, 0.0f));
		float pan = Vec3f::dot(ear_vector, sound_direction);
		if (reverse_stereo)
			pan = -pan;

		obj->session.set_volume(obj->volume * t);
		obj->session.set_pan(pan);
	}
	else
	{
		obj->session.set_volume(obj->volume);
		obj->session.set_pan(0.0f);
	}
}

}
