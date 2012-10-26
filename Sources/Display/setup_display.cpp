/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "Display/precomp.h"
#include "API/Display/setup_display.h"
#include "API/Display/ImageProviders/provider_type_register.h"
#include "API/Display/ImageProviders/targa_provider.h"
#include "API/Display/ImageProviders/jpeg_provider.h"
#include "API/Display/ImageProviders/png_provider.h"

#ifndef WIN32
#ifndef __APPLE__
#include <X11/Xlib.h>
#endif
#endif

namespace clan
{

static ProviderType_Register<JPEGProvider> *jpeg_provider = NULL;
static ProviderType_Register<JPEGProvider> *jpg_provider = NULL;
static ProviderType_Register<PNGProvider> *png_provider = NULL;
static ProviderType_Register<TargaProvider> *targa_provider = NULL;
static ProviderType_Register<TargaProvider> *tga_provider = NULL;

/////////////////////////////////////////////////////////////////////////////
// SetupDisplay Construction:

SetupDisplay::SetupDisplay()
{
#ifndef WIN32
#ifndef __APPLE__
	// The XInitThreads() function initializes Xlib support for concurrent threads.
	// This function must be the first Xlib function a multi-threaded program calls, and it must complete before any other Xlib call is made.
	XInitThreads();
#endif
#endif
	jpeg_provider  = new ProviderType_Register<JPEGProvider>("jpeg");
	jpg_provider   = new ProviderType_Register<JPEGProvider>("jpg");
	png_provider   = new ProviderType_Register<PNGProvider>("png");
	targa_provider = new ProviderType_Register<TargaProvider>("targa");
	tga_provider   = new ProviderType_Register<TargaProvider>("tga");
}

SetupDisplay::~SetupDisplay()
{
	delete jpeg_provider;
	jpeg_provider = NULL;

	delete jpg_provider;
	jpg_provider = NULL;

	delete png_provider;
	png_provider = NULL;

	delete targa_provider;
	targa_provider = NULL;

	delete tga_provider;
	tga_provider = NULL;
}

}
