
#pragma once

namespace clan
{

class RingBuffer
{
public:
	RingBuffer();
	RingBuffer(size_t size);
	RingBuffer(const RingBuffer &other);
	~RingBuffer();
	RingBuffer &operator=(const RingBuffer &other);

	const char *get_read_pos();
	size_t get_read_size();
	char *get_write_pos();
	size_t get_write_size();
	void write(size_t length);
	void read(size_t length);
	std::string read_to_string(size_t length);
	size_t find(const char *data, size_t size);

	static const size_t npos = (size_t)(-1);

private:
	char *data;
	size_t size;
	size_t pos, length;
};

}
