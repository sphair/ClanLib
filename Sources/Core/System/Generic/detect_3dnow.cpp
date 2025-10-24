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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/System/system.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/log.h"

static bool do_3dnow_test();
static bool do_ext_3dnow_test();

bool CL_System::detect_3dnow()
{
	static bool has_3dnow = false;
	static bool first_time = true;
	
	if (first_time)
	{
		first_time = false;
		has_3dnow = do_3dnow_test();
	}
	
	return has_3dnow;
}

bool CL_System::detect_ext_3dnow()
{
	static bool has_ext_3dnow = false;
	static bool first_time = true;
	
	if (first_time)
	{
		first_time = false;
		has_ext_3dnow = do_ext_3dnow_test();
	}
	
	return has_ext_3dnow;
}

static bool do_3dnow_test()
{
#ifdef USE_I386_ASSEMBLER
#ifdef __MSC__
	unsigned long RegEDX = 0;
	unsigned long RegEAX = 0;

	try 
	{
		_asm
		{
			mov		eax, 0x80000000		// set up CPUID to return processor extended functions
			CPUID						// code bytes = 0fh,  0a2h
			mov		RegEAX, eax
		}

		if (!(RegEAX & 0x80000000))		// no functions > 0x80000000 ?
			return false;				//we don't have any extended features, quit
		else
			_asm
			{
				mov		eax, 0x80000001 // CPUID ext. function 0x80000001
				CPUID		                // EDX contains extended feature flags
				mov		RegEDX, edx     // extended features returned in edx
			}

	 	}
	catch(...)					// catch everything
	{
		return false;
	}

	if (RegEDX & 0x80000000) 	// bit 31 indicates presence of 3DNow! technology
	{
		try
		{
			//DB 0Fh, 0Eh			// try executing the 3DNow! instruction "femms"
		     	_asm _emit 0x0f;
		     	_asm _emit 0x0e;
		}
		catch(...) { return false; }
	}
   	else
		return false;

	return true;
#elif __BORLANDC__

//This is for detecting assembly when using the Borland compilers
//If you do not have TASM then this will not compile
//If you do have TASM then uncomment the code below, otherwise ignore it

	#ifdef USE_TASM
	int RegEDX=0,RegEAX=0;
  try
  {
  	asm
    {
    	mov		eax, 0x80000000		// set up CPUID to return processor extended functions
    	CPUID						// code bytes = 0fh,  0a2h
    	mov		RegEAX, eax
    }
  }
  catch(...) { return false; }

  if(RegEAX > 0x80000000)
  {
  	asm
    {
			mov     eax, 0x80000001		// CPUID ext. function 0x80000001
    	CPUID                       // EDX contains extended feature flags
    	mov		RegEDX, edx			// extended features returned in edx
    }
  }
  else
  	return false;

  if( RegEDX & 0x80000000)
  {
  	try
		{
			//DB 0Fh, 0Eh			// try executing the 3DNow! instruction "femms"
      __emit__(0x0f,0x0e);
		}
		catch(...) { return false; }
  }
  else
  	return false;
  return true;
  #else

	CL_Log::log("debug", "3DNow check support not compiled in.");
  return false;
	#endif
#elif __GNUC__

	unsigned int RegEDX=0,RegEAX=0;
	try
	{
		asm(
		"mov 0x80000000, %%eax \n" //Get ready for extended functions
		"CPUID \n" //Access CPUID
		: "=a"(RegEAX));
	}
	catch(...) 
	{
		return false;
	}
	if(RegEDX > 0x80000000)
	{
		asm(
				"mov 0x80000001, %%eax \n" //Access extended features
				"CPUID \n"  //Query CPU
				: "=d"(RegEDX));
	}
	else 
	{
		return false;
	}

	if(!(RegEDX & 0x80000000))
	{
		return false;
	}
	try
	{
		asm("femms");
	}
	catch(...)
	{
		return false;
	}
	return true;

#endif
#else
	// Not implemented under unix
	return false;
#endif //Endif USE_I386_ASSEMBLER
}

static bool do_ext_3dnow_test()
{
#ifdef USE_I386_ASSEMBLER
#ifdef __MSC__
	unsigned long RegEDX = 0;
	unsigned long RegEAX = 0;

	try 
	{
		_asm
		{
			mov		eax, 0x80000000		// set up CPUID to return processor extended functions
			CPUID						// code bytes = 0fh,  0a2h
			mov		RegEAX, eax
		}

		if (!(RegEAX & 0x80000000))		// no functions > 0x80000000 ?
			return false;				//we don't have any extended features, quit
		else
			_asm
			{
				mov     eax, 0x80000001		// CPUID ext. function 0x80000001
				CPUID                       // EDX contains extended feature flags
				mov		RegEDX, edx			// extended features returned in edx
			}

   	}
	catch(...)					// catch everything
	{
		return false;
	}

	if (RegEDX & 0x40000000) 	// bit 30 indicates presence of extended 3DNow! technology
	{
		try
		{
			//DB 0Fh, 0Eh			// try executing the 3DNow! instruction "femms"
		     	_asm _emit 0x0f;
		     	_asm _emit 0x0e;
		}
		catch(...) { return false; }
	}
   	else
		return false;

	return true;
#elif __BORLANDC__

//This is for detecting assembly when using the Borland compilers
//If you do not have TASM then this will not compile
//If you do have TASM then uncomment the code below, otherwise ignore it

	#ifdef USE_TASM
	int RegEDX=0,RegEAX=0;
  try
  {
  	asm
    {
    	mov		eax, 0x80000000		// set up CPUID to return processor extended functions
    	CPUID						// code bytes = 0fh,  0a2h
    	mov		RegEAX, eax
    }
  }
  catch(...) { return false; }

  if(RegEAX > 0x80000000)
  {
  	asm
    {
			mov     eax, 0x80000001		// CPUID ext. function 0x80000001
    	CPUID                       // EDX contains extended feature flags
    	mov		RegEDX, edx			// extended features returned in edx
    }
  }
  else
  	return false;

  if( RegEDX & 0x40000000)
  {
  	try
		{
			//DB 0Fh, 0Eh			// try executing the 3DNow! instruction "femms"
      __emit__(0x0f,0x0e);
		}
		catch(...) { return false; }
  }
  else
  	return false;
  return true;
  #else

	CL_Log::log("debug", "3DNow check support not compiled in.");
  return false;
	#endif
#elif __GNUC__

	unsigned int RegEDX=0,RegEAX=0;
	try
	{
		asm(
		"mov 0x80000000, %%eax \n" //Get ready for extended functions
		"CPUID \n" //Access CPUID
		: "=a"(RegEAX));
	}
	catch(...) 
	{
		return false;
	}
	if(RegEDX > 0x80000000)
	{
		asm(
				"mov 0x80000001, %%eax \n" //Access extended features
				"CPUID \n"  //Query CPU
				: "=d"(RegEDX));
	}
	else 
	{
		return false;
	}

	if(!(RegEDX & 0x40000000))
	{
		return false;
	}
	try
	{
		asm("femms");
	}
	catch(...)
	{
		return false;
	}
	return true;

#endif
#else
	// Not implemented under unix
	return false;
#endif //Endif USE_I386_ASSEMBLER

}
