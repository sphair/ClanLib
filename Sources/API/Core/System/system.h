/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Animehunter
*/

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once


#include "../api_core.h"
#include "../Text/string_types.h"
#include <vector>

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
struct CL_API_CORE CL_PreallocatedMemory
{
	int dummy;
};

class CL_Mutex;

/// \brief General system helper functions.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_System
{
/// \name Attributes
/// \{

public:
	/// \brief Get the current time (since system boot), in milliseconds.
	static unsigned int get_time();

    enum CL_CPU_ExtensionX86 { mmx, mmx_ex, _3d_now, _3d_now_ex, sse, sse2, sse3, ssse3, sse4_a, sse4_1, sse4_2, sse5 };
    enum CL_CPU_ExtensionPPC { altivec };

    static bool detect_cpu_extension(CL_CPU_ExtensionX86 ext);
    static bool detect_cpu_extension(CL_CPU_ExtensionPPC ext);

	/// \brief Returns the mutex used by CL_SharedPtr.
	static CL_Mutex *get_sharedptr_mutex();

	/// \brief Return the number of CPU cores
	static int get_num_cores();


/// \}
/// \name Operations
/// \{

public:
	/// \brief Allocates aligned memory
	static void *aligned_alloc(size_t size, size_t alignment = 16);

	/// \brief Frees aligned memory
	static void aligned_free(void *ptr);

	/// \brief Captures a stack back trace by walking up the stack and recording the information for each frame
	static int capture_stack_trace(int frames_to_skip, int max_frames, void **out_frames, unsigned int *out_hash = 0);

	/// \brief Returns the function names and lines for the specified stack frame addresses
	/** <p>On Linux, to obtain function names, remember to link with the -rdynamic flag </p>*/
	static std::vector<CL_String> get_stack_frames_text(void **frames, int num_frames);

	/// \brief Sleep for 'millis' milliseconds.
	static void sleep(int millis);

	/// \brief Returns the full dirname of the executable that started this
	/// \brief process (aka argv[0])
	/** <p>This is necessary since when programms are started from
	    the PATH there is no clean and simple way to figure out
	    the location of the data files, thus information is read
	    from <tt>/proc/self/exe</tt> under GNU/Linux and from
	    GetModuleFileName() on Win32.</p>
	    \return full dirname of the executable, trailing slash is included*/
	static CL_String get_exe_path();

	/// \brief Calls the constructor of a class.
	template<typename T>
	static void call_constructor(T *memory)
	{
		new ((CL_PreallocatedMemory *) memory) T;
	}

	template<typename T, typename P1>
	static void call_constructor(T *memory, P1 p1)
	{
		new ((CL_PreallocatedMemory *) memory) T(p1);
	}

	template<typename T, typename P1, typename P2>
	static void call_constructor(T *memory, P1 p1, P2 p2)
	{
		new ((CL_PreallocatedMemory *) memory) T(p1, p2);
	}

	template<typename T, typename P1, typename P2, typename P3>
	static void call_constructor(T *memory, P1 p1, P2 p2, P3 p3)
	{
		new ((CL_PreallocatedMemory *) memory) T(p1, p2, p3);
	}

	template<typename T, typename P1, typename P2, typename P3, typename P4>
	static void call_constructor(T *memory, P1 p1, P2 p2, P3 p3, P4 p4)
	{
		new ((CL_PreallocatedMemory *) memory) T(p1, p2, p3, p4);
	}

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
	static void call_constructor(T *memory, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		new ((CL_PreallocatedMemory *) memory) T(p1, p2, p3, p4, p5);
	}

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static void call_constructor(T *memory, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
	{
		new ((CL_PreallocatedMemory *) memory) T(p1, p2, p3, p4, p5, p6);
	}

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static void call_constructor(T *memory, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
	{
		new ((CL_PreallocatedMemory *) memory) T(p1, p2, p3, p4, p5, p6, p7);
	}

	/// \brief Calls the destructor of a class.
	template<typename T>
	static void call_destructor(T *memory)
	{
		memory->~T();
	}

	static void alloc_thread_temp_pool();

	static void free_thread_temp_pool();


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


/// \}
