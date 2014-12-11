
#include "Network/precomp.h"
#include "ring_buffer.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{

RingBuffer::RingBuffer()
: data(nullptr), size(0), pos(0), length(0)
{
}

RingBuffer::RingBuffer(size_t size)
: data(new char[size]), size(size), pos(0), length(0)
{
	for (size_t i = 0; i < size; i++)
		data[i] = 0;
}

RingBuffer::RingBuffer(const RingBuffer &other)
: data(new char[other.size]), size(other.size), pos(other.pos), length(other.length)
{
	for (size_t i = 0; i < size; i++)
		data[i] = other.data[i];
}

RingBuffer::~RingBuffer()
{
	delete[] data;
}

const char *RingBuffer::get_read_pos()
{
	return data + pos;
}

size_t RingBuffer::get_read_size()
{
	size_t end_pos = min(pos + length, size);
	return end_pos - pos;
}

char *RingBuffer::get_write_pos()
{
	size_t end_pos = pos + length;
	if (end_pos >= size)
		end_pos -= size;
	return data + end_pos;
}

size_t RingBuffer::get_write_size()
{
	size_t end_pos = pos + length;
	if (end_pos >= size)
		end_pos -= size;

	if (pos < end_pos)
		return end_pos - pos;
	else
		return size - end_pos;
}

void RingBuffer::write(size_t written_length)
{
	length += written_length;
}

void RingBuffer::read(size_t read_length)
{
	pos += read_length;
	length -= read_length;
	if (pos >= size)
		pos -= size;
}

std::string RingBuffer::read_to_string(size_t length)
{
	std::string s(length, ' ');
	size_t bytes_read = 0;
	while (bytes_read < length)
	{
		const char *d = get_read_pos();
		size_t available = min(length - bytes_read, get_read_size());
		for (size_t i = 0; i < available; i++)
			s[bytes_read + i] = d[i];
		bytes_read += available;
		read(available);
	}
	return s;
}

size_t RingBuffer::find(const char *search_data, size_t search_size)
{
	if (search_size <= length)
	{
		for (size_t i = pos; i <= pos+length-search_size; i++)
		{
			bool found = true;
			for (size_t j = 0; j < search_size; j++)
			{
				size_t index = (i+j) >= size ? (i+j-size) : (i+j);
				if (data[index] != search_data[j])
				{
					found = false;
					break;
				}
			}
			if (found)
				return i-pos;
		}
	}
	return npos;
}

RingBuffer &RingBuffer::operator=(const RingBuffer &other)
{
	if (this != &other)
	{
		char *new_data = new char[other.size];
		delete[] data;
		data = new_data;
		size = other.size;
		pos = other.pos;
		length = other.length;
		for (size_t i = 0; i < size; i++)
			data[i] = other.data[i];
	}
	return *this;
}

}
