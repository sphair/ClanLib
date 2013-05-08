/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Mark Page
*/

#include "precomp.h"
#include "instanced_object.h"
#include "scene.h"
#include "shader_color_instanced.h"
#include "graphic_store.h"

InstancedObject::InstancedObject(Scene &scene_owner, SceneObject *parent_object) : SceneObject(scene_owner, parent_object)
{
}

InstancedObject::~InstancedObject()
{
}

void InstancedObject::Draw(GraphicContext &gc, const Mat4f &current_modelview)
{
	UpdateOrientationMatrix();
	UpdateModelViewMatrix(current_modelview);

	if (!model.is_null())
	{
		if (centers.size() > 0)
		{
			scene->gs->shader_color_instanced.SetCenters(centers);
			scene->gs->shader_color_instanced.SetMaterialAmbientSpecular(colors);
			model.Draw(gc, scene->gs, modelview_matrix, centers.size());
		}
	}

	std::vector<SceneObject *>::size_type size, cnt;
	size = child_objects.size();
	for (cnt=0; cnt< size; cnt++)
	{
		child_objects[cnt]->Draw(gc, modelview_matrix);
	}
}

