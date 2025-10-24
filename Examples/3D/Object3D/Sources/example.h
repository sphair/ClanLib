/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

typedef struct _Model
{
	std::vector<CL_Collada_Triangles> positions_surface_list;
	std::vector<CL_Vec3f> object_positions;
	std::vector<CL_Vec3f> object_normals;

	std::vector<CL_Collada_Triangles> texcoord_surface_list;
	std::vector<CL_Vec2f> object_texcoords;
} Model;

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();

private:
	void create_objects_without_texture(Model &model, std::vector<CL_Collada_Image> &library_images);
	void create_objects(Model &model, std::vector<CL_Collada_Image> &library_images);

	void scale_points(CL_Vec3f *points_ptr, int size, const CL_Vec3f &object_scale);
	void invert_uvmap(CL_Vec2f *points_ptr, int size);
	void translate_points(CL_Vec3f *points_ptr, int size, const CL_Vec3f &object_translation);
	void insert_object(CL_Collada &object, const CL_String &geometry_name, const CL_Vec3f &object_scale, const CL_Vec3f &object_translation, Model &model, std::vector<CL_Collada_Image> &library_images, const CL_Angle &smooth_threshold);

	bool quit;
};
