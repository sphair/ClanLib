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

#include "Display/display_precomp.h"
#include "API/Display/Providers/provider_type_register.h"
#include "API/Display/Providers/pcx_provider.h"
#include "API/Display/Providers/bmp_provider.h"
#include "API/Display/Providers/targa_provider.h"
#include "API/Display/Providers/jpeg_provider.h"
#include "API/Display/Providers/png_provider.h"
#include "API/Display/setupdisplay.h"
#include "API/Core/Resources/resource_manager.h"
#include "resourcedata_surface.h"
#include "resourcedata_sprite.h"
#include "resourcedata_sprite_description.h"
#include "resourcedata_font.h"
#include "resourcedata_text_styler.h"
#include "resourcedata_collisionoutline.h"

static int init_count = 0;
static CL_Slot slot_resource_added;

static CL_ProviderType_Register<CL_PCXProvider> *pcx_provider = NULL;
static CL_ProviderType_Register<CL_BMPProvider> *bmp_provider = NULL;
static CL_ProviderType_Register<CL_TargaProvider> *tga_provider = NULL;
static CL_ProviderType_Register<CL_TargaProvider> *targa_provider = NULL;
static CL_ProviderType_Register<CL_JPEGProvider> *jpeg_provider = NULL;
static CL_ProviderType_Register<CL_JPEGProvider> *jpg_provider = NULL;
static CL_ProviderType_Register<CL_PNGProvider> *png_provider = NULL;

static void clandisplay_resource_added(CL_Resource &resource)
{
	std::string type = resource.get_type();
	
	if (type == "surface") new CL_ResourceData_Surface(resource);
	if (type == "sprite") new CL_ResourceData_Sprite(resource);
	if (type == "description") new CL_ResourceData_SpriteDescription(resource);
	if (type == "font") new CL_ResourceData_Font(resource);
	if (type == "text_styler") new CL_ResourceData_TextStyler(resource);
	if (type == "collisionoutline") new CL_ResourceData_CollisionOutline(resource);
}

CL_SetupDisplay::CL_SetupDisplay(bool register_resources_only)
{
	CL_SetupDisplay::init(register_resources_only);
}

CL_SetupDisplay::~CL_SetupDisplay()
{
	CL_SetupDisplay::deinit();
}

void CL_SetupDisplay::init(bool register_resources_only)
{
	init_count++;
	if (init_count != 1) return;

	slot_resource_added = CL_ResourceManager::sig_resource_added().connect(&clandisplay_resource_added);

	pcx_provider   = new CL_ProviderType_Register<CL_PCXProvider>("pcx");
	bmp_provider   = new CL_ProviderType_Register<CL_BMPProvider>("bmp");
	tga_provider   = new CL_ProviderType_Register<CL_TargaProvider>("tga");
	targa_provider = new CL_ProviderType_Register<CL_TargaProvider>("targa");
	jpeg_provider  = new CL_ProviderType_Register<CL_JPEGProvider>("jpeg");
	jpg_provider   = new CL_ProviderType_Register<CL_JPEGProvider>("jpg");
	png_provider   = new CL_ProviderType_Register<CL_PNGProvider>("png");
	
	if (register_resources_only) return;
}

void CL_SetupDisplay::deinit()
{
	init_count--;
	if (init_count != 0) return;

	delete pcx_provider;
	pcx_provider = NULL;

	delete bmp_provider;
	bmp_provider = NULL;

	delete tga_provider;
	tga_provider = NULL;

	delete targa_provider;
	targa_provider = NULL;

	delete jpeg_provider;
	jpeg_provider = NULL;

	delete jpg_provider;
	jpg_provider = NULL;

	delete png_provider;
	png_provider = NULL;

	slot_resource_added = CL_Slot();
}
