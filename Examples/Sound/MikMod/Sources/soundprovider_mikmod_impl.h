
#pragma once

#include <cstring>

namespace clan {
class InputSourceProvider;
class IODevice;
}

class SoundProvider_MikMod_Impl
{
public:
	void load(clan::IODevice &input);

	clan::DataBuffer buffer;
};


