/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_consumed_keys.h"

#include <map>

class CL_GUIConsumedKeys_Impl
{
public:
	CL_GUIConsumedKeys_Impl()
	{
		keys[CL_GUIConsumedKeys::key_arrows] = false;
		keys[CL_GUIConsumedKeys::key_characters] = false;
		keys[CL_GUIConsumedKeys::key_enter] = false;
		keys[CL_GUIConsumedKeys::key_space] = false;
		keys[CL_GUIConsumedKeys::key_tab] = false;
	}
	
	std::map<CL_GUIConsumedKeys::ConsumedKey, bool> keys;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUIConsumedKeys Construction:

CL_GUIConsumedKeys::CL_GUIConsumedKeys() : impl(new CL_GUIConsumedKeys_Impl())
{
}

CL_GUIConsumedKeys::~CL_GUIConsumedKeys()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIConsumedKeys Attributes:

bool CL_GUIConsumedKeys::is_consumed(ConsumedKey key)
{
	std::map<ConsumedKey, bool>::iterator it;
	it = impl->keys.find(key);
	if (it != impl->keys.end())
		return (*it).second;
	else
		throw CL_Exception(cl_text("CL_GUIConsumedKeys::is_consumed(key): key value not handled!"));

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIConsumedKeys Operations:

void CL_GUIConsumedKeys::set_consumed(ConsumedKey key, bool consumed)
{
	impl->keys[key] = consumed;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIConsumedKeys Implementation:
