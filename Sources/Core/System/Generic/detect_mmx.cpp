/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Darryl Agostinelli March 4, 2000
**    Code was adopted from Intel web page
*/

#include "Core/precomp.h"
#include "API/Core/System/system.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/log.h"

static bool do_mmx_test();

bool CL_System::detect_mmx()
{
	static bool has_mmx = false;
	static bool first_time = true;
	
	if (first_time)
	{
		first_time = false;
		has_mmx = do_mmx_test();
	}
	
	return has_mmx;
}

static bool do_mmx_test()
{
#ifdef USE_I386_ASSEMBLER
#ifdef __MSC__
	unsigned long RegEDX;

	try 
	{
		_asm
		{
			mov eax, 1			// set up CPUID to return processor version and features
								//	0 = vendor string, 1 = version info, 2 = cache info
			CPUID				// code bytes = 0fh,  0a2h
			mov RegEDX, edx		// features returned in edx
		}
	}
	catch(...)					// catch everything
	{
		return false;			// processor does not support CPUID
	}

	if (RegEDX & 0x800000) 		// bit 23 is set for MMX technology
	{
		try { _asm {emms} }		// try executing the MMX instruction "emms"
		catch(...) { return false; }
	}
	else
		return false;			// processor supports CPUID but does not support MMX technology

	// if we get here, it means the processor has MMX technology but
	// floating-point emulation is on; so MMX technology is unavailable

	return true;
#elif  __GNUC__

	int edx=0;
	try 
	{
		asm(
				"mov 1,%%eax \n" //Get ready for CPUID
				"CPUID \n" //Access CPUID
				:"=d"(edx) 
			 );
	}
	catch(...) //Catch all
	{
		return false;
	}
	if (edx & 0x800000)
	{
		try{ asm("emms"); }
		catch(...) {return false;}
	}
	else
		return false;
	
	return true;
#elif __BORLANDC__

//This is for detecting assembly when using the Borland compilers
//If you do not have TASM then this will not compile
//If you do have TASM then uncomment the code below, otherwise ignore it

	#ifdef USE_TASM
	int RegEDX=0;
	try
	{
	asm
	{
		mov eax, 1
		CPUID
		mov RegEDX, edx
	}
	}
	catch(...) {return false;}

	if (RegEDX & 0x800000)
	{
		try{asm{emms}}
		catch(...) {return false;}
	}
	else
		return false;
	return true;
#else

	CL_Log::log("debug", "Support to detect this has not been compiled in.");
	return false;
#endif //Endif USE_TASM

#else
	// Not implemented
	return false;
#endif

#else
	return false;
#endif //End USE_I386_ASSEMBLER
}
