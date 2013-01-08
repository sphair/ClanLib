
#pragma once

#if defined(_MSC_VER)
	#if !defined(_MT)
		#error Your application is set to link with the single-threaded version of the run-time library. Go to project settings, in the C++ section, and change it to multi-threaded.
	#endif
	#if !defined(_DEBUG)
		#if defined(CL_DLL)
			#pragma comment(lib, "mikmod-dll.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "mikmod-static-mtdll.lib")
		#else
			#pragma comment(lib, "mikmod-static-mt.lib")
		#endif
	#else
		#if defined(CL_DLL)
			#pragma comment(lib, "mikmod-dll-debug.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "mikmod-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "mikmod-static-mt-debug.lib")
		#endif
	#endif
#endif

/// \brief ClanMikMod initialization.
class SetupMikMod
{
public:
	/// \brief Initializes clanMikMod.
	///
	/// Constructing a SetupMikMod object is equalent to calling SetupMikMod::init().
	///
	/// When the SetupMikMod instance is destroyed, SetupMikMod::deinit() is called.
	SetupMikMod(bool register_resources_only = false);
	~SetupMikMod();

	/// \brief Call this function in init_modules() to use clanMikMod.
	static void init(bool register_resources_only = false);

	/// \brief Call this function in deinit_modules().
	static void deinit();
};
