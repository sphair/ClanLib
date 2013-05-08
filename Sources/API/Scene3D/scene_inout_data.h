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
#include <memory>
#include <map>

namespace clan
{

class SceneInOutData_BaseImpl
{
public:
	virtual ~SceneInOutData_BaseImpl() { }
};

template<typename Type>
class SceneInOutData_Impl : public SceneInOutData_BaseImpl
{
public:
	SceneInOutData_Impl() : value(), generation(0) { }
	Type value;
	int generation;
};

template<typename Type>
class SceneInOutData
{
	SceneInOutData()
	: object(new SceneInOutData_Impl<Type>()), generation(-1)
	{
	}

	SceneInOutData(std::shared_ptr<SceneInOutData_Impl<Type> > object)
	: object(object), generation(-1)
	{
	}

	Type *operator->()
	{
		return &object->value;
	}

	const Type *operator->() const
	{
		return &object->value;
	}

	bool updated()
	{
		bool updated = (generation != object->generation);
		generation = object->generation;
		return updated;
	}

	void set(const Type &value)
	{
		object->value = value;
		generation = ++obj->generation;
	}

	Type &get()
	{
		return object->value;
	}

	const Type &get() const
	{
		return object->value;
	}

private:
	std::shared_ptr<SceneInOutData_Impl<Type> > object;
	int generation;
};

class SceneInOutDataContainer_Impl
{
public:
	std::map<std::string, std::shared_ptr<SceneInOutData_BaseImpl> > inout_data;
};

class SceneInOutDataContainer
{
public:
	SceneInOutDataContainer() : impl(new SceneInOutDataContainer_Impl())
	{
	}

	template<typename Type>
	SceneInOutData<Type> get(const std::string &name)
	{
		std::shared_ptr<SceneInOutData_BaseImpl> inout_base = impl->inout_data[name];
		if (inout_base)
		{
			std::shared_ptr<SceneInOutData_Impl<Type> > inout = std::dynamic_pointer_cast<Type>(inout_base);
			if (!inout)
				throw Exception(string_format("ScenePass inout type mismatch for %1", name));
			return SceneInOutData<Type>(inout);
		}
		else
		{
			std::shared_ptr<SceneInOutData_Impl<Type> > inout(new SceneInOutData_Impl<Type>());
			impl->inout_data[name] = std::static_pointer_cast<SceneInOutData_BaseImpl>(inout);
			return SceneInOutData<Type>(inout);
		}
	}

private:
	std::shared_ptr<SceneInOutDataContainer_Impl> impl;
};

}

/// \}
