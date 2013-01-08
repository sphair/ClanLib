
#pragma once

#include <string>

class clan::InputSourceProvider;
class SoundProvider_MikMod_Impl;

/// \brief Module format (.mod, .s3m, .xm, etc) sound provider.
class SoundProvider_MikMod : public clan::SoundProvider
{
public:
	/// \brief Constructs a sound provider based on a MikMod compatible module file.
	///
	/// \param filename Filename of module file.
	/// \param provider Input source provider used to retrieve module file.
	/// \param stream If true, will stream from disk. If false, will load it to memory.
	SoundProvider_MikMod(
		const std::string &filename,
		const clan::VirtualDirectory &provider,
		bool stream = false);

	SoundProvider_MikMod(
		const std::string &fullname,
		bool stream = false);

	SoundProvider_MikMod(
		clan::IODevice &file,
		bool stream = false);

	~SoundProvider_MikMod();

	/// \brief Called by clan::SoundBuffer when a new session starts.
	///
	/// \return The soundbuffer session to be attached to the newly started session.
	clan::SoundProvider_Session *begin_session();

	/// \brief Called by clan::SoundBuffer when a session has finished.
	///
	/// After this call, clan::SoundBuffer will not access the session anymore. It can
	/// safely be deleted here (and in most cases should be delete here).
	void end_session(clan::SoundProvider_Session *session);

private:
	std::shared_ptr<SoundProvider_MikMod_Impl> impl;

	friend class SoundProvider_MikMod_Session;
};
