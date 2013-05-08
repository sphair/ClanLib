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

/// \addtogroup clanScene_Scene clanScene Scene
/// \{

#pragma once

#include "api_scene3d.h"
#include "scene_inout_data.h"
#include "../Core/Signals/callback_v1.h"
#include "../Core/Text/string_format.h"
#include <memory>
#include <string>

namespace clan
{

class GraphicContext;
class Scene;
class ScenePass_Impl;

class CL_API_SCENE ScenePass
{
public:
	ScenePass();
	ScenePass(Scene &scene, const std::string &name, const std::string &insert_before = std::string());

	bool is_null() const;

	Callback_v1<GraphicContext &> &func_run();

	const std::string &get_name() const;

	SceneInOutDataContainer &get_inout_container();

	template<typename Type>
	SceneInOutData<Type> get_inout(const std::string &name)
	{
		return get_inout_container().get<Type>(name);
	}

private:
	std::shared_ptr<ScenePass_Impl> impl;
};

}

/// \}
