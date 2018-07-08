#include "SceneLoader.hpp"
#include "Mesh.hpp"
#include "TextureLoader.hpp"
#include "Texture.hpp"
#include "Skeleton.hpp"
#include "Animation.hpp"

namespace KG
{

	SceneLoader::SceneLoader(void)
		: m_pScene(nullptr)
	{}

	Mesh_SmartPtr SceneLoader::Load(const std::string & p_rPath)
	{
		if (!this->LoadAiScene(p_rPath))
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "Mesh load failure.");
			return nullptr;
		}
		assert(m_pScene != nullptr);
		m_ScenePath = p_rPath;
		return InitFromAiScene(m_pScene, p_rPath);
	}

	const bool SceneLoader::LoadAiScene(const std::string & p_rPath)
	{
		const std::size_t index(p_rPath.find_last_of('x'));
		const std::size_t index2(p_rPath.find_first_of("model"));
		if (index != std::string::npos
				&& index != 0 // not first character.
				&& index2 == 0 // starts with "model"
				&& p_rPath.at(index-1) == '.'
			)// flip winding for CITS .x file. (normally .x has CW winding, but the CITS ones have CCW.
		{
			m_pScene
				= m_Importer.ReadFile
				(
					p_rPath.c_str()
					, aiProcessPreset_TargetRealtime_MaxQuality
						|aiProcess_FlipWindingOrder
						|aiProcess_FlipUVs
						|aiProcess_LimitBoneWeights
				);
		}
		else
		{
			m_pScene
				= m_Importer.ReadFile
				(
					p_rPath.c_str()
					, aiProcessPreset_TargetRealtime_MaxQuality
						|aiProcess_FlipUVs
						|aiProcess_LimitBoneWeights
				);
		}

		if (!m_pScene)
		{
			KE::Debug::print
			(
				KE::Debug::DBG_ERROR, "Meshes LoadMeshes scene import failed. Importer error string = "
				+ std::string(m_Importer.GetErrorString())
			);
			return false;
		}
		return true;
	}

	Mesh_SmartPtr SceneLoader::InitFromAiScene(const aiScene * p_pScene, const std::string & p_rPath)
	{
		KG::Mesh_SmartPtr master_mesh_sp(new KG::Mesh);
		KG::Skeleton_SmartPtr skeleton_sp(new KG::Skeleton(master_mesh_sp->GetEntityID()));
		this->RecursiveInitScene(master_mesh_sp, skeleton_sp, m_pScene->mRootNode);

		// cleanup
		m_Importer.FreeScene();
		m_pScene = nullptr;
		m_ScenePath = "";
		return master_mesh_sp;
	}

	void SceneLoader::RecursiveInitScene
		(
			KG::Mesh_SmartPtr p_spParentMesh
			, KG::Skeleton_SmartPtr p_spSkeleton
			, const aiNode * const p_pAiNode
		)
	{
		// check null parent mesh and skeleton??
		assert(p_pAiNode);
		assert(p_spParentMesh);
		assert(p_pAiNode);

		// create/recover BoneNodes.


		if (p_pAiNode->mNumMeshes > 0)
		{
			for (unsigned i = 0; i < p_pAiNode->mNumMeshes; ++i)
			{
				const unsigned aimesh_index(p_pAiNode->mMeshes[i]);
				const unsigned aimaterial_index(m_pScene->mMeshes[aimesh_index]->mMaterialIndex);
				KG::Mesh_SmartPtr new_mesh_sp(this->InitMesh(m_pScene->mMeshes[aimesh_index], aimaterial_index));
				p_spParentMesh->AddChild(new_mesh_sp);
				KG::Skeleton_SmartPtr skeleton_sp(new KG::Skeleton(KE::EntityIDGenerator::NewID()));
				new_mesh_sp->SetSkeleton(skeleton_sp);
				this->RecursiveInitScene(new_mesh_sp, skeleton_sp, p_pAiNode->mChildren[i]);
			}			
		}
		else
		{
			for (unsigned i = 0; i < p_pAiNode->mNumChildren; ++i)
				this->RecursiveInitScene(p_spParentMesh, p_spSkeleton, p_pAiNode->mChildren[i]);
		}
		
	}

	Mesh_SmartPtr SceneLoader::InitMesh(const aiMesh * const p_pAiMesh, const unsigned p_AiMaterialIndex)
	{
		KE::Debug::print("MeshLoader::InitMesh : New mesh.");
		assert(p_pAiMesh); // cannot be null
		KG::Mesh_SmartPtr mesh_sp(new KG::Mesh());
		mesh_sp->SetName(p_pAiMesh->mName.C_Str());

		// pos vertex
		this->InitPositions(mesh_sp, p_pAiMesh);

		// index
		this->InitFaces(mesh_sp, p_pAiMesh);

		// normal
		this->InitNormals(mesh_sp, p_pAiMesh);

		// material
		this->InitMaterials(mesh_sp, p_AiMaterialIndex);

		// color vertex.
		this->InitColors(mesh_sp, p_pAiMesh);

		// texture coords
		this->InitTexCoords(mesh_sp, p_pAiMesh);

		// load skeleton if there's any.
		this->InitSkeleton(mesh_sp, p_pAiMesh);

		// load all animatins if there's any.
		this->InitAnimations(mesh_sp);

		// load texture
		this->InitMaterialTexture(mesh_sp, p_pAiMesh, m_pScene, m_ScenePath);

		mesh_sp->m_Loaded = true;
		return mesh_sp;
	}

	void SceneLoader::InitPositions(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		if (p_pAiMesh->HasPositions())
		{
			KE::Debug::print("MeshLoader::InitPositions : Init positions.");
			const unsigned num_pos(p_pAiMesh->mNumVertices);
			p_spMesh->m_PosVertices.reserve(num_pos);
			for (int i = 0; i < static_cast<int>(num_pos); ++i)
			{
				const aiVector3D & ai_pos = p_pAiMesh->mVertices[i];
				p_spMesh->m_PosVertices.push_back(glm::vec3(ai_pos.x, ai_pos.y, ai_pos.z));
			}
		}
		else
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "MeshLoader::InitPositions : aiMesh has no position vertices!");
			assert(false);
		}
	}

	void SceneLoader::InitFaces(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		if (p_pAiMesh->HasFaces())
		{
			KE::Debug::print("MeshLoader::InitFaces : Init indices.");
			const unsigned num_faces(p_pAiMesh->mNumFaces);
			//p_spMesh->SetNumIndex(num_faces * 3);
			//p_spMesh->SetNumElement(num_faces * 3);
			p_spMesh->m_Indices.reserve(num_faces * 3);
			for (int i = 0; i < static_cast<int>(num_faces); ++i)
			{
				const aiFace & face = p_pAiMesh->mFaces[i];
				assert(face.mNumIndices == 3);
				for (int j = 0; j < 3; ++j)
					p_spMesh->m_Indices.push_back(face.mIndices[j]);
			}
		}
		else
			KE::Debug::print("MeshLoader::InitNormals : aiMesh has no vertex indices.");
	}

	void SceneLoader::InitNormals(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		if (p_pAiMesh->HasNormals())
		{
			KE::Debug::print("MeshLoader::InitNormals : Init normals.");
			const unsigned num_normal(p_pAiMesh->mNumVertices);
			p_spMesh->m_NormalVertices.reserve(num_normal);
			for (int i = 0; i < static_cast<int>(num_normal); ++i)
			{
				const aiVector3D & ai_normal = p_pAiMesh->mNormals[i];
				p_spMesh->m_NormalVertices.push_back(glm::vec3(ai_normal.x, ai_normal.y, ai_normal.z));
			}
		}
		else
			KE::Debug::print("MeshLoader::InitNormals : aiMesh has no vertex normals.");
	}

	void SceneLoader::InitTexCoords(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		const unsigned texcoord_id = 0;
		if (p_pAiMesh->HasTextureCoords(texcoord_id))
		{
			KE::Debug::print("MeshLoader::InitTexCoords : Init material.");
			const unsigned num_tex_coord(p_spMesh->GetVertices().size());
			p_spMesh->m_TexCoordVertices.reserve(num_tex_coord);
			for (int i = 0; i < static_cast<int>(num_tex_coord); ++i)
			{				
				const aiVector3D & tex_coord = p_pAiMesh->mTextureCoords[texcoord_id][i];
				p_spMesh->m_TexCoordVertices.push_back(glm::vec3(tex_coord.x, tex_coord.y, tex_coord.z));
			}
		}
		else
			KE::Debug::print("MeshLoader::InitTexCoords : aiMesh has no texture coordinates.");
	}

	void SceneLoader::InitColors(KG::Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		const unsigned vc_id = 0; // TODO : ??
		if (p_pAiMesh->HasVertexColors(vc_id))
		{
			KE::Debug::print("MeshLoader::InitColors : Init colors.");
			const unsigned num_colors(p_spMesh->GetVertices().size());
			p_spMesh->m_ColorVertices.reserve(num_colors);
			for (int i = 0; i < static_cast<int>(num_colors); ++i)
			{
				const aiColor4D & color_vert = p_pAiMesh->mColors[vc_id][i];
				p_spMesh->m_ColorVertices.push_back(glm::vec4(color_vert.r, color_vert.g, color_vert.b, color_vert.a));
			}
		}
		else
			KE::Debug::print("MeshLoader::InitColors : aiMesh has no vertex colors.");
	}

	void SceneLoader::InitMaterials(KG::Mesh_SmartPtr p_spMesh, const unsigned p_MaterialIndex)
	{
		if (m_pScene->HasMaterials())
		{
			KE::Debug::print("MeshLoader::InitMaterials : Init texture coords.");
			const aiMaterial * const material_ptr = m_pScene->mMaterials[p_MaterialIndex];
			aiString name;
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_NAME, name))
				p_spMesh->m_Material.Name = name.C_Str();
			float shininess(0.0f);
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_SHININESS, shininess))
				p_spMesh->m_Material.Shininess = shininess;
			aiColor3D ai_vec3;
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_AMBIENT, ai_vec3)) // ambient
				p_spMesh->m_Material.Ambient = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_DIFFUSE, ai_vec3)) // diffuse
				p_spMesh->m_Material.Diffuse = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_SPECULAR, ai_vec3)) // specular
				p_spMesh->m_Material.Specular = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
			if (AI_SUCCESS == material_ptr->Get(AI_MATKEY_COLOR_EMISSIVE, ai_vec3)) // emissive
				p_spMesh->m_Material.Emissive = glm::vec3(ai_vec3.r, ai_vec3.g, ai_vec3.b);
		}
		else
			KE::Debug::print("MeshLoader::InitMaterials : aiScene has no materials.");
	}

	const bool SceneLoader::InitMaterialTexture
		(
			Mesh_SmartPtr p_spMesh
			, const aiMesh * const p_pAiMesh
			, const aiScene * const p_pAiScene
			, const std::string & p_Path
		)
	{
		bool result(true);

		// Extract the directory part from the file name
		const std::string::size_type slash_index(p_Path.find_last_of("/"));
		std::string dir;

		if (slash_index == std::string::npos)
			dir = ".";
		else if (slash_index == 0) 
			dir = "/";
		else 
			dir = p_Path.substr(0, slash_index);

		const int material_index(p_pAiMesh->mMaterialIndex);
		const aiMaterial * const material_ptr(p_pAiScene->mMaterials[material_index]);
		if (!material_ptr)
			return false;

		// load material texture. Only diffuse ATM.
		if (material_ptr->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;
			if ( material_ptr->GetTexture(aiTextureType_DIFFUSE, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr)
					== AI_SUCCESS
				)
			{
				const std::string full_path(dir + "/" + Path.C_Str());
				KG::Texture_SmartPtr texture(new KG::Texture(KG::Texture::DType::Tex2D, full_path));
				p_spMesh->SetTexture(texture);
			}
		}	

		return result; // TODO
	}

	void SceneLoader::InitSkeleton(Mesh_SmartPtr p_spMesh, const aiMesh * const p_pAiMesh)
	{
		if (!p_pAiMesh->HasBones())
		{
			KE::Debug::print("MeshLoader::InitSkeleton : aiMesh has no bones.");
			return; // no bones, leave.
		}

		KE::Debug::print("MeshLoader::InitSkeleton : Init Skeleton.");
		KG::Skeleton_SmartPtr skeleton_sp(new KG::Skeleton(p_spMesh->GetEntityID()));
		
		// get and set global inverse matrix
		skeleton_sp->global_inverse_transform = glm::inverse(this->AiMatToGLMMat(m_pScene->mRootNode->mTransformation));
		
		// convert Bone to Vertices relation to Vertex to bones relation.
		const unsigned num_bones(p_pAiMesh->mNumBones);
		skeleton_sp->ReserveForBones(num_bones);

		// resize Skeleton's intermediate and final transform arrays.
		skeleton_sp->intermediate_transforms.resize(num_bones);
		skeleton_sp->final_transforms.resize(num_bones);

		// create a map which will hold all the vertex to bone weights relationships:
		std::map<unsigned, std::multimap<std::string, float>> vertex_to_bones_map; // <v_index, <bone_name, weight>>
		// pre-fill map with empty entries first.
		for (unsigned vertex_index = 0; vertex_index < p_spMesh->m_PosVertices.size(); ++vertex_index)
			vertex_to_bones_map.insert(std::make_pair(vertex_index, std::multimap<std::string, float>()));

		// collect name, offset, and weights of each bone:
		for (unsigned i = 0; i < num_bones; ++i)
		{
			const aiBone * const ai_bone_ptr(p_pAiMesh->mBones[i]);
			
			// collect bone name for Skeleton (per-bone)
			const std::string bone_name(ai_bone_ptr->mName.C_Str());
			skeleton_sp->bone_names.push_back(bone_name);
			
			// collect offset transform for Skeleton (per-bone)
			skeleton_sp->bone_offsets.push_back(this->AiMatToGLMMat(ai_bone_ptr->mOffsetMatrix));

			// collect bone transform relative to parent (per-bone/aiNode)
			const aiNode * const ai_node_ptr(this->FindAiNodeByName(bone_name, m_pScene->mRootNode));
			if (ai_node_ptr)
				skeleton_sp->bone_transforms.push_back(this->AiMatToGLMMat(ai_node_ptr->mTransformation));
			else
				KE::Debug::print(KE::Debug::DBG_WARNING, "MeshLoader::InitSkeleton : cannot find the corresponding aiNode to the given bone name.");

			// insert bone weights into map. (per-vertex & per-weight)
			for (unsigned weight_i = 0; weight_i < ai_bone_ptr->mNumWeights; ++weight_i)
			{
				const aiVertexWeight & ai_vweight(ai_bone_ptr->mWeights[weight_i]);
				vertex_to_bones_map[ai_vweight.mVertexId].insert(std::make_pair(bone_name, ai_vweight.mWeight));
			}
		}

		for (auto & pair_it  : vertex_to_bones_map) // needs map to be pre-filled to work properly.
		{
			while (pair_it.second.size() > 4) // reduce num of bones for each vertex to 4.
				pair_it.second.erase(pair_it.second.begin());
			while (pair_it.second.size() != 4) // fill up with 0 weights if less than 4 actual bone influences.
				pair_it.second.insert(std::make_pair(p_pAiMesh->mBones[0]->mName.C_Str(), 0.0f)); // use any bone name. It doesn't matter since weight is 0 anyway.
				// NOTE : weights are not automatically normalized to a total of 1 here.
				//			Shouldn't matter if Assimp is already set to limit bones per vertex.
		}

		// fill in Skeleton's ID's and weights vectors for each vertex
		//	also calculate indices for each weight for each vertex.
		unsigned vertex_index(0);
		skeleton_sp->bone_IDs.resize(p_pAiMesh->mNumVertices);		// resize first and then iterate.
		skeleton_sp->bone_weights.resize(p_pAiMesh->mNumVertices);	// resize first and then iterate.
		for (auto & vertex_to_names : vertex_to_bones_map)
		{ // for vertex to 4 x Weights pair
			auto bone_weight_pair_it(vertex_to_names.second.begin()); //--bone_weight_pair_it;

			// 1st weight
			auto bone_name_it(std::find(skeleton_sp->bone_names.begin(), skeleton_sp->bone_names.end(), bone_weight_pair_it->first));
			if (bone_name_it == skeleton_sp->bone_names.end())
				assert(false); // should never fail.
			unsigned bone_index(std::distance(skeleton_sp->bone_names.begin(), bone_name_it));
			skeleton_sp->bone_IDs[vertex_index].x = bone_index;
			skeleton_sp->bone_weights[vertex_index].x = bone_weight_pair_it->second;

			// 2nd weight
			++bone_weight_pair_it;
			bone_name_it = std::find(skeleton_sp->bone_names.begin(), skeleton_sp->bone_names.end(), bone_weight_pair_it->first);
			if (bone_name_it == skeleton_sp->bone_names.end())
				assert(false); // should never fail.
			bone_index = std::distance(skeleton_sp->bone_names.begin(), bone_name_it);
			skeleton_sp->bone_IDs[vertex_index].y = bone_index;
			skeleton_sp->bone_weights[vertex_index].y = bone_weight_pair_it->second;

			// 3rd weight
			++bone_weight_pair_it;
			bone_name_it = std::find(skeleton_sp->bone_names.begin(), skeleton_sp->bone_names.end(), bone_weight_pair_it->first);
			if (bone_name_it == skeleton_sp->bone_names.end())
				assert(false); // should never fail.
			bone_index = std::distance(skeleton_sp->bone_names.begin(), bone_name_it);
			skeleton_sp->bone_IDs[vertex_index].z = bone_index;
			skeleton_sp->bone_weights[vertex_index].z = bone_weight_pair_it->second;

			// 4th weight
			++bone_weight_pair_it;
			bone_name_it = std::find(skeleton_sp->bone_names.begin(), skeleton_sp->bone_names.end(), bone_weight_pair_it->first);
			if (bone_name_it == skeleton_sp->bone_names.end())
				assert(false); // should never fail.
			bone_index = std::distance(skeleton_sp->bone_names.begin(), bone_name_it);
			skeleton_sp->bone_IDs[vertex_index].w = bone_index;
			skeleton_sp->bone_weights[vertex_index].w = bone_weight_pair_it->second;

			++vertex_index;
		} // if has bones

		this->ConstructSkeleton(skeleton_sp, p_pAiMesh, m_pScene->mRootNode);
		p_spMesh->SetSkeleton(skeleton_sp);
	}

	void SceneLoader::InitAnimations(Mesh_SmartPtr p_spMesh)
	{
		assert(p_spMesh);
		assert(m_pScene);
		if (!m_pScene->HasAnimations())
			return; // no animations
		if (!p_spMesh->HasSkeleton())
			return; // this mesh has no skeleton

		// load all aiAnimations
		for (unsigned i = 0; i < m_pScene->mNumAnimations; ++i) // for each aiAnimation
		{ // load each animation into proprietary format.
			KG::Animation_SmartPtr animation_sp(new KG::Animation(p_spMesh->GetEntityID()));
			const aiAnimation * const ai_anim_ptr(m_pScene->mAnimations[i]);

			// get animation frame rate:
			double anim_fps; // ticks per second
			if (ai_anim_ptr->mTicksPerSecond > 0)
			{
				anim_fps = ai_anim_ptr->mTicksPerSecond;
				KE::Debug::print("MeshLoader::InitAnimations : animation ticks/sec = " + std::to_string(anim_fps));
			}
			else
			{
				anim_fps = 1.0;
				KE::Debug::print(KE::Debug::DBG_WARNING, "MeshLoader::InitAnimations : animation framerate not specified. Set to " + std::to_string(anim_fps));
			}

			for (unsigned a = 0; a < ai_anim_ptr->mNumChannels; ++a)
			{ // iterate through each aiNode
				KG::AnimationNode_SmartPtr anim_node_sp(new KG::AnimationNode(p_spMesh->GetEntityID()));
				const aiNodeAnim * const ai_animnode_ptr(ai_anim_ptr->mChannels[a]);

				// compute index to array in Skeleton.
				unsigned index(0);
				const std::string bone_name(ai_animnode_ptr->mNodeName.C_Str());
				anim_node_sp->SetName(bone_name);
				bool bone_found(false);
				for (const std::string & name : p_spMesh->m_spSkeleton->bone_names)
				{
					if (bone_name == name)
					{
						bone_found = true;
						anim_node_sp->m_SkeletonBoneIndex = index;
						break;
					}
					else
						++index;
				}
				if (!bone_found)
				{
					KE::Debug::print(KE::Debug::DBG_ERROR, "MeshLoader::InitAnimations : cannot find matching bone from Skeleton for aiAnimNode.");
					KE::Debug::print(KE::Debug::DBG_ERROR, "	Missing bone name: " + bone_name);
				}

				// collect scaling keys
				for (unsigned node_index = 0; node_index < ai_animnode_ptr->mNumScalingKeys; ++node_index)
				{
					const aiVectorKey & ai_key(ai_animnode_ptr->mScalingKeys[node_index]);
					const glm::dvec3 vec(ai_key.mValue.x, ai_key.mValue.y, ai_key.mValue.z);
					const KE::Duration time(KE::Duration::Seconds(ai_key.mTime/anim_fps));
					const KG::AnimationScaleKey key(time, vec);
					anim_node_sp->m_ScaleKeys.push_back(key);
				}

				// collect translation offset keys
				for (unsigned node_index = 0; node_index < ai_animnode_ptr->mNumPositionKeys; ++node_index)
				{
					const aiVectorKey & ai_key(ai_animnode_ptr->mPositionKeys[node_index]);
					const glm::dvec3 vec(ai_key.mValue.x, ai_key.mValue.y, ai_key.mValue.z);
					const KE::Duration time(KE::Duration::Seconds(ai_key.mTime/anim_fps));
					const KG::AnimationTranslationKey key(time, vec);
					anim_node_sp->m_TranslationKeys.push_back(key);
				}

				// collect rotation keys.
				for (unsigned node_index = 0; node_index < ai_animnode_ptr->mNumRotationKeys; ++node_index)
				{
					const aiQuatKey & ai_key(ai_animnode_ptr->mRotationKeys[node_index]);
					const glm::dquat quat(ai_key.mValue.w, ai_key.mValue.x, ai_key.mValue.y, ai_key.mValue.z);
					const KE::Duration time(KE::Duration::Seconds(ai_key.mTime/anim_fps));
					const KG::AnimationRotationKey key(time, quat);
					anim_node_sp->m_RotationKeys.push_back(key);
				}
				animation_sp->m_Channels.push_back(anim_node_sp);
			} // end for each channel.

			p_spMesh->m_spSkeleton->m_Animations.push_back(animation_sp);
		} // end for each aiAnimation.

	}

	void SceneLoader::ConstructSkeleton(KG::Skeleton_SmartPtr p_spSkeleton, const aiMesh * const p_pAiMesh, const aiNode * const p_AiNode)
	{
		/*
			To reconstruct the bone tree. Build a bone depth map by going through each Bone name and search them in the scene.
			For each bone. calculate the depth from the root. the one with the lowest depth would be the root bone.
		*/

		// get scene root node first (used later).
		const aiNode * const scene_root_node(m_pScene->mRootNode);
		if (!scene_root_node) // check root node first.
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "MeshLoader::ConstructSkeleton : root node is null");
			assert(false);
		}

		//construct a bone depth map
		std::map<std::string, unsigned> bone_depth_map; // bones must have different names.
		for ( auto & bone_name : p_spSkeleton->bone_names )
		{
			unsigned depth(0); // depth starts from 1 at the root AiNode
			if (this->FindBoneDepth(depth, scene_root_node, bone_name))
				bone_depth_map.insert(std::make_pair(bone_name, depth));
			else
				assert(false);
		}

		// find the true root node/bone of the skeleton.
		// NOTE :  This algorithm assumes that all aiNode with the common lowest depth have a common ancestor.
		//			Which should be true at all times.
		unsigned num_current_root(9999);
		unsigned min_depth(9999); // just something bigger than the possible number of bones...
		while ( (num_current_root > 1) || (min_depth > 1) )
		{
			// calculate the top root depth level in the aiScene
			for ( auto & it : bone_depth_map )
				if (it.second < min_depth)
					min_depth = it.second;

			// count number of root.
			num_current_root = 0;
			for ( auto & it : bone_depth_map )
				if (it.second == min_depth)
					++num_current_root;

			// get current root bone/s.
			std::vector<std::string> root_bone_names;
			for ( auto & it : bone_depth_map ) // get all root bone names.
				if (it.second == min_depth)
					root_bone_names.push_back(it.first);

			// move up a level in the scene tree.
			for (const std::string & bone_name : root_bone_names)
			{
				const aiNode * const ainode(this->FindAiNodeByName(bone_name, p_AiNode));
				if (ainode && ainode->mParent)
				{
					const aiNode * const new_parent_ainode(ainode->mParent);
					const std::string bone_name(new_parent_ainode->mName.C_Str());
					if ( std::find(p_spSkeleton->bone_names.begin(), p_spSkeleton->bone_names.end(), bone_name) != p_spSkeleton->bone_names.end() )
					{ // bone already in Skeleton.
						KE::Debug::print(KE::Debug::DBG_WARNING, "MeshLoader::ConstructSkeleton : bone already in skeleton!");
						KE::Debug::print(KE::Debug::DBG_WARNING, "	bone name: " + bone_name);
						continue;
					}
					else
					{
						p_spSkeleton->bone_names.push_back(bone_name);
						p_spSkeleton->bone_offsets.push_back(this->CalculateBoneOffset(new_parent_ainode));
						p_spSkeleton->bone_transforms.push_back(this->AiMatToGLMMat(new_parent_ainode->mTransformation));
						p_spSkeleton->intermediate_transforms.push_back(glm::dmat4());
						p_spSkeleton->final_transforms.push_back(glm::mat4());
					}					
				}
			}

			// rebuild bone depth map
			bone_depth_map.clear();
			for ( auto & bone_name : p_spSkeleton->bone_names )
			{
				unsigned depth(0); // depth starts from 1 at the root AiNode
				if (this->FindBoneDepth(depth, scene_root_node, bone_name))
					bone_depth_map.insert(std::make_pair(bone_name, depth));
				else
					assert(false);
			}
		}

		// compute and confirm the minimum depth (The level where the root bone/s reside).1
		min_depth = 9999; // just something bigger than the possible number of bones...
		for ( auto & it : bone_depth_map )
		{
			if (it.second < min_depth)
				min_depth = it.second;
		}
		// find root bones of skeleton.
		std::vector<std::string> root_bone_names;
		for ( auto & it : bone_depth_map ) // get all root bone names.
		{
			if (it.second == min_depth)
				root_bone_names.push_back(it.first);
		}
		
		if (root_bone_names.size() != 1) // there should only be one root bone at this stage.
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "MeshLoader::ConstructSkeleton : still more than 1 root bones! ");
			assert(false);
		}
		
		// grow bone tree.
		for ( auto & name : root_bone_names ) // there should be only 1.
			this->GrowBoneTree(p_spSkeleton, nullptr, p_pAiMesh, this->FindAiNodeByName(name,  p_AiNode));

		// DEBUG : dump bone heirachy
		/*std::multimap<unsigned, std::string> skeleton_dump_map;
		for ( auto & it : bone_depth_map )
			skeleton_dump_map.insert(std::make_pair(it.second, it.first));
		for ( auto & it : skeleton_dump_map )
		{
			std::string output_dump; output_dump.reserve(30);
			for (unsigned i = 0; i < it.first; ++i)
				output_dump.append(" ");
			output_dump.append(it.second);
			KE::Debug::print(output_dump);
		}*/
		KE::Debug::print("");
		for ( const std::string & bone_name : p_spSkeleton->bone_names)
			KE::Debug::print(bone_name);
		KE::Debug::print("");
	}

	const bool SceneLoader::FindBoneDepth(unsigned & p_Depth, const aiNode * const p_pAiNode, const std::string & p_BoneName)
	{
		++p_Depth;
		const std::string this_bone_name(p_pAiNode->mName.C_Str());
		if (p_BoneName == this_bone_name) // FOUND!!!
			return true;
		else
		{
			for (unsigned i = 0; i < p_pAiNode->mNumChildren; ++i)
			{
				if (this->FindBoneDepth(p_Depth, p_pAiNode->mChildren[i], p_BoneName))
					return true;
				else
					--p_Depth;
			}
		}
		return false;
	}

	const aiNode * const SceneLoader::FindAiNodeByName(const std::string & p_rNodeName, const aiNode * const p_pAiNode)
	{
		if (!p_pAiNode)
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "MeshLoader::FindAiNodeByName : received a nullptr");
			assert(false);
		}
		
		const std::string this_bone_name(p_pAiNode->mName.C_Str());
		if (p_rNodeName == this_bone_name) // FOUND!!!
			return p_pAiNode;
		else
		{
			for (unsigned i = 0; i < p_pAiNode->mNumChildren; ++i)
			{
				const aiNode * const ai_node_ptr = this->FindAiNodeByName(p_rNodeName, p_pAiNode->mChildren[i]);
				if (ai_node_ptr)
					return ai_node_ptr;
			}
		}
		return nullptr;
	}

	void SceneLoader::GrowBoneTree
		(
			KG::Skeleton_SmartPtr p_spSkeleton, KG::BoneNode_SmartPtr p_spBoneNode
			, const aiMesh * const p_pAiMesh, const aiNode * const p_pAiNode
		)
	{
		if (!p_pAiNode) // check valid pointer.
		{
			KE::Debug::print(KE::Debug::DBG_ERROR, "MeshLoader::GrowBoneTree : received a nullptr");
			assert(false);
		}

		// create and fill in bone node
		KG::BoneNode_SmartPtr bone_node_sp(new KG::BoneNode(p_spSkeleton->GetEntityID(), KG::RenderPass::NotRendered));
		const std::string bone_name(p_pAiNode->mName.C_Str());
		bone_node_sp->SetName(bone_name);
		bool bone_found = false;
		for (unsigned i = 0; i < p_pAiMesh->mNumBones; ++i)
		{
			if (bone_name == std::string(p_pAiMesh->mBones[i]->mName.C_Str()))
			{
				bone_found = true;

				// compute index to array in Skeleton.
				unsigned index(0);
				for (const std::string & name : p_spSkeleton->bone_names)
				{
					if (bone_name == name)
					{
						bone_node_sp->skeleton_bone_index = index;
						break;
					}
					else
						++index;
				}

				break; // found
			}
		}
		if (!bone_found) // bone was not specified in aiMesh.
		{
			KE::Debug::print(KE::Debug::DBG_WARNING, "MeshLoader::GrowBoneTree : recovering bone not found in aiMesh!");
			KE::Debug::print(KE::Debug::DBG_WARNING, "	This bone either does not have weight influence on any vertex or it's belonged to something else.");
			KE::Debug::print(KE::Debug::DBG_WARNING, "	- bone name: " + bone_name);

			if ( std::find(p_spSkeleton->bone_names.begin(), p_spSkeleton->bone_names.end(), bone_name) != p_spSkeleton->bone_names.end() )
			{ // bone already in Skeleton. Found in MeshLoader::ConstructSkeleton probably.
				KE::Debug::print(KE::Debug::DBG_WARNING, "MeshLoader::GrowBoneTree : bone already in skeleton!");
				KE::Debug::print(KE::Debug::DBG_WARNING, "	bone name: " + bone_name);
			}
			else
			{ // Reconstruct a bone for the skeleton.
				p_spSkeleton->bone_names.push_back(bone_name);
				p_spSkeleton->bone_transforms.push_back(this->AiMatToGLMMat(p_pAiNode->mTransformation));
				p_spSkeleton->bone_offsets.push_back(this->CalculateBoneOffset(p_pAiNode));
				p_spSkeleton->intermediate_transforms.push_back(glm::dmat4());
				p_spSkeleton->final_transforms.push_back(glm::mat4());
				bone_node_sp->skeleton_bone_index = p_spSkeleton->bone_names.size() - 1;
			}			
		}

		// attach to parent:
		if (p_spBoneNode == nullptr) // if given BoneNode is null then it's a root bone.
			p_spSkeleton->AddChild(bone_node_sp);
		else
			p_spBoneNode->AddChild(bone_node_sp);

		// recursively grow branches:
		for (unsigned i = 0; i < p_pAiNode->mNumChildren; ++i)
			this->GrowBoneTree(p_spSkeleton, bone_node_sp, p_pAiMesh, p_pAiNode->mChildren[i]);
	}

	const glm::dmat4 SceneLoader::CalculateBoneOffset(const aiNode * const p_pAiNode, glm::dmat4 p_Offset)
	{
		if (p_pAiNode)
		{
			glm::dmat4 transform(this->AiMatToGLMMat(p_pAiNode->mTransformation));
			p_Offset = p_Offset * transform; // note: we are traversing back to scene root node from a child. Therefore the multiplication order used.
			return this->CalculateBoneOffset(p_pAiNode->mParent, p_Offset);
		}
		else // reached root node.
			return glm::inverse(p_Offset); // calculate the inverse of the bind pose.
	}

	const glm::dmat4 SceneLoader::AiMatToGLMMat(const aiMatrix4x4 & p_rAiMat)
	{
		glm::dmat4 dmat4;
		dmat4[0][0] = p_rAiMat.a1; dmat4[1][0] = p_rAiMat.a2; dmat4[2][0] = p_rAiMat.a3; dmat4[3][0] = p_rAiMat.a4;
		dmat4[0][1] = p_rAiMat.b1; dmat4[1][1] = p_rAiMat.b2; dmat4[2][1] = p_rAiMat.b3; dmat4[3][1] = p_rAiMat.b4;
		dmat4[0][2] = p_rAiMat.c1; dmat4[1][2] = p_rAiMat.c2; dmat4[2][2] = p_rAiMat.c3; dmat4[3][2] = p_rAiMat.c4;
		dmat4[0][3] = p_rAiMat.d1; dmat4[1][3] = p_rAiMat.d2; dmat4[2][3] = p_rAiMat.d3; dmat4[3][3] = p_rAiMat.d4;
		return dmat4;
	}

} // KG ns
