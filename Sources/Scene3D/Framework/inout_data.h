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

#pragma once
namespace clan
{

template<typename Type>
class InOutDataImpl
{
public:
	InOutDataImpl() : value(), generation(0) { }
	Type value;
	int generation;

	std::shared_ptr<InOutDataImpl> redirect;
};

template<typename Type>
class InOutData
{
public:
	InOutData()
	: object(new InOutDataImpl<Type>())
	{
	}

	Type *operator->()
	{
		return &find_value_object()->value;
	}

	const Type *operator->() const
	{
		return &find_value_object()->value;
	}

	void bind_from(InOutData &other)
	{
		if (this != &other)
		{
			object->redirect = other.object;
			generation = find_value_object()->generation - 1;
		}
	}

	bool updated()
	{
		std::shared_ptr<InOutDataImpl<Type> > obj = find_value_object();
		bool updated = (generation != obj->generation);
		generation = obj->generation;
		return updated;
	}

	void set(const Type &value)
	{
		std::shared_ptr<InOutDataImpl<Type> > obj = find_value_object();
		obj->value = value;
		generation = ++obj->generation;
	}

	Type &get()
	{
		return find_value_object()->value;
	}

	const Type &get() const
	{
		return find_value_object()->value;
	}

private:
	std::shared_ptr<InOutDataImpl<Type> > find_value_object() const
	{
		std::shared_ptr<InOutDataImpl<Type> > obj = object;
		while (obj->redirect && obj->redirect != object)
			obj = obj->redirect;
		return obj;
	}

	std::shared_ptr<InOutDataImpl<Type> > object;
	int generation;
};

#if !defined(_MSC_VER) || _MSC_VER >= 1800 // C++11 syntax
template<typename Type> using InPass = InOutPass<Type>;
template<typename Type> using OutPass = InOutPass<Type>;
#else
#define InData InOutData
#define OutData InOutData
#endif

}

