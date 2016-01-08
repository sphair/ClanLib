/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "scene_object.h"
#include "scene.h"

SceneObject::SceneObject(Scene &scene_owner, SceneObject *parent_object) : scale(1.0f, 1.0f, 1.0f), scene(&scene_owner), parent(parent_object)
{
	if (parent_object)
		parent_object->child_objects.push_back(this);
}

SceneObject::~SceneObject()
{
	std::vector<SceneObject *>::size_type size, cnt;
	size = child_objects.size();
	for (cnt=0; cnt< size; cnt++)
	{
		delete child_objects[cnt];
	}
}

void SceneObject::UpdateOrientationMatrix()
{
	Mat4f rotation_matrix_x = Mat4f::rotate(rotation_x, 1.0f, 0.0f, 0.0f, false);
	Mat4f rotation_matrix_y = Mat4f::rotate(rotation_y, 0.0f, 1.0f, 0.0f, false);
	Mat4f rotation_matrix_z = Mat4f::rotate(rotation_z, 0.0f, 0.0f, 1.0f, false);

	orientation_matrix = rotation_matrix_y * rotation_matrix_x * rotation_matrix_z;

	orientation_matrix.matrix[ (4*0) + 0 ] *= scale.x;
	orientation_matrix.matrix[ (4*0) + 1 ] *= scale.x;
	orientation_matrix.matrix[ (4*0) + 2 ] *= scale.x;
	orientation_matrix.matrix[ (4*0) + 3 ] = 0.0f;

	orientation_matrix.matrix[ (4*1) + 0 ] *= scale.y;
	orientation_matrix.matrix[ (4*1) + 1 ] *= scale.y;
	orientation_matrix.matrix[ (4*1) + 2 ] *= scale.y;
	orientation_matrix.matrix[ (4*1) + 3 ] = 0.0f;

	orientation_matrix.matrix[ (4*2) + 0 ] *= scale.z;
	orientation_matrix.matrix[ (4*2) + 1 ] *= scale.z;
	orientation_matrix.matrix[ (4*2) + 2 ] *= scale.z;
	orientation_matrix.matrix[ (4*2) + 3 ] = 0.0f;

	orientation_matrix.matrix[ (4*3) + 0 ] = position.x;
	orientation_matrix.matrix[ (4*3) + 1 ] = position.y;
	orientation_matrix.matrix[ (4*3) + 2 ] = position.z;
	orientation_matrix.matrix[ (4*3) + 3 ] = 1.0f;
}

void SceneObject::UpdateModelViewMatrix(const Mat4f &current_modelview)
{
	modelview_matrix = current_modelview * orientation_matrix;

	if ( (pivot.x != 0.0f) || (pivot.y != 0.0f) || (pivot.z != 0.0f) )
	{
		Mat4f pivot_matrix;
		pivot_matrix[ (4*0) + 0 ] = 1.0f;
		pivot_matrix[ (4*1) + 1 ] = 1.0f;
		pivot_matrix[ (4*2) + 2 ] = 1.0f;
		pivot_matrix[ (4*3) + 3 ] = 1.0f;

		pivot_matrix[ (4*3) + 0 ] = -pivot.x;
		pivot_matrix[ (4*3) + 1 ] = -pivot.y;
		pivot_matrix[ (4*3) + 2 ] = -pivot.z;

		//TODO: pivot position not checked, the line below may be the wrong way round
		modelview_matrix = modelview_matrix * pivot_matrix;
	}
}

void SceneObject::Draw(GraphicContext &gc, const Mat4f &current_modelview)
{
	UpdateOrientationMatrix();
	UpdateModelViewMatrix(current_modelview);

	if (!model.is_null())
		model.Draw(gc, scene->gs, modelview_matrix);

	std::vector<SceneObject *>::size_type size, cnt;
	size = child_objects.size();
	for (cnt=0; cnt< size; cnt++)
	{
		child_objects[cnt]->Draw(gc, modelview_matrix);
	}
}

void SceneObject::GetWorldMatrix(Mat4f &world_matrix)
{
	if (parent)
		parent->GetWorldMatrix(world_matrix);

	UpdateOrientationMatrix();

	world_matrix = world_matrix * orientation_matrix;

	if ( (pivot.x != 0.0f) || (pivot.y != 0.0f) || (pivot.z != 0.0f) )
	{
		Mat4f pivot_matrix;
		pivot_matrix[ (4*0) + 0 ] = 1.0f;
		pivot_matrix[ (4*1) + 1 ] = 1.0f;
		pivot_matrix[ (4*2) + 2 ] = 1.0f;
		pivot_matrix[ (4*3) + 3 ] = 1.0f;

		pivot_matrix[ (4*3) + 0 ] = -pivot.x;
		pivot_matrix[ (4*3) + 1 ] = -pivot.y;
		pivot_matrix[ (4*3) + 2 ] = -pivot.z;

		//TODO: pivot position not checked, the line below may be the wrong way round
		world_matrix = world_matrix *  pivot_matrix;
	}
}
