
/*! \page SoundOverview Sound Overview

*** THIS PAGE REQUIRES UPDATING FOR CLANLIB 3.0 ***
<br/>

ClanSound provides you with easy-to-use and powerful sound functionality. 


<h2>Sound overview</h2>

To play sound in ClanLib, you have to initialize \ref clan::SetupSound and also create a \ref clan::SoundOutput. \ref clan::SoundOutput is the interface to a sound output device, which is used to control the main mixer volume and other global settings. It is used as a singleton. 


\code 
	#include <ClanLib/sound.h> 
 
	...
 
	clan::SetupSound setup_sound;
	clan::SoundOutput output(44100);
\endcode
	...

<hr>Playing wav samples</h2>

A \ref clan::SoundBuffer contains a sample or music stream. To load and play a WAV file, you do the following: 


\code 
	// Load a sample from a wave file:
	clan::SoundBuffer sample("beep.wav");
 
	// Play sample
	sample.play();
\endcode

You can set the volume and panning of soundbuffers. This will change the default attributes of the soundbuffer, and will be effective on all subsequent calls to play(). 


\code 
	// Turn down volume on this soundbuffer.
	sample.set_volume(0.1f);
 
	// Play only in left speaker.
	sample.set_pan(-1.0f);
\endcode

Note that setting these attributes don't affect already playing soundbuffers, only when you call play() again afterwards. To have individual control over each playing sound, you need to use a sound session - read about that further down. 

<h2>Playing music streams</h2>

There isn't much difference between playing wav samples and music stream. ClanSound itself doesn't provide more than wav samples playback, so you'll need one or more additional music modules. ClanLib currently has two, MikMod and Vorbis. They are initialized using \ref clan::SetupMikMod and \ref clan::SetupVorbis. 

\code
 	#include <ClanLib/sound.h> 
	#include <ClanLib/vorbis.h> 
	#include <ClanLib/mikmod.h> 
 
	clan::SetupSound setup_sound;
	clan::SetupMikMod setup_mikmod;
	clan::SetupVorbis setup_vorbis;
 
	clan::SoundOutput output(44100);
\endcode

Then to play music, you use the same approach as with wav samples: 


\code 
	// Load ogg file into a soundbuffer
	clan::SoundBuffer vorbis("cheer1.ogg");
 
	// Play vorbis stream
	vorbis.play();
 
	...
 
	// Load an XM mod into a soundbuffer
	clan::SoundBuffer mod("clanbeat.xm");
 
	// Play mod
	mod.play();
\endcode

<h2>Resources</h2>

TODO: Write about loading samples from resources. 

<h2>Sessions</h2>

If you want to control the playback of sound, or want to know if the sound is still playing, you use \ref clan::SoundBuffer_Session. This can be created in two ways, either by calling \ref clan::SoundBuffer::play() or \ref clan::SoundBuffer::prepare(). The difference between prepare() and play() is that prepare will just load the sound, but not play it. You can then call play() from your resulting \ref clan::SoundBuffer_Session. 


 \code
	// Load ogg file into a soundbuffer
	clan::SoundBuffer vorbis("cheer1.ogg");
 
	// Create a session from soundbuffer.
	clan::SoundBuffer_Session playback = vorbis.play();
 
	..
 
	// Load ogg file into a soundbuffer
	clan::SoundBuffer vorbis("cheer1.ogg");
 
	// Create a session from soundbuffer.
	clan::SoundBuffer_Session playback = vorbis.prepare();
 
	playback.play();
\endcode

When you have a session object, you can modify the attributes of the playing sound in many ways. These take effect immidiately, you can change it while it is playing. You can set the volume, panning, position, and frequency (speed). You can make it loop, and adding filters. You can query if it is still playing, and you can ofcourse start and stop it as you want. 

\code
 
	// Load ogg file into a soundbuffer
	clan::SoundBuffer vorbis("cheer1.ogg");
 
	// Create a session from soundbuffer.
	clan::SoundBuffer_Session playback = vorbis.prepare();
 
	playback.set_volume(0.5f);
	playback.set_looping(true);
	playback.play();
\endcode

<h2>Filters</h2>

If you want to modify sounds in special, dynamic ways, you can apply a \ref clan::SoundFilter to it. 

ClanLib has three builtin filters: \ref clan::EchoFilter, \ref clan::FadeFilter and \ref clan::InverseEchoFilter. The usefulness of an \ref clan::InverseEchoFilter can be discussed :) 

Example of the fade filter: 

\code
 
	// Load ogg file into a soundbuffer
	clan::SoundBuffer vorbis("cheer1.ogg");
 
	// Create a session from soundbuffer.
	clan::SoundBuffer_Session playback = vorbis.prepare();
 
	clan::FadeFilter fade(0.0f);
 
	playback.add_filter(&fade);
	playback.play();
 
	fade.fade_to_volume(1.0f, 10*1000); // fade in, duration 10 seconds.
\endcode

Example of the echo filter: 


\code 
	// Load ogg file into a soundbuffer
	clan::SoundBuffer vorbis("cheer1.ogg");
 
	// Create a session from soundbuffer.
	clan::SoundBuffer_Session playback = vorbis.prepare();
 
	playback.add_filter(new clan::EchoFilter(8*1024), true);
	playback.play();
\endcode

You can ofcourse also code your own filters. Check the SoundFilters example how do to that - it creates a \ref clan::DistortFilter. 

It is also possible to add filters on mixer level too. Ie fade all sound out alltogether. Use \ref clan::SoundOutput::add_filter() just like with \ref clan::SoundBuffer_Session. 

<h2>Sound providers</h2>

It is also possible to create sound on-the-fly, or adding extra sound providers. See the StreamSoundProvider example for code how to do this. 


*/
