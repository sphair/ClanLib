
/*! \page Sound Sound Module Reference

 <h2>Basic Sound</h2>

 <ul>
  <li>clan::SoundOutput - Connects to a sound output device</li>
  <li>clan::SoundOutput_Description - Allows you to setup a more advanced description when creating a sound output</li>
  <li>clan::SoundBuffer - A sound that can be played</li>
  <li>clan::SoundBuffer_Session - A sound being played</li>
  <li>clan::SoundFilter - Filter effect applied to the mixing stream</li>
 </ul>

 <h2>Sound Sources and Formats</h2>

 <ul>
  <li>clan::SoundProvider - Interface to sound sources</li>
  <li>clan::SoundProvider_Session - Interface to a playing sound source</li>
  <li>clan::SoundProviderFactory, clan::SoundProviderType - Create sound buffers from registered sound provider types</li>
  <li>clan::SoundProvider_Vorbis - Ogg Vorbis (.ogg) sound source</li>
  <li>clan::SoundProvider_Wave - Windows WAVE (.wav) sound source</li>
  <li>clan::SoundProvider_Raw - Raw PCM format sound source</li>
 </ul>

 <h2>Sound Filters</h2>

 <ul>
  <li>clan::EchoFilter - Echo effect</li>
  <li>clan::FadeFilter - Fades sound in or out</li>
  <li>clan::InverseEchoFilter - Inverse echo effect</li>
 </ul>

 <h2>Positional Audio</h2>

 <ul>
  <li>clan::AudioWorld - Main class for positional audio</li>
  <li>clan::AudioObject - Sound object placed in the audio world</li>
  <li>clan::AudioDefinition - Describes a sound object</li>
 </ul>

 <h2>Resources</h2>

 <ul>
  <li>clan::SoundCache - Resource cache interface for clanSound resources</li>
 </ul>

*/
