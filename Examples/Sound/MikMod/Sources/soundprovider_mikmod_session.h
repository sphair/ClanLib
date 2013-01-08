
#pragma once

#include "soundprovider_mikmod.h"
#include "module_reader.h"

class SoundProvider_MikMod_Impl;
class clan::IODevice;

class SoundProvider_MikMod_Session : public clan::SoundProvider_Session
{
public:
	SoundProvider_MikMod_Session(SoundProvider_MikMod &source);
	~SoundProvider_MikMod_Session();

	int get_num_samples() const;
	int get_frequency() const;
	int get_num_channels() const;
	int get_position() const;

	bool set_looping(bool loop);
	bool eof() const;
	void stop();
	bool play();
	bool set_position(int pos);
	bool set_end_position(int pos) { return false; }
	int get_data(float **data_ptr, int data_requested);

private:
	SoundProvider_MikMod source;
	clan::SoundFormat format;
	int num_channels;
	int num_samples;
	int position;
	bool stream_eof;
	int frequency;
	MODULE *module;
	SBYTE buffer[64*1024];
};
