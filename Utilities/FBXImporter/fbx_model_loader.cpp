
#include "precomp.h"
#include "fbx_model_loader.h"

using namespace clan;

std::shared_ptr<ModelData> FBXModelLoader::load(const std::string &filename)
{
	FBXModelLoader loader(filename);
	return loader.model_data;
}

FBXModelLoader::FBXModelLoader(const std::string &filename)
: base_path(PathHelp::get_basepath(filename)), manager(nullptr), iosettings(nullptr), scene(nullptr), model_data(new ModelData())
{
	try
	{
		import_scene(filename);
		//bake_geometric_transforms();
		convert_node(scene->GetRootNode());
		convert_bones();
	}
	catch (...)
	{
		if (manager)
			manager->Destroy();
		throw;
	}
}

FBXModelLoader::~FBXModelLoader()
{
	if (manager)
		manager->Destroy();
}

void FBXModelLoader::import_scene(const std::string &filename)
{
	manager = FbxManager::Create();
	if (!manager)
		throw Exception("FbxManager::Create failed");

	iosettings = FbxIOSettings::Create(manager, IOSROOT);
	if (!iosettings)
		throw Exception("FbxIOSettings::Create failed");

	manager->SetIOSettings(iosettings);

	FbxString plugins_directory = FbxGetApplicationDirectory();
	bool result = manager->LoadPluginsDirectory(plugins_directory.Buffer());
	if (!result)
		throw Exception("FbxManager::LoadPluginsDirectory failed");

	scene = FbxScene::Create(manager, "My Scene");
	if (!scene)
		throw Exception("FbxScene::Create failed");


	// Convert to DirectX coordinate system:
	// Note: this line MUST be before importing a scene or something bugs in FBX! (or the documentation is unclear)
	FbxAxisSystem::DirectX.ConvertScene(scene);


	FbxImporter *importer = FbxImporter::Create(manager, "");
	if (!importer)
		throw Exception("FbxImporter::Create failed");

	result = importer->Initialize(filename.c_str(), -1, manager->GetIOSettings());
	if (!result)
		throw Exception(importer->GetStatus().GetErrorString());

	result = importer->Import(scene);
	if (!result && importer->GetStatus().GetCode() == FbxStatus::ePasswordError)
		throw Exception("FBX file is password protected!");

	importer->Destroy();
}

void FBXModelLoader::bake_geometric_transforms(FbxNode *node)
{
	// bake transform components: pre- and post-rotation, rotation and scale pivots and offsets - into the standard transforms

	if (node == nullptr)
	{
		float sampling_rate = 60.0f;
		bake_geometric_transforms(scene->GetRootNode());
		scene->GetRootNode()->ConvertPivotAnimationRecursive(nullptr, FbxNode::eDestinationPivot, sampling_rate);
	}
	else
	{
		// We set up what we want to bake via ConvertPivotAnimationRecursive.
		// When the destination is set to 0, baking will occur.
		// When the destination value is set to the source’s value, the source values will be retained and not baked.

		FbxVector4 zero(0,0,0);

		// Activate pivot converting
		node->SetPivotState(FbxNode::eSourcePivot, FbxNode::ePivotActive);
		node->SetPivotState(FbxNode::eDestinationPivot, FbxNode::ePivotActive);

		// We want to set all these to 0 and bake them into the transforms.
		node->SetPostRotation(FbxNode::eDestinationPivot, zero);
		node->SetPreRotation(FbxNode::eDestinationPivot, zero);
		node->SetRotationOffset(FbxNode::eDestinationPivot, zero);
		node->SetScalingOffset(FbxNode::eDestinationPivot, zero);
		node->SetRotationPivot(FbxNode::eDestinationPivot, zero);
		node->SetScalingPivot(FbxNode::eDestinationPivot, zero);
		node->SetRotationOrder(FbxNode::eDestinationPivot, eEulerXYZ);
		node->SetGeometricTranslation(FbxNode::eDestinationPivot, zero);
		node->SetGeometricRotation(FbxNode::eDestinationPivot, zero);
		node->SetGeometricScaling(FbxNode::eDestinationPivot, zero);
		//node->SetGeometricTranslation(FbxNode::eDestinationPivot, node->GetGeometricTranslation(FbxNode::eSourcePivot));
		//node->SetGeometricRotation(FbxNode::eDestinationPivot, node->GetGeometricRotation(FbxNode::eSourcePivot));
		//node->SetGeometricScaling(FbxNode::eDestinationPivot, node->GetGeometricScaling(FbxNode::eSourcePivot));

		// Keep the quaternion interpolation mode
		node->SetQuaternionInterpolation(FbxNode::eDestinationPivot, node->GetQuaternionInterpolation(FbxNode::eSourcePivot));

		for (int i = 0; i < node->GetChildCount(); i++)
		{
			bake_geometric_transforms(node->GetChild(i));
		}
	}
}

void FBXModelLoader::convert_node(FbxNode *node)
{
	FbxNodeAttribute *node_attribute = node->GetNodeAttribute();
	if (node_attribute)
	{
		FbxNodeAttribute::EType node_type = node_attribute->GetAttributeType();
		switch (node_type)
		{
		case FbxNodeAttribute::eMesh:
			convert_mesh(node);
			break;

		case FbxNodeAttribute::eCamera:
			convert_camera(node);
			break;

		case FbxNodeAttribute::eLight:
			convert_light(node);

		case FbxNodeAttribute::eMarker:
		case FbxNodeAttribute::eSkeleton:
		case FbxNodeAttribute::eNurbs:
		case FbxNodeAttribute::ePatch:
		case FbxNodeAttribute::eLODGroup:
			break;
		}
	}

	// FbxProperty user_property = node->GetFirstProperty();
	// bool has_user_properties = user_property.IsValid();

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		convert_node(node->GetChild(i));
	}
}

void FBXModelLoader::convert_mesh(FbxNode *node)
{
	FbxMesh *mesh = static_cast<FbxMesh*>(node->GetNodeAttribute());

	Mat4f mesh_to_world = to_mat4f(node->EvaluateGlobalTransform() * FbxAMatrix(node->GetGeometricTranslation(FbxNode::eSourcePivot), node->GetGeometricRotation(FbxNode::eSourcePivot), node->GetGeometricScaling(FbxNode::eSourcePivot)));
	Mat3f normal_mesh_to_world = Mat3f(mesh_to_world).inverse().transpose();

	VertexMappingVector vertices(mesh->GetControlPointsCount());
	std::vector<VertexMapping *> elements;

	convert_polygons(mesh, vertices, elements);
	convert_skins(node, mesh, vertices);

	if (model_data->meshes.empty())
		model_data->meshes.resize(1);

	ModelDataMesh &model_mesh = model_data->meshes[0];

	model_mesh.channels.resize(4);

	for (size_t i = 0; i < vertices.size(); i++)
	{
		for (VertexMapping *mapping = vertices[i]; mapping != nullptr; mapping = mapping->next)
		{
			mapping->vertex_index = model_mesh.vertices.size();

			model_mesh.vertices.push_back(Vec3f(mesh_to_world * Vec4f(mapping->position, 1.0f)));
			model_mesh.colors.push_back(mapping->color);
			model_mesh.normals.push_back(normal_mesh_to_world * mapping->normal);
			model_mesh.tangents.push_back(mapping->tangent);
			model_mesh.bitangents.push_back(mapping->bitangent);
			model_mesh.bone_weights.push_back(mapping->bone_weights);
			model_mesh.bone_selectors.push_back(mapping->bone_selectors);
			model_mesh.channels[0].push_back(mapping->diffuse_uv);
			model_mesh.channels[1].push_back(mapping->specular_uv);
			model_mesh.channels[2].push_back(mapping->normal_uv);
			model_mesh.channels[3].push_back(mapping->emission_uv);
		}
	}

	std::vector<std::vector<VertexMapping *> > material_elements(node->GetMaterialCount());

	for (int element_index = 0; element_index < mesh->GetElementMaterialCount(); element_index++)
	{
		FbxGeometryElementMaterial *element = mesh->GetElementMaterial(element_index);
		if (element->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			int index_array_size = element->GetIndexArray().GetCount();
			for (int index = 0; index < index_array_size; index++)
			{
				int material_index = element->GetIndexArray().GetAt(index);

				material_elements[material_index].insert(material_elements[material_index].end(), elements.begin() + index * 3, elements.begin() + index * 3 + 3);
			}
		}
		else if (element->GetMappingMode() == FbxGeometryElement::eAllSame)
		{
			int material_index = element->GetIndexArray().GetAt(0);
			material_elements[material_index].insert(material_elements[material_index].end(), elements.begin(), elements.end());
		}
	}

	for (size_t material_index = 0; material_index < material_elements.size(); material_index++)
	{
		size_t start_element = model_mesh.elements.size();
		size_t num_elements = material_elements[material_index].size();

		if (num_elements == 0)
			continue;

		for (size_t i = 0; i < num_elements; i++)
		{
			model_mesh.elements.push_back(material_elements[material_index][i]->vertex_index);
		}

		model_mesh.draw_ranges.push_back(create_draw_range(start_element, num_elements, node->GetMaterial(material_index)));
	}
}

ModelDataDrawRange FBXModelLoader::create_draw_range(size_t start_element, size_t num_elements, FbxSurfaceMaterial *material)
{
	ModelDataDrawRange range;
	range.start_element = start_element;
	range.num_elements = num_elements;

	// To do: where can we retrieve these flags from?
	range.two_sided = true;
	range.transparent = false;
	range.alpha_test = false;

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

		range.ambient.set_single_value(to_vec3f(ambient) * ambient_factor);
		range.diffuse.set_single_value(to_vec3f(diffuse) * diffuse_factor);
		range.self_illumination_amount.set_single_value(emissive_factor);
		range.self_illumination.set_single_value(to_vec3f(emissive));

		if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			FbxSurfacePhong *phong = static_cast<FbxSurfacePhong*>(lambert);

			FbxDouble3 specular = phong->Specular.Get();
			float factor = (float)phong->SpecularFactor.Get();
			float shininess = (float)phong->Shininess.Get();
			FbxDouble3 reflection = phong->Reflection.Get();
			float reflection_factor = (float)phong->ReflectionFactor.Get();

			// To do: find the shader math (haha, good luck!) for Autodesk FBX phong and make sure this is correct:

			range.specular.set_single_value(to_vec3f(specular));
			range.specular_level.set_single_value(factor);
			range.glossiness.set_single_value(shininess);
		}
		else
		{
			range.specular.set_single_value(Vec3f());
			range.specular_level.set_single_value(0.0f);
			range.glossiness.set_single_value(0.0f);
		}

		// Get diffuse texture:
		const char *property_name = FbxSurfaceMaterial::sDiffuse;
		if (material->FindProperty(property_name).GetSrcObjectCount<FbxFileTexture>() > 0)
		{
			FbxFileTexture *texture = material->FindProperty(property_name).GetSrcObject<FbxFileTexture>(0);
			std::string filename = PathHelp::combine(base_path, PathHelp::get_filename(texture->GetFileName()));
			std::string uv_set = texture->UVSet.Get();

			range.diffuse_map.texture = model_data->textures.size();

			// To do: where to get this from?
			range.diffuse_map.channel = 0;
			range.diffuse_map.wrap_x = ModelDataTextureMap::wrap_repeat;
			range.diffuse_map.wrap_y = ModelDataTextureMap::wrap_repeat;
			range.diffuse_map.uvw_offset.set_single_value(Vec3f(0.0f));
			range.diffuse_map.uvw_rotation.set_single_value(Quaternionf());
			range.diffuse_map.uvw_scale.set_single_value(Vec3f(1.0f));

			model_data->textures.push_back(ModelDataTexture(filename, 2.2f));
		}
	}
	else
	{
		// Unknown material class id

		range.ambient.set_single_value(Vec3f(1.0f));
		range.diffuse.set_single_value(Vec3f(1.0f));
		range.specular.set_single_value(Vec3f(1.0f));
		range.specular_level.set_single_value(0.0f);
		range.glossiness.set_single_value(0.0f);
		range.self_illumination_amount.set_single_value(0.0f);
		range.self_illumination.set_single_value(Vec3f(0.0f));
	}

	return range;
}

void FBXModelLoader::convert_polygons(FbxMesh *mesh, VertexMappingVector &vertices, std::vector<VertexMapping *> &face_vertices)
{
	FbxVector4 *control_points = mesh->GetControlPoints();

	int num_polygons = mesh->GetPolygonCount();

	face_vertices.reserve(num_polygons * 3);

	int vertex_index = 0;
	for (int poly = 0; poly < num_polygons; poly++)
	{
		int num_points = mesh->GetPolygonSize(poly);
		if (num_points != 3)
			throw Exception("FBX loader can only handle triangle faces");

		for (int point = 0; point < num_points; point++, vertex_index++)
		{
			int control_index = mesh->GetPolygonVertex(poly, point);

			Vec3f position = Vec3f(to_vec4f(control_points[control_index]));
			Vec4ub color = get_color(mesh, poly, point, control_index, vertex_index);
			Vec3f normal = get_normal(mesh, poly, point, control_index, vertex_index);
			Vec3f tangent = get_tangent(mesh, poly, point, control_index, vertex_index);
			Vec3f bitangent = get_bitangent(mesh, poly, point, control_index, vertex_index);
			Vec2f diffuse_uv = get_uv(mesh, poly, point, control_index, vertex_index, 0);

			if (vertices[control_index] == nullptr)
			{
				vertices[control_index] = new VertexMapping();
				vertices[control_index]->position = position;
				vertices[control_index]->color = color;
				vertices[control_index]->normal = normal;
				vertices[control_index]->tangent = tangent;
				vertices[control_index]->bitangent = bitangent;
				vertices[control_index]->diffuse_uv = diffuse_uv;

				face_vertices.push_back(vertices[control_index]);
			}
			else
			{
				VertexMapping *mapping = vertices[control_index];
				while (mapping->position == position && mapping->color == color && mapping->normal == normal && mapping->tangent == tangent && mapping->bitangent == bitangent && mapping->diffuse_uv == diffuse_uv)
				{
					if (mapping->next)
					{
						mapping = mapping->next;
					}
					else
					{
						mapping->next = new VertexMapping();
						mapping = mapping->next;

						mapping->position = position;
						mapping->color = color;
						mapping->normal = normal;
						mapping->tangent = tangent;
						mapping->bitangent = bitangent;
						mapping->diffuse_uv = diffuse_uv;
						break;
					}
				}

				face_vertices.push_back(mapping);
			}
		}
	}
}

/*
	bind_node_to_world = pCluster->GetTransformMatrix();
	// Multiply bind_mesh_to_world by Geometric Transformation
	mesh_to_node = node->GeometryXX
	bind_mesh_to_world = mesh_to_node * bind_node_to_world;

	// Get the link initial global position and the link current global position.
	bind_bone_to_world = pCluster->GetTransformLinkMatrix();
	bone_to_world = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

	// Compute the initial position of the link relative to the reference.
	bind_mesh_to_bind_bone = bind_bone_to_world.Inverse() * bind_mesh_to_world;

	// Compute the current position of the link relative to the reference.
	bone_to_mesh = mesh_to_world.Inverse() * bone_to_world;

	// Compute the shift of the link relative to the reference.
	bind_mesh_to_mesh = bone_to_mesh * bind_mesh_to_bind_bone;
*/

void FBXModelLoader::convert_skins(FbxNode *node, FbxMesh *mesh, VertexMappingVector &vertices)
{
	int num_skins = mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int skin_index = 0; skin_index < num_skins; skin_index++)
	{
		FbxSkin *skin = static_cast<FbxSkin *>(mesh->GetDeformer(skin_index, FbxDeformer::eSkin));

		FbxSkin::EType skinning_type = skin->GetSkinningType();
		if (skinning_type != FbxSkin::eLinear && skinning_type != FbxSkin::eRigid)
			throw Exception("Only linear or rigid skinning types supported");

		int num_clusters = skin->GetClusterCount();
		for (int cluster_index = 0; cluster_index < num_clusters; cluster_index++)
		{
			FbxCluster *cluster = skin->GetCluster(cluster_index);

			if (cluster->GetLinkMode() != FbxCluster::eNormalize)
				throw Exception("Unsupported skinning link mode");

			if (!cluster->GetLink())
				throw Exception("Skin cluster with no link is not supported");

			std::string link_name = cluster->GetLink()->GetName();

			FbxAMatrix bind_node_to_world;
			cluster->GetTransformMatrix(bind_node_to_world);

			FbxAMatrix mesh_to_node(node->GetGeometricTranslation(FbxNode::eSourcePivot), node->GetGeometricRotation(FbxNode::eSourcePivot), node->GetGeometricScaling(FbxNode::eSourcePivot));

			FbxAMatrix bind_mesh_to_world = bind_node_to_world * mesh_to_node;

			FbxAMatrix bind_bone_to_world;
			cluster->GetTransformLinkMatrix(bind_bone_to_world);

			FbxAMatrix mesh_to_world = node->EvaluateGlobalTransform() * mesh_to_node;

			int bone_selector = bones.size();
			SkinnedBone bone;
			bone.bone_node = cluster->GetLink();
			bone.bind_mesh_to_world = to_mat4f(bind_mesh_to_world);
			bone.bind_bone_to_world = to_mat4f(bind_bone_to_world);
			bone.mesh_to_world = to_mat4f(mesh_to_world);
			bones.push_back(bone);

			int num_indices = cluster->GetControlPointIndicesCount();
			int *indices = cluster->GetControlPointIndices();
			double *weights = cluster->GetControlPointWeights();

			for (int i = 0; i < num_indices; i++)
			{
				for (VertexMapping *mapping = vertices[indices[i]]; mapping != nullptr; mapping = mapping->next)
				{
					for (unsigned int j = 0; j < 4; j++)
					{
						if (mapping->bone_selectors[j] == 255)
						{
							mapping->bone_selectors[j] = bone_selector;
							mapping->bone_weights[j] = clamp((int)(weights[i] * 255 + 0.5), 0, 255);
							break;
						}
					}
				}
			}
		}
	}

	for (size_t i = 0; i < vertices.size(); i++)
	{
		for (VertexMapping *mapping = vertices[i]; mapping != nullptr; mapping = mapping->next)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				if (mapping->bone_selectors[j] == 255)
				{
					mapping->bone_selectors[j] = 0;
				}
			}
		}
	}
}

Vec4ub FBXModelLoader::get_color(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index)
{
	FbxGeometryElementVertexColor *element = mesh->GetElementVertexColor(0);
	if (element)
	{
		if (element->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (element->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				return to_vec4ub(element->GetDirectArray().GetAt(control_index));
			}
			else if (element->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = element->GetIndexArray().GetAt(control_index);
				return to_vec4ub(element->GetDirectArray().GetAt(id));
			}
		}
		else if (element->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (element->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				return to_vec4ub(element->GetDirectArray().GetAt(vertex_index));
			}
			else if (element->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = element->GetIndexArray().GetAt(vertex_index);
				return to_vec4ub(element->GetDirectArray().GetAt(id));
			}
		}
	}
	return Vec4ub();
}

Vec3f FBXModelLoader::get_normal(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index)
{
	FbxGeometryElementNormal *element = mesh->GetElementNormal(0);
	if (element)
	{
		if (element->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (element->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				return Vec3f(to_vec4f(element->GetDirectArray().GetAt(vertex_index)));
			}
			else if (element->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = element->GetIndexArray().GetAt(vertex_index);
				return Vec3f(to_vec4f(element->GetDirectArray().GetAt(id)));
			}
		}
		else if (element->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (element->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				return Vec3f(to_vec4f(element->GetDirectArray().GetAt(control_index)));
			}
			else if (element->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = element->GetIndexArray().GetAt(control_index);
				return Vec3f(to_vec4f(element->GetDirectArray().GetAt(id)));
			}
		}
		else
		{
			throw Exception("Unsupported normal type");
		}
	}
	return Vec3f();
}

Vec3f FBXModelLoader::get_tangent(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index)
{
	FbxGeometryElementTangent *element = mesh->GetElementTangent(0);
	if (element)
	{
		if (element->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (element->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				return Vec3f(to_vec4f(element->GetDirectArray().GetAt(vertex_index)));
			}
			else if (element->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = element->GetIndexArray().GetAt(vertex_index);
				return Vec3f(to_vec4f(element->GetDirectArray().GetAt(id)));
			}
		}
		else
		{
			throw Exception("Unsupported tangent type");
		}
	}
	return Vec3f();
}

Vec3f FBXModelLoader::get_bitangent(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index)
{
	FbxGeometryElementBinormal *element = mesh->GetElementBinormal(0);
	if (element)
	{
		if (element->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (element->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				return Vec3f(to_vec4f(element->GetDirectArray().GetAt(vertex_index)));
			}
			else if (element->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = element->GetIndexArray().GetAt(vertex_index);
				return Vec3f(to_vec4f(element->GetDirectArray().GetAt(id)));
			}
		}
		else
		{
			throw Exception("Unsupported bitangent type");
		}
	}
	return Vec3f();
}

Vec2f FBXModelLoader::get_uv(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index, int uv_channel_index)
{
	FbxGeometryElementUV *element = mesh->GetElementUV(uv_channel_index);
	if (element)
	{
		if (element->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (element->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				return to_vec2f(element->GetDirectArray().GetAt(control_index));
			}
			else if (element->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = element->GetIndexArray().GetAt(control_index);
				return to_vec2f(element->GetDirectArray().GetAt(id));
			}
		}
		else if (element->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (element->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				return to_vec2f(element->GetDirectArray().GetAt(vertex_index));
			}
			else if (element->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = element->GetIndexArray().GetAt(vertex_index);
				return to_vec2f(element->GetDirectArray().GetAt(id));
			}
		}
		else
		{
			throw Exception("Unsupported bitangent type");
		}
	}

	return Vec2f();
}

void FBXModelLoader::convert_camera(FbxNode *node)
{
	FbxCamera *camera = static_cast<FbxCamera*>(node->GetNodeAttribute());
/*
	std::string name = camera->GetName();
	FbxCamera::EProjectionType projection_type = camera->ProjectionType.Get(); // Perspective, Orthogonal
	FbxDouble3 position = camera->Position.Get();
*/
}

void FBXModelLoader::convert_light(FbxNode *node)
{
	FbxLight *light = static_cast<FbxLight*>(node->GetNodeAttribute());
/*
	std::string name = light->GetName();

	FbxLight::EType type = light->LightType.Get(); // point, directional, spot
	bool casts_light = light->CastLight.Get();
	bool casts_shadows = light->CastShadows.Get();

	std::string light_texture = light->FileName.Get();
	bool ground_projection = light->DrawGroundProjection.Get();
	bool volumetric_projection = light->DrawVolumetricLight.Get();
	bool front_volumetric_projection = light->DrawFrontFacingVolumetricLight.Get();

	FbxDouble3 color = light->Color.Get();
	float intensity = (float)light->Intensity.Get();
	float outer_angle = (float)light->OuterAngle.Get();
	float inner_angle = (float)light->InnerAngle.Get();
	float fog = (float)light->Fog.Get();
*/
}

void FBXModelLoader::convert_bones()
{
	model_data->bones.resize(bones.size());

	FbxTime::EMode time_mode = scene->GetGlobalSettings().GetTimeMode();
	FbxTimeSpan timespan;

	scene->GetGlobalSettings().GetTimelineDefaultTimeSpan(timespan);
	float start_time = (float)timespan.GetStart().GetSecondDouble();
	float stop_time = (float)timespan.GetStop().GetSecondDouble();

	FbxAnimEvaluator *scene_evaluator = scene->GetEvaluator();

	ModelDataAnimation animation;
	animation.name = "default";
	animation.length = stop_time - start_time;
	animation.loop = true;
	animation.playback_speed = 1.0f;
	animation.moving_speed = 0.0f;
	model_data->animations.push_back(animation);

	int num_steps = (int)std::ceil((stop_time - start_time) * 60.0f);
	for (int step = 0; step <= num_steps; step++)
	{
		float step_time = std::min(start_time + step / 60.0f, stop_time);

		FbxTime current_time;
		current_time.SetSecondDouble(step_time);

		for (size_t bone_index = 0; bone_index < bones.size(); bone_index++)
		{
			Mat4f bone_to_world = to_mat4f(scene_evaluator->GetNodeGlobalTransform(bones[bone_index].bone_node, current_time, FbxNode::eSourcePivot, true));
			const Mat4f &bind_mesh_to_world = bones[bone_index].bind_mesh_to_world;
			const Mat4f &bind_bone_to_world = bones[bone_index].bind_bone_to_world;
			const Mat4f &mesh_to_world = bones[bone_index].mesh_to_world;

			// bind mesh -> (bind) world -> bind bone -> (current bone) world -> bind mesh -> (current mesh) world
			Mat4f skinned_bone_to_world = mesh_to_world * Mat4f::inverse(bind_mesh_to_world) * bone_to_world * Mat4f::inverse(bind_bone_to_world) * bind_mesh_to_world * Mat4f::inverse(mesh_to_world);

			Vec3f position, scale;
			Quaternionf orientation;
			skinned_bone_to_world.decompose(position, orientation, scale);
			orientation.normalize();

			model_data->bones[bone_index].position.timelines.resize(1);
			model_data->bones[bone_index].orientation.timelines.resize(1);
			model_data->bones[bone_index].scale.timelines.resize(1);

			model_data->bones[bone_index].position.timelines[0].timestamps.push_back(step_time - start_time);
			model_data->bones[bone_index].position.timelines[0].values.push_back(position);

			model_data->bones[bone_index].orientation.timelines[0].timestamps.push_back(step_time - start_time);
			model_data->bones[bone_index].orientation.timelines[0].values.push_back(orientation);

			model_data->bones[bone_index].scale.timelines[0].timestamps.push_back(step_time - start_time);
			model_data->bones[bone_index].scale.timelines[0].values.push_back(scale);
		}

		// FbxMatrix &object_to_world = scene_evaluator->GetNodeGlobalTransform(node, current_time);
		// FbxAnimCurveNode &value = scene_evaluator->GetPropertyValue(value, current_time);
	}
}

Vec2f FBXModelLoader::to_vec2f(const FbxVector2 &v)
{
	return Vec2f((float)v[0], (float)v[1]);
}

Vec3f FBXModelLoader::to_vec3f(const FbxDouble3 &d)
{
	return Vec3f((float)d[0], (float)d[1], (float)d[2]);
}

Vec4f FBXModelLoader::to_vec4f(const FbxVector4 &v)
{
	return Vec4f((float)v[0], (float)v[1], (float)v[2], (float)v[3]);
}

Vec4ub FBXModelLoader::to_vec4ub(const FbxColor &c)
{
	return Vec4ub(clamp((int)(c.mRed*255+0.5), 0, 255), clamp((int)(c.mGreen*255+0.5), 0, 255), clamp((int)(c.mBlue*255+0.5), 0, 255), clamp((int)(c.mAlpha*255+0.5), 0, 255));
}

Mat4f FBXModelLoader::to_mat4f(const FbxAMatrix &m)
{
	const double *src_matrix = m;
	Mat4f matrix;
	for (int i = 0; i < 16; i++)
		matrix[i] = (float)src_matrix[i];
	return matrix;
}

/*
	// Global settings:

	const FbxGlobalSettings &globals = scene->GetGlobalSettings();

	Colorf scene_ambient((float)globals.GetAmbientColor().mRed, (float)globals.GetAmbientColor().mGreen, (float)globals.GetAmbientColor().mBlue, (float)globals.GetAmbientColor().mAlpha);
	std::string default_camera = globals.GetDefaultCamera();
*/

/*
int get_group(FbxMesh *mesh, int polygon)
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
*/
