/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Magnus Norddahl
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/iodevice_provider.h"
#include "API/Core/IOData/cl_endian.h"
#include "iodevice_impl.h"

namespace clan
{
	IODevice::IODevice()
	{
	}

	IODevice::IODevice(IODeviceProvider *provider)
		: impl(std::make_shared<IODevice_Impl>())
	{
		impl->provider = provider;
	}

	IODevice::~IODevice()
	{
	}

	void IODevice::throw_if_null() const
	{
		if (!impl)
			throw Exception("IODevice is null");
	}

	size_t IODevice::get_size() const
	{
		if (impl)
			return impl->provider->get_size();
		return -1;
	}

	size_t IODevice::get_position() const
	{
		if (impl)
			return impl->provider->get_position();
		return -1;
	}

	bool IODevice::is_little_endian() const
	{
		return impl->little_endian_mode;
	}

	const IODeviceProvider *IODevice::get_provider() const
	{
		throw_if_null();
		return impl->provider;
	}

	IODeviceProvider *IODevice::get_provider()
	{
		throw_if_null();
		return impl->provider;
	}

	size_t IODevice::send(const void *data, size_t len, bool send_all)
	{
		if (impl)
			return impl->provider->send(data, len, send_all);
		return -1;
	}

	size_t IODevice::receive(void *data, size_t len, bool receive_all)
	{
		if (impl)
			return impl->provider->receive(data, len, receive_all);
		return -1;
	}

	size_t IODevice::peek(void *data, size_t len)
	{
		if (impl)
			return impl->provider->peek(data, len);
		return -1;
	}

	bool IODevice::seek(int position, SeekMode mode)
	{
		if (impl)
			return impl->provider->seek(position, mode);
		return false;
	}

	size_t IODevice::read(void *data, size_t len, bool receive_all)
	{
		return receive(data, len, receive_all);
	}

	size_t IODevice::write(const void *data, size_t len, bool send_all)
	{
		return send(data, len, send_all);
	}

	void IODevice::set_system_mode()
	{
		impl->little_endian_mode = !Endian::is_system_big();
	}

	void IODevice::set_big_endian_mode()
	{
		impl->little_endian_mode = false;
	}

	void IODevice::set_little_endian_mode()
	{
		impl->little_endian_mode = true;
	}

	void IODevice::write_int64(int64_t data)
	{
		int64_t final = data;
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&final, sizeof(final));
		}
		else
		{
			Endian::swap_if_little(&final, sizeof(final));
		}
		write(&final, sizeof(int64_t));
	}

	void IODevice::write_uint64(uint64_t data)
	{
		uint64_t final = data;
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&final, sizeof(final));
		}
		else
		{
			Endian::swap_if_little(&final, sizeof(final));
		}
		write(&final, sizeof(uint64_t));
	}

	void IODevice::write_int32(int32_t data)
	{
		int32_t final = data;
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&final, sizeof(final));
		}
		else
		{
			Endian::swap_if_little(&final, sizeof(final));
		}
		write(&final, sizeof(int32_t));
	}

	void IODevice::write_uint32(uint32_t data)
	{
		uint32_t final = data;
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&final, sizeof(final));
		}
		else
		{
			Endian::swap_if_little(&final, sizeof(final));
		}
		write(&final, sizeof(uint32_t));
	}

	void IODevice::write_int16(int16_t data)
	{
		int16_t final = data;
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&final, sizeof(final));
		}
		else
		{
			Endian::swap_if_little(&final, sizeof(final));
		}
		write(&final, sizeof(int16_t));
	}

	void IODevice::write_uint16(uint16_t data)
	{
		uint16_t final = data;
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&final, sizeof(final));
		}
		else
		{
			Endian::swap_if_little(&final, sizeof(final));
		}
		write(&final, sizeof(uint16_t));
	}

	void IODevice::write_int8(int8_t data)
	{
		write(&data, sizeof(int8_t));
	}

	void IODevice::write_uint8(uint8_t data)
	{
		write(&data, sizeof(uint8_t));
	}

	void IODevice::write_float(float data)
	{
		float final = data;
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&final, sizeof(final));
		}
		else
		{
			Endian::swap_if_little(&final, sizeof(final));
		}
		write(&final, sizeof(float));
	}

	void IODevice::write_string_a(const std::string &str)
	{
		size_t size = str.length();
		write_int32(int32_t(size));
		write(str.data(), size);
	}

	void IODevice::write_string_nul(const std::string &str)
	{
		write(str.c_str(), str.length() + 1);
	}

	void IODevice::write_string_text(const std::string &str)
	{
		write(str.data(), str.length());
#ifdef WIN32
		write("\r\n", 2);
#else
		write("\n", 1);
#endif
	}

	int64_t IODevice::read_int64()
	{
		int64_t answer;
		if (read(&answer, sizeof(int64_t)) != sizeof(int64_t)) throw Exception("IODevice_Datafile::read_int64() failed");
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&answer, sizeof(answer));
		}
		else
		{
			Endian::swap_if_little(&answer, sizeof(answer));
		}
		return answer;
	}

	uint64_t IODevice::read_uint64()
	{
		uint64_t answer;
		if (read(&answer, sizeof(uint64_t)) != sizeof(uint64_t)) throw Exception("IODevice_Datafile::read_uint64() failed");
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&answer, sizeof(answer));
		}
		else
		{
			Endian::swap_if_little(&answer, sizeof(answer));
		}
		return answer;
	}

	int32_t IODevice::read_int32()
	{
		int32_t answer;
		if (read(&answer, sizeof(int32_t)) != sizeof(int32_t)) throw Exception("IODevice_Datafile::read_int32() failed");
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&answer, sizeof(answer));
		}
		else
		{
			Endian::swap_if_little(&answer, sizeof(answer));
		}
		return answer;
	}

	uint32_t IODevice::read_uint32()
	{
		uint32_t answer;
		if (read(&answer, sizeof(uint32_t)) != sizeof(uint32_t)) throw Exception("IODevice_Datafile::read_uint32() failed");
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&answer, sizeof(answer));
		}
		else
		{
			Endian::swap_if_little(&answer, sizeof(answer));
		}
		return answer;
	}

	int16_t IODevice::read_int16()
	{
		int16_t answer;
		if (read(&answer, sizeof(int16_t)) != sizeof(int16_t)) throw Exception("IODevice_Datafile::read_int16() failed");
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&answer, sizeof(answer));
		}
		else
		{
			Endian::swap_if_little(&answer, sizeof(answer));
		}
		return answer;
	}

	uint16_t IODevice::read_uint16()
	{
		uint16_t answer;
		if (read(&answer, sizeof(uint16_t)) != sizeof(uint16_t)) throw Exception("IODevice_Datafile::read_uint16() failed");
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&answer, sizeof(answer));
		}
		else
		{
			Endian::swap_if_little(&answer, sizeof(answer));
		}
		return answer;
	}

	int8_t IODevice::read_int8()
	{
		int8_t answer;
		if (read(&answer, sizeof(int8_t)) != sizeof(int8_t)) throw Exception("IODevice_Datafile::read_int8() failed");
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&answer, sizeof(answer));
		}
		else
		{
			Endian::swap_if_little(&answer, sizeof(answer));
		}
		return answer;
	}

	uint8_t IODevice::read_uint8()
	{
		uint8_t answer;
		if (read(&answer, sizeof(uint8_t)) != sizeof(uint8_t)) throw Exception("IODevice_Datafile::read_uint8() failed");
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&answer, sizeof(answer));
		}
		else
		{
			Endian::swap_if_little(&answer, sizeof(answer));
		}
		return answer;
	}

	float IODevice::read_float()
	{
		float answer;
		if (read(&answer, sizeof(float)) != sizeof(float)) throw Exception("IODevice_Datafile::read_float() failed");
		if (impl->little_endian_mode)
		{
			Endian::swap_if_big(&answer, sizeof(answer));
		}
		else
		{
			Endian::swap_if_little(&answer, sizeof(answer));
		}
		return answer;
	}

	std::string IODevice::read_string_a()
	{
		size_t size = size_t(read_int32());

		auto str = new char[size];
		try
		{
			read(str, size);

			std::string ret(str, size);
			delete[] str;

			return ret;
		}
		catch (...)
		{
			delete[] str;
			throw;
		}
	}

	std::string IODevice::read_string_nul()
	{
		return (read_string_text(nullptr, nullptr, false));
	}

	std::string IODevice::read_string_text(const char *skip_initial_chars, const char *read_until_chars, bool allow_eof)
	{
		const size_t buffer_size = 64;
		char buffer[buffer_size];
		size_t read_size;
		size_t size = 0;
		bool find_flag = true;
		bool null_found = false;
		size_t current_position = get_position();

		// Skip initial unwanted chars
		if (skip_initial_chars)
		{
			while (find_flag)	// For the entire file
			{
				read_size = receive(buffer, buffer_size, true);

				char *dptr = buffer;
				for (size_t cnt = 0; cnt != read_size; ++cnt, dptr++)	// Search the buffer
				{
					char letter = *dptr;
					bool match_flag = false;
					for (const char *rptr = skip_initial_chars; *rptr; rptr++)	// Search the initial chars array
					{
						if (letter == *rptr)
						{
							match_flag = true;
							break;
						}
					}
					if (!match_flag)	// Not found a match
					{
						find_flag = false;
						break;
					}
					current_position++;	// Skip the char		
				}

				if ((find_flag) && (read_size != buffer_size))	// eof found
				{
					if (!allow_eof)
					{
						throw Exception("IODevice_Datafile::read_string_text() failed - end of file");
					}
					break;
				}
			}

			seek(int(current_position), SeekMode::set);	// Set new position
		}

		find_flag = true;

		// Calculate the length of the string
		while (find_flag)
		{
			read_size = receive(buffer, buffer_size, true);
			char *dptr = buffer;
			for (size_t cnt = 0; cnt != read_size; ++cnt, dptr++)
			{
				char letter = *dptr;
				// Treat NUL as a special terminating character
				if (letter == 0)
				{
					null_found = true;
					find_flag = false;
					break;
				}
				bool match_flag = false;
				if (read_until_chars)
				{
					for (const char *rptr = read_until_chars; *rptr; rptr++)	// Search the read until chars array
					{
						if (letter == *rptr)
						{
							match_flag = true;
							break;
						}
					}
					if (match_flag)	// Found a match
					{
						find_flag = false;
						break;
					}
				}

				size++;
			}

			if ((find_flag) && (read_size != buffer_size))	// eof found
			{
				if (!allow_eof)
				{
					throw Exception("IODevice_Datafile::read_string_text() failed - end of file");
				}
				break;
			}
		}
		seek(int(current_position), SeekMode::set);

		// Read the string, now that we know its length

		auto str = new char[size];
		try
		{
			read(str, size);
			std::string ret(str, size);
			if (null_found)
			{
				read(buffer, 1);	// Read the NUL terminator
			}
			delete[] str;

			return ret;
		}
		catch (...)
		{
			delete[] str;
			throw;
		}
	}

	IODevice IODevice::duplicate()
	{
		return IODevice(impl->provider->duplicate());
	}
}
