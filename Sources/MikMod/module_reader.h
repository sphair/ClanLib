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

#ifndef header_module_reader
#define header_module_reader

#if _MSC_VER > 1000
#pragma once
#endif

/*
 * The following structure is the ClanLib file reader driver for MikMod.
 * In fact MikMod core functions expect an MREADER struct, so the first
 * field of MCLANLIBREADER (core) is an MREADER, and this way the MikMod
 * code can access correctly all the fields of the MREADER core struct,
 * while we provide an additionnal pointer (input) to handle the file,
 * which is a CL_InputSource in ClanLib's case.
 * This method might look like an ugly hack but MikMod does this internally,
 * with an MFILEREADER struct casted an into a plain MREADER.
 */


#ifdef __APPLE__
#include <mikmod/mikmod.h>
#else
#include <mikmod.h>
#endif

typedef struct MCLANLIBREADER {
	MREADER core;
	void *input;
} MCLANLIBREADER;


#define CLANLIB_READER_CHANNELS     32 

extern BOOL clanlib_reader_feof(MREADER* reader);
extern BOOL clanlib_reader_read(MREADER* reader,void* ptr,size_t size);
extern int clanlib_reader_get(MREADER* reader);
extern BOOL clanlib_reader_seek(MREADER* reader,long offset,int whence);
extern long clanlib_reader_tell(MREADER* reader);
extern MREADER *new_clanlib_reader(void *input);
extern void delete_clanlib_reader (MREADER* reader);

#endif

