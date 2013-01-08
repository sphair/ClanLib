
#pragma once

#include <cstring>

class clan::InputSourceProvider;
class clan::IODevice;

class SoundProvider_MikMod_Impl
{
public:
	void load(clan::IODevice &input);

	clan::DataBuffer buffer;
};


