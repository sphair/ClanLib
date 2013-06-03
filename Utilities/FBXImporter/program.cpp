
#include "precomp.h"
#include "program.h"
#include "fbx_scene_cache.h"
#include <ClanLib/application.h>

#pragma comment(lib, "libfbxsdk-mt.lib")

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupD3D setup_d3d;

	ResourceManager resources;
	SceneCache::set(resources, std::shared_ptr<SceneCache>(new FBXSceneCache()));

	DisplayWindow window("FBX Viewer", 1024, 768, false, true);
	Canvas canvas(window);
	GraphicContext gc = canvas.get_gc();

	bool exit = false;
	Slot slot_window_close = window.sig_window_close().connect_functor([&exit]() { exit = true; });

	Scene scene(gc, resources, "../../Resources/Scene3D");

	SceneLight light1(scene);
	light1.set_position(Vec3f(100.0f, 100.0f, 100.0f));
	light1.set_type(SceneLight::type_omni);
	light1.set_attenuation_start(900.0f);
	light1.set_attenuation_end(1000.0f);
	light1.set_color(Vec3f(1.0f));

	SceneLight light2(scene);
	light2.set_position(Vec3f(-100.0f, -100.0f, -100.0f));
	light2.set_type(SceneLight::type_omni);
	light2.set_attenuation_start(900.0f);
	light2.set_attenuation_end(1000.0f);
	light2.set_color(Vec3f(0.5f));

	SceneCamera camera(scene);
	camera.set_position(Vec3f(0.0f, 0.0f, -100.0f));

	SceneModel model(gc, scene, "test.fbx");

	SceneObject object(scene, model, Vec3f(), Quaternionf(), Vec3f(0.5f));

	scene.set_skybox_gradient(gc, std::vector<Colorf>(1, Colorf::whitesmoke));
	scene.set_camera(camera);

	GameTime gametime;

	while (!exit)
	{
		gametime.update();
		scene.update(gc, gametime.get_time_elapsed());

		scene.render(gc);

		window.flip(1);
		KeepAlive::process();
	}

	return 0;
}


/*
	// Animation:

	FbxTime::EMode time_mode = scene->GetGlobalSettings().GetTimeMode();
	FbxTimeSpan timespan;

	scene->GetGlobalSettings().GetTimelineDefaultTimeSpan(timespan);
	float start_time = (float)timespan.GetStart().GetSecondDouble();
	float stop_time = (float)timespan.GetStop().GetSecondDouble();

	FbxAnimEvaluator *scene_evaluator = scene->GetEvaluator();

	for (float frame_time = start_time; frame_time < stop_time; frame_time += 1.0f/60.0f)
	{
		FbxTime current_time;
		current_time.SetSecondDouble(frame_time);

		// FbxMatrix &object_to_world = sceneEvaluator->GetNodeGlobalTransform(node, current_time);
		// FbxAnimCurveNode &value = scene_evaluator->GetPropertyValue(ambient_color, current_time);
	}

	// Global settings:

	const FbxGlobalSettings &globals = scene->GetGlobalSettings();

	Colorf scene_ambient((float)globals.GetAmbientColor().mRed, (float)globals.GetAmbientColor().mGreen, (float)globals.GetAmbientColor().mBlue, (float)globals.GetAmbientColor().mAlpha);
	std::string default_camera = globals.GetDefaultCamera();
*/



/*
void Program::mesh(FbxNode *node)
{
	FbxMesh *mesh = static_cast<FbxMesh*>(node->GetNodeAttribute());

	int vertex_index = 0;
	int num_polygons = mesh->GetPolygonCount();
	for (int poly = 0; poly < num_polygons; poly++)
	{
		int group = get_group(mesh, poly);

		int num_points = mesh->GetPolygonSize(poly);
		for (int point = 0; point < num_points; point++)
		{
			int control_index = mesh->GetPolygonVertex(poly, point);

			Vec4f position = get_position(mesh, poly, point, control_index);
			Vec4f color = get_color(mesh, poly, point, control_index, vertex_index);
			Vec2f uv = get_uv(mesh, poly, point, control_index, vertex_index);
			Vec3f normal = get_normal(mesh, poly, point, control_index, vertex_index);
			Vec3f tangent = get_tangent(mesh, poly, point, control_index, vertex_index);
			Vec3f bitangent = get_bitangent(mesh, poly, point, control_index, vertex_index);

			vertex_index++;
		}
	}

	int num_skins = mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int skin_index = 0; skin_index < num_skins; skin_index++)
	{
		FbxSkin *skin = static_cast<FbxSkin *>(mesh->GetDeformer(skin_index, FbxDeformer::eSkin));
		int num_clusters = skin->GetClusterCount();
		for (int cluster_index = 0; cluster_index < num_clusters; cluster_index++)
		{
			FbxCluster *cluster = skin->GetCluster(cluster_index);

			std::string link_name;
			if (cluster->GetLink())
				link_name = cluster->GetLink()->GetName();

			FbxCluster::ELinkMode mode = cluster->GetLinkMode();

			int num_indices = cluster->GetControlPointIndicesCount();
			int *indices = cluster->GetControlPointIndices();
			double *weights = cluster->GetControlPointWeights();

			FbxAMatrix transform, transform_link;
			cluster->GetTransformMatrix(transform);
			cluster->GetTransformLinkMatrix(transform_link);

			if (cluster->GetAssociateModel())
			{
				std::string associate_model = cluster->GetAssociateModel()->GetName();

				FbxAMatrix associate_model_transform;
				cluster->GetTransformAssociateModelMatrix(associate_model_transform);
			}
		}
	}

	for (int element_index = 0; element_index < mesh->GetElementMaterialCount(); element_index++)
	{
		FbxGeometryElementMaterial *element = mesh->GetElementMaterial(element_index);
		if (element->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			int index_array_size = element->GetIndexArray().GetCount();
			for (int index = 0; index < index_array_size; index++)
			{
				int material_index = element->GetIndexArray().GetAt(index);
			}
		}
		else if (element->GetMappingMode() == FbxGeometryElement::eAllSame)
		{
			int material_index = element->GetIndexArray().GetAt(0);
			// One material for the entire mesh
		}
	}

	int num_materials = node->GetMaterialCount();
	for (int material_index = 0; material_index < num_materials; material_index++)
	{
		FbxSurfaceMaterial *material = node->GetMaterial(material_index);
		if (material->GetClassId().Is(FbxSurfaceLambert::ClassId) || material->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			FbxSurfaceLambert *lambert = static_cast<FbxSurfaceLambert*>(material);

			FbxDouble3 emissive = lambert->Emissive.Get();
			float emissive_factor = (float)lambert->EmissiveFactor.Get();

			FbxDouble3 ambient = lambert->Ambient.Get();
			float ambient_factor = (float)lambert->AmbientFactor.Get();

			FbxDouble3 diffuse = lambert->Diffuse.Get();
			float diffuse_factor = (float)lambert->DiffuseFactor.Get();

			FbxDouble3 normal_map = lambert->NormalMap.Get();

			FbxDouble3 bump = lambert->Bump.Get();
			float bump_factor = (float)lambert->BumpFactor.Get();

			FbxDouble3 transparent_color = lambert->TransparentColor.Get();
			float transparency_factor = (float)lambert->TransparencyFactor.Get();

			FbxDouble3 displacement_color = lambert->DisplacementColor.Get();
			float displacement_factor = (float)lambert->DisplacementFactor.Get();

			FbxDouble3 vector_displacement_color = lambert->VectorDisplacementColor.Get();
			float vector_displacement_factor = (float)lambert->VectorDisplacementFactor.Get();

			if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				FbxSurfacePhong *phong = static_cast<FbxSurfacePhong*>(lambert);

				FbxDouble3 specular = phong->Specular.Get();
				float factor = (float)phong->SpecularFactor.Get();
				float shininess = (float)phong->Shininess.Get();
				FbxDouble3 reflection = phong->Reflection.Get();
				float reflection_factor = (float)phong->ReflectionFactor.Get();
			}

			// Get diffuse texture:
			const char *property_name = FbxSurfaceMaterial::sDiffuse;
			if (material->FindProperty(property_name).GetSrcObjectCount<FbxFileTexture>() > 0)
			{
				FbxFileTexture *texture = material->FindProperty(property_name).GetSrcObject<FbxFileTexture>(0);
				std::string filename = texture->GetFileName();
				std::string uv_set = texture->UVSet.Get();
			}
		}
		else
		{
			// Unknown material class id
		}
	}
}

int Program::get_group(FbxMesh *mesh, int polygon)
{
	int num_groups = mesh->GetElementPolygonGroupCount();
	for (int group = 0; group < num_groups; group++)
	{
		FbxGeometryElementPolygonGroup *poly_group = mesh->GetElementPolygonGroup(group);
		if (poly_group->GetMappingMode() == FbxGeometryElement::eByPolygon && poly_group->GetReferenceMode() == FbxGeometryElement::eIndex)
		{
			return poly_group->GetIndexArray().GetAt(polygon);
		}
	}
	return -1;
}

Vec2f Program::get_uv(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index)
{
	FbxVector2 uv;
	for (int i = 0; i < mesh->GetElementUVCount(); i++)
	{
		FbxGeometryElementUV *element = mesh->GetElementUV(i);
		if (element->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (element->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				uv = element->GetDirectArray().GetAt(control_index);
			}
			else if (element->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = element->GetIndexArray().GetAt(control_index);
				uv = element->GetDirectArray().GetAt(id);
			}
		}
		else if (element->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (element->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				uv = element->GetDirectArray().GetAt(vertex_index);
			}
			else if (element->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = element->GetIndexArray().GetAt(vertex_index);
				uv = element->GetDirectArray().GetAt(id);
			}
		}
	}
	return Vec2f((float)uv[0], (float)uv[1]);
}
*/
