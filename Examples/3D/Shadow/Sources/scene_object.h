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
**    Mark Page
*/

#pragma once

#include "model.h"

class Scene;

class SceneObject
{
public:
	SceneObject(Scene &scene_owner, SceneObject *parent_object);
	virtual ~SceneObject();

	void Draw(CL_GraphicContext &gc, const CL_Mat4f &current_modelview, bool is_draw_shadow);

	void GetWorldMatrix(CL_Mat4f &world_matrix);

protected:
	void UpdateOrientationMatrix();
	void UpdateModelViewMatrix(const CL_Mat4f &current_modelview);

public:
	std::vector<SceneObject *> child_objects;

	CL_Vec3f pivot;		// Translation after rotation
	CL_Vec3f position;	// Translation before rotation
	CL_Angle rotation_x;
	CL_Angle rotation_y;
	CL_Angle rotation_z;
	CL_Vec3f scale;		// Scale

	CL_Mat4f orientation_matrix;	// Calculated by UpdateOrientationMatrix (Excludes pivot)
	CL_Mat4f modelview_matrix;		// Calculated by UpdateModelViewMatrix (Uses the orientation_matrix)

	Model model;		// The model (shared resource, that also may be null if not required)

	Scene *scene;		// The scene that this object belongs to
	SceneObject *parent;	// The parent object. NULL = Top of the tree
};

