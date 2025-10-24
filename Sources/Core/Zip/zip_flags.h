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
*/

#ifndef __header_zip_flags__
#define __header_zip_flags__

// Version from which Zip64 was introduced
#define CL_ZIP_64Version 45

// General purpose flags:
#define CL_ZIP_ENCRYPTED                        0x0001

#define CL_ZIP_IMPLODE_8K_SLIDING_DICT          0x0002
#define CL_ZIP_IMPLODE_3_SHANNON_FANO_TREES     0x0004

#define CL_ZIP_DEFLATE_COMPRESS_MAXIMUM         0x0002
#define CL_ZIP_DEFLATE_COMPRESS_FAST            0x0004
#define CL_ZIP_DEFLATE_COMPRESS_SUPER_FAST      0x0006

#define CL_ZIP_CRC32_IN_FILE_DESCRIPTOR         0x0008
#define CL_ZIP_DEFLATE64_RESERVED               0x0010
#define CL_ZIP_FILE_IS_COMPRESSED_PATCHED_DATA  0x0020
#define CL_ZIP_STRONG_ENCRYPTED                 0x0040
#define CL_ZIP_USE_UTF8                         0x0800
#define CL_ZIP_ENHANCED_COMPRESSION_RESERVED    0x1000
#define CL_ZIP_ENCRYPTED_LOCAL_HEADER_MASKED    0x2000

// CRC-32 properties:

#define CL_ZIP_CRC_MAGIC_NUMBER             0xdebb20e3
#define CL_ZIP_CRC_START_VALUE              0xffffffff

// File attributes:

#define CL_ZIP_FILE_ATTR_TEXT                   0x0001
#define CL_ZIP_FILE_ATTR_MAINFRAME_RECORDS      0x0002

#endif
