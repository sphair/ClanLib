lib_LTLIBRARIES = libclan40Sound.la

libclan40Sound_la_SOURCES = \
Mixer/sound_format_conversion.cpp \
soundbuffer_session.cpp \
sound.cpp \
SoundProviders/soundprovider_raw.cpp \
SoundProviders/soundprovider_vorbis.cpp \
SoundProviders/soundprovider_raw_session.cpp \
SoundProviders/soundprovider_factory.cpp \
SoundProviders/soundprovider.cpp \
SoundProviders/soundprovider_session.cpp \
SoundProviders/soundprovider_vorbis_session.cpp \
SoundProviders/soundprovider_type.cpp \
SoundProviders/soundprovider_wave_session.cpp \
SoundProviders/soundprovider_wave.cpp \
setupsound.cpp \
precomp.cpp \
soundoutput_impl.cpp \
soundfilter.cpp \
soundbuffer_impl.cpp \
SoundFilters/inverse_echofilter.cpp \
SoundFilters/echofilter.cpp \
SoundFilters/fadefilter.cpp \
SoundFilters/fadefilter_provider.cpp \
SoundFilters/echofilter_provider.cpp \
SoundFilters/inverse_echofilter_provider.cpp \
AudioWorld/audio_object.cpp \
AudioWorld/audio_world.cpp \
AudioWorld/audio_definition.cpp \
soundbuffer_session_impl.cpp \
soundbuffer.cpp \
soundoutput_description.cpp \
sound_sse.cpp \
soundoutput.cpp

if WIN32
libclan40Sound_la_SOURCES += \
Platform/Win32/soundoutput_directsound.cpp \
Platform/Win32/init_guids.cpp \
Platform/Win32/soundoutput_win32.cpp
else
libclan40Sound_la_SOURCES += \
Platform/Linux/soundoutput_oss.cpp
if ALSA
libclan40Sound_la_SOURCES += \
Platform/Linux/soundoutput_alsa.cpp
endif
endif

libclan40Sound_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanSound)

libclan40Sound_la_CXXFLAGS=$(clanSound_CXXFLAGS) $(extra_CFLAGS_clanSound)
# EOF #

