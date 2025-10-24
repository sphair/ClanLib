/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Kevin Bluck
*/

#include "Display/precomp.h"
#include "font_config.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/System/databuffer.h"
#include "API/Display/Font/font_description.h"
#include <cstdlib>

/////////////////////////////////////////////////////////////////////////////
// CL_FontConfig Construction:

CL_FontConfig::CL_FontConfig()
{
        if (!fc_config)
        {
		fc_config = FcInitLoadConfigAndFonts();
        }
        if  (!fc_config)
	{
		throw CL_Exception("CL_FontConfig: Initializing FontConfig library failed.");
	} 
}

CL_FontConfig::~CL_FontConfig()
{
	// For some reason calling FcFini() throws an internal FC assert.*/
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontConfig Attributes:

CL_FontConfig &CL_FontConfig::instance()
{
	static CL_FontConfig fc;
	return fc;
}


/////////////////////////////////////////////////////////////////////////////
// CL_FontConfig Operations:

CL_String CL_FontConfig::match_font(const CL_FontDescription &desc) const
{
	FcPattern * fc_pattern = NULL;
	FcPattern * fc_match = NULL;
	try
	{
		int weight = desc.get_weight();

		// Build font matching pattern.
		fc_pattern = FcPatternBuild (NULL,
		FC_FAMILY, FcTypeString, desc.get_typeface_name().c_str(),
		FC_PIXEL_SIZE, FcTypeDouble, (double) std::abs(desc.get_height()),
		FC_WEIGHT, FcTypeInteger, ((weight > 0) ? (int)(weight * (FC_WEIGHT_HEAVY/900.0)) : FC_WEIGHT_NORMAL),
		FC_SLANT, FcTypeInteger, desc.get_italic() ? FC_SLANT_ITALIC : FC_SLANT_ROMAN,
		FC_SPACING, FcTypeInteger, desc.get_fixed_pitch() ? FC_MONO : FC_PROPORTIONAL,
		(char*) NULL);
		if (!fc_pattern)
		{
			throw CL_Exception("CL_FontConfig: Building FontConfig pattern failed.");
		}

		// Execute any needed param substitutions required by the system config.
		if (FcTrue !=  FcConfigSubstitute(fc_config, fc_pattern, FcMatchPattern))
		{
			throw CL_Exception("CL_FontConfig: Font config substitutions failed.");
		}

		// Supply default values for underspecified font patterns. Never fails.
		FcDefaultSubstitute(fc_pattern);

		// Find best match for pattern and extract filename.
		FcResult match_result; // Doesn't appear to be actually updated.
                fc_match = FcFontMatch(fc_config, fc_pattern, &match_result);
		FcChar8 * fc_font_file_path = NULL;
		if (FcResultMatch != FcPatternGetString (fc_match, FC_FILE, 0, &fc_font_file_path))
		{
			throw CL_Exception("CL_FontConfig: Could not resolve font pattern to a font file.");
		}

		// Release resources and return results.
		CL_String cl_font_file_path((char*) fc_font_file_path);
		FcPatternDestroy(fc_match);
		FcPatternDestroy(fc_pattern);
		return cl_font_file_path;
	}
	catch(...)
	{
		// If any exceptions thrown, ensure fontconfig resources are released.
		if (fc_match) FcPatternDestroy(fc_match);
		if (fc_pattern) FcPatternDestroy(fc_pattern);
		throw;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontConfig Implementation:

