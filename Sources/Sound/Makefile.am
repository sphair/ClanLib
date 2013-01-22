lib_LTLIBRARIES = libclan30Sound.la

libclan30Sound_la_SOURCES = \
SoundFilters/echofilter.cpp \
SoundFilters/fadefilter.cpp \
SoundFilters/inverse_echofilter.cpp \
SoundFilters/echofilter_provider.h \
SoundFilters/fadefilter_provider.h \
SoundFilters/inverse_echofilter_provider.h \
SoundFilters/echofilter_provider.cpp \
SoundFilters/fadefilter_provider.cpp \
SoundFilters/inverse_echofilter_provider.cpp \
SoundProviders/soundprovider_factory.cpp \
SoundProviders/soundprovider_raw.cpp \
SoundProviders/soundprovider_raw_session.cpp \
SoundProviders/soundprovider_type.cpp \
SoundProviders/soundprovider_wave.cpp \
SoundProviders/soundprovider_wave_session.cpp \
SoundProviders/soundprovider.cpp \
SoundProviders/soundprovider_session.cpp \
cd_drive.cpp \
precomp.cpp \
resourcetype_sample.cpp \
setupsound.cpp \
sound.cpp \
soundbuffer.cpp \
soundbuffer_impl.cpp \
soundbuffer_session.cpp \
soundbuffer_session_impl.cpp \
soundfilter.cpp \
soundoutput.cpp \
soundoutput_description.cpp \
soundoutput_impl.cpp \
sound_sse.cpp

if WIN32
libclan30Sound_la_SOURCES += \
Win32/soundoutput_directsound.cpp \
Win32/soundoutput_directsound.h
else
libclan30Sound_la_SOURCES += \
Unix/soundoutput_oss.cpp \
Unix/soundoutput_oss.h
if ALSA
libclan30Sound_la_SOURCES += \
Unix/soundoutput_alsa.cpp \
Unix/soundoutput_alsa.h
endif
endif

libclan30Sound_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanSound)

libclan30Sound_la_CXXFLAGS=$(clanSound_CXXFLAGS) $(extra_CFLAGS_clanSound)
# EOF #

