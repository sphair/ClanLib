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
**    Animehunter
*/

#include "Core/precomp.h"
#include "API/Core/System/system.h"

namespace clan
{

#if defined(ARM_PLATFORM) || defined(CL_ARM) || defined(__sun)
	bool System::detect_cpu_extension(CPU_ExtensionPPC ext)
	{
		throw ("Congratulations, you've just been selected to code this feature!");
	}

	bool System::detect_cpu_extension(CPU_ExtensionX86 ext)
	{
		return false;
	}
#else

#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64)) && !defined __MINGW32__
#include <intrin.h>
#endif
#ifdef __GNUC__


#ifdef __amd64__

#define __cpuid(out, infoType)\
	asm("cpuid": "=a" ((out)[0]), "=b" ((out)[1]), "=c" ((out)[2]), "=d" ((out)[3]): "a" (infoType));
#else

#define __cpuid(out, infoType) \
	asm volatile(	"pushl %%ebx \n" \
			"cpuid \n" \
			"movl %%ebx, %1 \n" \
			"popl %%ebx" \
		: "=a" ((out)[0]), "=r" ((out)[1]), "=c" ((out)[2]), "=d" ((out)[3]): "a" (infoType));

#endif

#endif

	bool System::detect_cpu_extension(CPU_ExtensionPPC ext)
	{
		throw ("Congratulations, you've just been selected to code this feature!");
	}

	bool System::detect_cpu_extension(CPU_ExtensionX86 ext)
	{
		unsigned int cpuinfo[4] = { 0 };

		if (ext == mmx)
		{
			__cpuid((int*)cpuinfo, 0x1);
			return ((cpuinfo[3] & (1 << 23)) != 0);
		}
		else if (ext == mmx_ex)
		{
			__cpuid((int*)cpuinfo, 0x80000000);
			if (cpuinfo[0] < 0x80000001)
				return false;

			__cpuid((int*)cpuinfo, 0x80000001);
			return ((cpuinfo[3] & (1 << 22)) != 0);
		}
		else if (ext == _3d_now)
		{
			__cpuid((int*)cpuinfo, 0x80000000);
			if (cpuinfo[0] < 0x80000001)
				return false;

			__cpuid((int*)cpuinfo, 0x80000001);
			return ((cpuinfo[3] & (1 << 31)) != 0);
		}
		else if (ext == _3d_now_ex)
		{
			__cpuid((int*)cpuinfo, 0x80000000);
			if (cpuinfo[0] < 0x80000001)
				return false;

			__cpuid((int*)cpuinfo, 0x80000001);
			return ((cpuinfo[3] & (1 << 30)) != 0);
		}
		else if (ext == sse)
		{
			__cpuid((int*)cpuinfo, 0x1);
			return ((cpuinfo[3] & (1 << 25)) != 0);
		}
		else if (ext == sse2)
		{
			__cpuid((int*)cpuinfo, 0x1);
			return ((cpuinfo[3] & (1 << 26)) != 0);
		}
		else if (ext == sse3)
		{
			__cpuid((int*)cpuinfo, 0x1);
			return ((cpuinfo[2] & (1 << 0)) != 0);
		}
		else if (ext == ssse3)
		{
			__cpuid((int*)cpuinfo, 0x1);
			return ((cpuinfo[2] & (1 << 9)) != 0);
		}

		else if (ext == sse4_a)
		{
			__cpuid((int*)cpuinfo, 0x80000000);
			if (cpuinfo[0] < 0x80000001)
				return false;

			__cpuid((int*)cpuinfo, 0x80000001);
			return ((cpuinfo[2] & (1 << 6)) != 0);
		}
		else if (ext == sse4_1)
		{
			__cpuid((int*)cpuinfo, 0x1);
			return ((cpuinfo[2] & (1 << 19)) != 0);
		}
		else if (ext == sse4_2)
		{
			__cpuid((int*)cpuinfo, 0x1);
			return ((cpuinfo[2] & (1 << 20)) != 0);
		}
		else if (ext == xop)
		{
			__cpuid((int*)cpuinfo, 0x80000000);
			if (cpuinfo[0] < 0x80000001)
				return false;

			__cpuid((int*)cpuinfo, 0x80000001);
			return ((cpuinfo[2] & (1 << 11)) != 0);
		}
		else if (ext == avx)
		{
			__cpuid((int*)cpuinfo, 0x1);
			return ((cpuinfo[2] & (1 << 28)) != 0);
		}
		else if (ext == aes)
		{
			__cpuid((int*)cpuinfo, 0x1);
			return ((cpuinfo[2] & (1 << 25)) != 0);
		}
		else if (ext == fma3)
		{
			__cpuid((int*)cpuinfo, 0x1);
			return ((cpuinfo[2] & (1 << 12)) != 0);
		}
		else if (ext == fma4)
		{
			__cpuid((int*)cpuinfo, 0x80000000);
			if (cpuinfo[0] < 0x80000001)
				return false;

			__cpuid((int*)cpuinfo, 0x80000001);
			return ((cpuinfo[2] & (1 << 16)) != 0);
		}
		return false;
	}

#endif

}
