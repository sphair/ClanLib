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

#include "Sound/precomp.h"

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include "resourcetype_sample.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/error.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/IOData/outputsource.h"
#include "API/Core/IOData/outputsource_provider.h"
#include "API/Sound/soundbuffer.h"
#include "API/Sound/soundprovider.h"
#include "API/Sound/SoundProviders/soundprovider_wave.h"
#include "API/Sound/SoundProviders/soundprovider_factory.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Sample Construction:

CL_ResourceData_Sample::CL_ResourceData_Sample(class CL_Resource &resource)
: CL_ResourceData(resource)
{
	resource.attach_data("sample", this);
}

CL_ResourceData_Sample::~CL_ResourceData_Sample()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Sample Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Sample Implementation:

void CL_ResourceData_Sample::on_load()
{
	CL_Resource resource = get_resource();
	CL_DomElement &element = resource.get_element();

	CL_SoundProvider *provider = 0;

	std::string name = resource.get_element().get_attribute("file");
	std::string sound_format = resource.get_element().get_attribute("format");
	bool streamed = (element.get_attribute("stream", "no") == "yes");

	provider = CL_SoundProviderFactory::load(
		name,
		streamed,
		sound_format,
		resource.get_manager().get_resource_provider());

	if (provider)
	{
		soundbuffer = new CL_SoundBuffer(provider, true);
	}
	else
	{
		throw CL_Error("Unknown sample format");
	}
}

void CL_ResourceData_Sample::on_unload()
{
	delete soundbuffer;
	soundbuffer = NULL;
}
