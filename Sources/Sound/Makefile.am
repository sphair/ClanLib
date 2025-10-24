lib_LTLIBRARIES = libclanSound.la

libclanSound_la_SOURCES = \
SoundFilters/echofilter_generic.cpp \
SoundFilters/fadefilter_generic.cpp \
SoundFilters/inverse_echofilter_generic.cpp \
SoundProviders/soundprovider_factory.cpp \
SoundProviders/soundprovider_raw.cpp \
SoundProviders/soundprovider_raw_session.cpp \
SoundProviders/soundprovider_type.cpp \
SoundProviders/soundprovider_wave.cpp \
SoundProviders/soundprovider_wave_session.cpp \
SoundProviders/soundprovider_recorder.cpp \
SoundProviders/soundprovider_recorder_generic.h \
cd_drive.cpp \
precomp.cpp \
resourcetype_sample.cpp \
setupsound.cpp \
sound.cpp \
soundbuffer.cpp \
soundbuffer_generic.cpp \
soundbuffer_session.cpp \
soundbuffer_session_generic.cpp \
soundfilter.cpp \
soundoutput.cpp \
soundoutput_description.cpp \
soundoutput_generic.cpp \
soundprovider.cpp \
soundprovider_session.cpp

if WIN32
libclanSound_la_SOURCES += \
Win32/soundoutput_directsound.cpp \
Win32/soundoutput_directsound.h \
SoundProviders/Win32/soundprovider_recorder_directsound.cpp \
SoundProviders/Win32/soundprovider_recorder_directsound.h
else
libclanSound_la_SOURCES += \
Unix/soundoutput_oss.cpp \
Unix/soundoutput_oss.h
if ALSA
libclanSound_la_SOURCES += \
Unix/soundoutput_alsa.cpp \
Unix/soundoutput_alsa.h
endif
libclanSound_la_SOURCES += \
SoundProviders/Unix/soundprovider_recorder_oss.cpp \
SoundProviders/Unix/soundprovider_recorder_oss.h
endif

libclanSound_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(SOUND_LDFLAGS)
  
# EOF #
