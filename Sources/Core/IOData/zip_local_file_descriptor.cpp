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
#include "zip_local_file_descriptor.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/outputsource.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_LocalFileDescriptor construction:

CL_Zip_LocalFileDescriptor::CL_Zip_LocalFileDescriptor()
{
}
	
CL_Zip_LocalFileDescriptor::~CL_Zip_LocalFileDescriptor()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_LocalFileDescriptor attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_Zip_LocalFileDescriptor operations:

void CL_Zip_LocalFileDescriptor::load(CL_InputSource *input)
{
	crc32 = input->read_int32();
	compressed_size = input->read_int32();
	uncompressed_size = input->read_int32();
}
	
void CL_Zip_LocalFileDescriptor::save(CL_OutputSource *output)
{
	output->write_int32(crc32);
	output->write_int32(compressed_size);
	output->write_int32(uncompressed_size);
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_Zip_LocalFileDescriptor implementation:
