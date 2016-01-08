/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

void TestApp::test_iodevice_memory(void)
{
	Console::write_line(" Header: memory_device.h");
	Console::write_line("  Class: MemoryDevice");

	std::string str;
	std::string str2;
	size_t buffer_size = 200;
	DataBuffer data(buffer_size);
	MemoryDevice mem(data);
	const size_t test_data_size = 256;
	char test_data[test_data_size];
	char test_data2[test_data_size];

	for (int cnt=0; cnt<test_data_size; cnt++)
	{
		test_data[cnt] = cnt;
	}

//*** testing get_data()
	Console::write_line("   Function: DataBuffer &MemoryDevice::get_data()");
	if (mem.get_data().get_data() != data.get_data()) fail();

//*** testing get_size()
	Console::write_line("   Function: int get_size()");
	if (mem.get_size() != buffer_size) fail();

//*** testing get_position()
	Console::write_line("   Function: int get_position()");
	if (mem.get_position() != 0) fail();
	mem.seek(123);
	if (mem.get_position() != 123) fail();
	mem.seek(0);

//*** testing int send()
	Console::write_line("   Function: int send(const void *data, int len, bool send_all = true)");
	if (mem.send(test_data, buffer_size, true) != buffer_size) fail();
	if (memcmp(data.get_data(), test_data, buffer_size)) fail();
	if (mem.get_position() != buffer_size) fail();
	mem.seek(buffer_size);
	if (mem.send(test_data, 1, true) != 1) fail();
	int extended_buffer_size = buffer_size + 1;
	if (mem.get_position() != extended_buffer_size) fail();
	mem.seek(0);


//*** testing int receive()
	Console::write_line("   Function: int receive(void *data, int len, bool receive_all = true)");
	memset(test_data2, 0, sizeof(test_data2));
	if (mem.receive(test_data2, buffer_size, true) != buffer_size) fail();
	if (memcmp(test_data2, test_data, buffer_size)) fail();
	if (mem.get_position() != buffer_size) fail();
	mem.seek(extended_buffer_size);
	if (mem.receive(test_data2, 1, true) != 0) fail();
	mem.seek(0);

//*** testing int peek()
	Console::write_line("   Function: int peek(void *data, int len)");
	memset(test_data2, 0, sizeof(test_data2));
	if (mem.peek(test_data2, buffer_size) != buffer_size) fail();
	if (memcmp(test_data2, test_data, buffer_size)) fail();
	if (mem.get_position() != 0) fail();
	mem.seek(extended_buffer_size);
	if (mem.peek(test_data2, 1) != 0) fail();
	mem.seek(0);


//*** testing int seek()
	Console::write_line("   Function: bool seek(int position, SeekMode mode = seek_set)");
	memset(test_data2, 0, sizeof(test_data2));
	if (mem.get_position() != 0) fail();
	if (!mem.seek(123)) fail();
	if (mem.get_position() != 123) fail();
	if (!mem.seek(7, IODevice::seek_cur)) fail();
	if (mem.get_position() != 130) fail();
	if (!mem.seek(-7, IODevice::seek_end)) fail();
	if (mem.get_position() != extended_buffer_size-7) fail();
	if (mem.seek(-1)) fail();
	if (mem.seek(extended_buffer_size+1)) fail();
	if (!mem.seek(0)) fail();

}

