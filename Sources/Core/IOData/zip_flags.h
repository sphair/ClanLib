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

#ifndef zip_flags
#define zip_flags

// Version from which Zip64 was introduced
#define CL_ZIP_64Version 45

// General purpose flags:
#define CL_ZIP_ENCRYPTED                        1

#define CL_ZIP_IMPLODE_8K_SLIDING_DICT          2
#define CL_ZIP_IMPLODE_3_SHANNON_FANO_TREES     4

#define CL_ZIP_DEFLATE_COMPRESS_MAXIMUM         2
#define CL_ZIP_DEFLATE_COMPRESS_FAST            4
#define CL_ZIP_DEFLATE_COMPRESS_SUPER_FAST      6

#define CL_ZIP_CRC32_IN_FILE_DESCRIPTOR         8
#define CL_ZIP_DEFLATE64_RESERVED              16
#define CL_ZIP_FILE_IS_COMPRESSED_PATCHED_DATA 32

// CRC-32 properties:

#define CL_ZIP_CRC_MAGIC_NUMBER 0xdebb20e3
#define CL_ZIP_CRC_START_VALUE 0xffffffff

// File attributes:

#define CL_ZIP_FILE_ATTR_TEXT 1

#endif
