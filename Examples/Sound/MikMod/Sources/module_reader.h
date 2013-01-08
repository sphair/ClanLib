
#pragma once

/*
 * The following structure is the ClanLib file reader driver for MikMod.
 * In fact MikMod core functions expect an MREADER struct, so the first
 * field of MCLANLIBREADER (core) is an MREADER, and this way the MikMod
 * code can access correctly all the fields of the MREADER core struct,
 * while we provide an additionnal pointer (input) to handle the file,
 * which is a clan::IODevice in ClanLib's case.
 * This method might look like an ugly hack but MikMod does this internally,
 * with an MFILEREADER struct casted an into a plain MREADER.
 */

#ifdef _M_X64
#ifndef __x86_64
#endif
// Required for mikmod.h (64 bit)
#define __x86_64	1
#endif

#include <mikmod.h>

typedef struct MCLANLIBREADER
{
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
