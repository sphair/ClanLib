
#include "precomp.h"
#include "soundprovider_mikmod_impl.h"
#include "soundprovider_mikmod_session.h"

SoundProvider_MikMod::SoundProvider_MikMod(
	const std::string &filename,
	const clan::VirtualDirectory &directory,
	bool stream)
: impl(new SoundProvider_MikMod_Impl)
{
	clan::VirtualDirectory new_directory = directory;
	clan::IODevice input = new_directory.open_file(filename, clan::File::open_existing, clan::File::access_read, clan::File::share_all);
	impl->load(input);
}

SoundProvider_MikMod::SoundProvider_MikMod(
	const std::string &fullname, bool stream)
: impl(new SoundProvider_MikMod_Impl)
{
	std::string path = clan::PathHelp::get_fullpath(fullname, clan::PathHelp::path_type_file);
	std::string filename = clan::PathHelp::get_filename(fullname, clan::PathHelp::path_type_file);
	clan::VirtualFileSystem vfs(path);
	clan::VirtualDirectory dir = vfs.get_root_directory();
	clan::IODevice input = dir.open_file(filename, clan::File::open_existing, clan::File::access_read, clan::File::share_all);
	impl->load(input);
}

SoundProvider_MikMod::SoundProvider_MikMod(
	clan::IODevice &file, bool stream)
: impl(new SoundProvider_MikMod_Impl)
{
	impl->load(file);
}

SoundProvider_MikMod::~SoundProvider_MikMod()
{
}

clan::SoundProvider_Session *SoundProvider_MikMod::begin_session()
{
	return new SoundProvider_MikMod_Session(*this);
}

void SoundProvider_MikMod::end_session(clan::SoundProvider_Session *session)
{
	delete session;
}

void SoundProvider_MikMod_Impl::load(clan::IODevice &input)
{
	int size = input.get_size();
	buffer = clan::DataBuffer(size);
	int bytes_read = input.read(buffer.get_data(), buffer.get_size());
	buffer.set_size(bytes_read);
}
