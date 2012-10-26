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
**    Mark Page
*/

#include "precomp.h"
#include "example.h"
#include "shader.h"

#if defined(_MSC_VER)
	#if !defined(_DEBUG)
		#if defined(_DLL)
			#pragma comment(lib, "assimp-static-mtdll.lib")
		#else
			#pragma comment(lib, "assimp-static-mt.lib")
		#endif
	#else
		#if defined(_DLL)
			#pragma comment(lib, "assimp-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "assimp-static-mt-debug.lib")
		#endif
	#endif
#endif

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	quit = false;
    GL1WindowDescription desc;

	desc.set_title("ClanLib Object 3D Example");
	desc.set_size(Size(640, 480), true);
	desc.set_multisampling(4);
	desc.set_depth_size(16);

	DisplayWindow window(desc);

#ifdef _DEBUG
	//struct aiLogStream stream;
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	//aiAttachLogStream(&stream);
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	//aiAttachLogStream(&stream);
#endif

	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	// Get the graphic context
	GraphicContext gc = window.get_gc();

#ifdef USE_OPENGL_1
    GraphicContext_GL1 gc_gl1 = gc;
#endif

	// Prepare the display
	gc.set_map_mode(cl_user_projection);

	gc.set_culled(true);
	gc.set_face_cull_mode(cl_cull_back);
	gc.set_front_face(cl_face_side_clockwise);
	
	gc.enable_depth_test(true);
	gc.set_depth_compare_function(cl_comparefunc_lequal);
	gc.enable_depth_write(true);
	
#ifdef USE_OPENGL_1
	// Set the lights
	LightModel_GL1 light_model;
	light_model.enable_lighting(true);
	light_model.set_flat_shading(false);
	light_model.set_scene_ambient_light(Colorf(0.2f, 0.2f, 0.2f, 1.0f));
	gc_gl1.set_light_model(light_model);

	LightSource_GL1 light_distant;
	light_distant.set_spot_cutoff(180.0f);
	light_distant.set_diffuse_intensity(Colorf(1.0f, 1.0f, 1.0f, 1.0f));
	light_distant.set_position(Vec4f(0.0f, -2.0f, 30.0f, 0.0f).normalize3());
	gc_gl1.set_light(0, light_distant);

	cl1Enable(GL_NORMALIZE);
#endif

#ifdef USE_OPENGL_2
    Shader shader(gc);
#endif

	// Create the objects

	aiPropertyStore* store = aiCreatePropertyStore();
	aiSetImportPropertyFloat(store, AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE,89.53f);

	const struct aiScene* scene_teapot = aiImportFileExWithProperties("../Clan3D/Resources/teapot.dae",aiProcessPreset_TargetRealtime_MaxQuality, NULL, store);
	if (!scene_teapot)
		throw Exception("Cannot load the teapot model");

	const struct aiScene* scene_clanlib = aiImportFileExWithProperties("../Clan3D/Resources/clanlib.dae",aiProcessPreset_TargetRealtime_MaxQuality, NULL, store);
	if (!scene_clanlib)
		throw Exception("Cannot load the clanlib model");

	const struct aiScene* scene_tuxball = aiImportFileExWithProperties("../Clan3D/Resources/tux_ball.dae",aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs, NULL, store);
	if (!scene_tuxball)
		throw Exception("Cannot load the tux ball model");

	// Load the texture
	Texture tux(gc, "../Clan3D/Resources/tux.png");

	float angle = 0.0f;
	// Run until someone presses escape
	while (!quit)
	{

		Mat4f perp = Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 1000.0f);
		gc.set_projection(perp);

		gc.clear(Colorf::black);
		gc.clear_depth(1.0f);

		angle += 1.0f;
		if (angle >= 360.0f)
			angle -= 360.0f;


#ifdef USE_OPENGL_2
        shader.Set(gc);
        shader.Use(gc);
#else
        gc.set_program_object(cl_program_color_only);
#endif

		PrimitivesArray prim_array(gc);

		gc.set_modelview(Mat4f::identity());
		gc.mult_scale(1.0f,1.0f, -1.0f);	// So +'ve Z goes into the screen
		gc.mult_translate(0.0f, 0.0f, 2.0f);
		gc.mult_rotate(Angle(angle, cl_degrees), 0.0f, 1.0f, 0.0f, false);

		gc.push_modelview();
		recursive_render(gc, scene_teapot, scene_teapot->mRootNode, false);
		gc.pop_modelview();

		gc.push_modelview();
		gc.mult_scale(0.5f, 0.5f, 0.5f);
		gc.mult_translate(0.0f, -0.5f, 0.0f);
		recursive_render(gc, scene_clanlib, scene_clanlib->mRootNode, false);
		gc.pop_modelview();

#ifdef USE_OPENGL_2
        shader.Set(gc, 0);
        shader.Use(gc);
#else
        gc.set_program_object(cl_program_single_texture);
#endif

		gc.set_texture(0, tux);
 		gc.set_modelview(Mat4f::identity());
		gc.mult_scale(1.0f,1.0f, -1.0f);	// So +'ve Z goes into the screen
		gc.mult_translate(0.7f, 0.5f, 2.0f);
		gc.mult_scale(0.05f, 0.05f, 0.05f);
		gc.mult_rotate(Angle(angle * 4.0f, cl_degrees), 0.0f, 1.0f, 0.0f, false);
		recursive_render(gc, scene_tuxball, scene_tuxball->mRootNode, true);
		gc.reset_texture(0);

		gc.reset_program_object();
		
		// Flip the display, showing on the screen what we have drawed
		// since last call to flip()
		window.flip(1);

		// This call processes user input and other events
		KeepAlive::process();
	}

	aiReleaseImport(scene_tuxball);
	aiReleaseImport(scene_clanlib);
	aiReleaseImport(scene_teapot);
	aiReleasePropertyStore(store);
	aiDetachAllLogStreams();

	return 0;
}

void App::recursive_render(GraphicContext &gc, const struct aiScene *sc, const struct aiNode* nd, bool use_texture_coords)
{
	int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	gc.push_modelview();
	gc.mult_modelview(Mat4f((float*)&m));

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

		if (mesh->mNormals == NULL)
			throw Exception("This example expects normals to be set");

		std::vector<Vec3f> normals;
		std::vector<Vec3f> vertices;
		std::vector<Vec2f> tex_coords;

		normals.reserve(mesh->mNumFaces * 3);
		vertices.reserve(mesh->mNumFaces * 3);

		if (use_texture_coords)
		{
			if (mesh->mTextureCoords == NULL || mesh->mTextureCoords[0] == NULL)
				throw Exception("This example expects texcoords to be set for this object");
			tex_coords.reserve(mesh->mNumFaces * 3);
		}

		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			const struct aiFace* face = &mesh->mFaces[t];

			if (face->mNumIndices != 3)
					throw Exception("This example only supports triangles");

			for(i = 0; i < face->mNumIndices; i++)
			{
				int index = face->mIndices[i];
				normals.push_back(Vec3f(&mesh->mNormals[index].x));
				vertices.push_back( Vec3f(&mesh->mVertices[index].x));
				if (use_texture_coords)
					tex_coords.push_back( Vec2f(&mesh->mTextureCoords[0][index].x));
			}
		}

		if (!vertices.empty())
		{
			PrimitivesArray prim_array(gc);
			prim_array.set_attributes(cl_attrib_position, &vertices[0]);
			prim_array.set_attribute(cl_attrib_color, Colorf::white);
			prim_array.set_attributes(cl_attrib_normal, &normals[0]);
			if (use_texture_coords)
				prim_array.set_attributes(cl_attrib_texture_position, &tex_coords[0]);
			gc.draw_primitives(cl_triangles, vertices.size(), prim_array);
		}
	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		recursive_render(gc, sc, nd->mChildren[n], use_texture_coords);
	}

	gc.pop_modelview();
}

// A key was pressed
void App::on_input_up(const InputEvent &key)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

