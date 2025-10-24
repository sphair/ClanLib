
#pragma once

class CL_RingBuffer
{
public:
	CL_RingBuffer();
	CL_RingBuffer(size_t size);
	CL_RingBuffer(const CL_RingBuffer &other);
	~CL_RingBuffer();
	CL_RingBuffer &operator=(const CL_RingBuffer &other);

	const char *get_read_pos();
	size_t get_read_size();
	char *get_write_pos();
	size_t get_write_size();
	void write(size_t length);
	void read(size_t length);
	CL_String read_to_string(size_t length);
	size_t find(const char *data, size_t size);

	static const size_t npos = (size_t)(-1);

private:
	char *data;
	size_t size;
	size_t pos, length;
};
