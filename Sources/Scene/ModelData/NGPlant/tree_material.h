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

#include "NGPCore/p3dmodel.h"

class TreeMaterialInstance : public P3DMaterialInstance
{
public:
	TreeMaterialInstance(const P3DMaterialDef &init_material_def) { material_def.CopyFrom(&init_material_def); }
	const P3DMaterialDef *GetMaterialDef() const { return &material_def; }
	P3DMaterialInstance *CreateCopy() const { return new TreeMaterialInstance(material_def); }

private:
	P3DMaterialDef material_def;
};

class TreeMaterialFactory : public P3DMaterialFactory
{
public:
	P3DMaterialInstance *CreateMaterial(const P3DMaterialDef &material_def) const { return new TreeMaterialInstance(material_def); }
};

class TreeMaterialSaver : public P3DMaterialSaver
{
public:
	void Save(P3DOutputStringStream *target_stream, const P3DMaterialInstance *material) const
	{
		material->GetMaterialDef()->Save(target_stream);
	}
};

}

