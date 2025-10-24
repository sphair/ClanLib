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

#ifndef header_zip_compression_method
#define header_zip_compression_method

#if _MSC_VER > 1000
#pragma once
#endif

enum CL_Zip_CompressionMethod
{
	zip_compress_store, // no compression
	zip_compress_shrunk,
	zip_compress_expand_factor_1,
	zip_compress_expand_factor_2,
	zip_compress_expand_factor_3,
	zip_compress_expand_factor_4,
	zip_compress_implode,
	zip_compress_tokenize,
	zip_compress_deflate,
	zip_compress_deflate64,
	zip_compress_pkware_implode
};

#endif
