
/*! \page SoundOverview Sound Overview

MainDocs:Sound

From ClanLib Game SDK
 

ClanSound provides you with easy-to-use and powerful sound functionality. 



Contents
•1 Sound overview
•2 Playing wav samples
•3 Playing music streams
•4 Resources
•5 Sessions
•6 Filters
•7 Sound providers
 

Sound overview

To play sound in ClanLib, you have to initialize CL_SetupSound and also create a CL_SoundOutput. CL_SoundOutput is the interface to a sound output device, which is used to control the main mixer volume and other global settings. It is used as a singleton. 


 
	#include <ClanLib/sound.h> 
 
	...
 
	CL_SetupSound setup_sound;
	CL_SoundOutput output(44100);
 
	...

Playing wav samples

A CL_SoundBuffer contains a sample or music stream. To load and play a WAV file, you do the following: 


 
	// Load a sample from a wave file:
	CL_SoundBuffer sample("beep.wav");
 
	// Play sample
	sample.play();

You can set the volume and panning of soundbuffers. This will change the default attributes of the soundbuffer, and will be effective on all subsequent calls to play(). 


 
	// Turn down volume on this soundbuffer.
	sample.set_volume(0.1f);
 
	// Play only in left speaker.
	sample.set_pan(-1.0f);

Note that setting these attributes don't affect already playing soundbuffers, only when you call play() again afterwards. To have individual control over each playing sound, you need to use a sound session - read about that further down. 

Playing music streams

There isn't much difference between playing wav samples and music stream. ClanSound itself doesn't provide more than wav samples playback, so you'll need one or more additional music modules. ClanLib currently has two, MikMod and Vorbis. They are initialized using CL_SetupMikMod and CL_SetupVorbis. 


 
	#include <ClanLib/sound.h> 
	#include <ClanLib/vorbis.h> 
	#include <ClanLib/mikmod.h> 
 
	CL_SetupSound setup_sound;
	CL_SetupMikMod setup_mikmod;
	CL_SetupVorbis setup_vorbis;
 
	CL_SoundOutput output(44100);

Then to play music, you use the same approach as with wav samples: 


 
	// Load ogg file into a soundbuffer
	CL_SoundBuffer vorbis("cheer1.ogg");
 
	// Play vorbis stream
	vorbis.play();
 
	...
 
	// Load an XM mod into a soundbuffer
	CL_SoundBuffer mod("clanbeat.xm");
 
	// Play mod
	mod.play();

Resources

TODO: Write about loading samples from resources. 

Sessions

If you want to control the playback of sound, or want to know if the sound is still playing, you use CL_SoundBuffer_Session. This can be created in two ways, either by calling CL_SoundBuffer::play() or CL_SoundBuffer::prepare(). The difference between prepare() and play() is that prepare will just load the sound, but not play it. You can then call play() from your resulting CL_SoundBuffer_Session. 


 
	// Load ogg file into a soundbuffer
	CL_SoundBuffer vorbis("cheer1.ogg");
 
	// Create a session from soundbuffer.
	CL_SoundBuffer_Session playback = vorbis.play();
 
	..
 
	// Load ogg file into a soundbuffer
	CL_SoundBuffer vorbis("cheer1.ogg");
 
	// Create a session from soundbuffer.
	CL_SoundBuffer_Session playback = vorbis.prepare();
 
	playback.play();

When you have a session object, you can modify the attributes of the playing sound in many ways. These take effect immidiately, you can change it while it is playing. You can set the volume, panning, position, and frequency (speed). You can make it loop, and adding filters. You can query if it is still playing, and you can ofcourse start and stop it as you want. 


 
	// Load ogg file into a soundbuffer
	CL_SoundBuffer vorbis("cheer1.ogg");
 
	// Create a session from soundbuffer.
	CL_SoundBuffer_Session playback = vorbis.prepare();
 
	playback.set_volume(0.5f);
	playback.set_looping(true);
	playback.play();

Filters

If you want to modify sounds in special, dynamic ways, you can apply a CL_SoundFilter to it. 

ClanLib has three builtin filters: CL_EchoFilter, CL_FadeFilter and CL_InverseEchoFilter. The usefulness of an CL_InverseEchoFilter can be discussed :) 

Example of the fade filter: 


 
	// Load ogg file into a soundbuffer
	CL_SoundBuffer vorbis("cheer1.ogg");
 
	// Create a session from soundbuffer.
	CL_SoundBuffer_Session playback = vorbis.prepare();
 
	CL_FadeFilter fade(0.0f);
 
	playback.add_filter(&fade);
	playback.play();
 
	fade.fade_to_volume(1.0f, 10*1000); // fade in, duration 10 seconds.

Example of the echo filter: 


 
	// Load ogg file into a soundbuffer
	CL_SoundBuffer vorbis("cheer1.ogg");
 
	// Create a session from soundbuffer.
	CL_SoundBuffer_Session playback = vorbis.prepare();
 
	playback.add_filter(new CL_EchoFilter(8*1024), true);
	playback.play();

You can ofcourse also code your own filters. Check the SoundFilters example how do to that - it creates a CL_DistortFilter. 

It is also possible to add filters on mixer level too. Ie fade all sound out alltogether. Use CL_SoundOutput::add_filter() just like with CL_SoundBuffer_Session. 

Sound providers

It is also possible to create sound on-the-fly, or adding extra sound providers. See the StreamSoundProvider example for code how to do this. 


*/
