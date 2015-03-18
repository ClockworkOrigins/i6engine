/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "OgrePrerequisites.h"

#include "OGRE/OgreResourceManager.h"
#include "OgreSingleton.h"
#include "OgreVector3.h"
#include "OgreHardwareBuffer.h"
#include "OgreMesh.h"
#include "OgrePatchMesh.h"

namespace i6engine {
namespace modules {

	class MeshSerializerListener;

	class MeshManager : public Ogre::ResourceManager, public Ogre::ManualResourceLoader {
	public:
		MeshManager();
		~MeshManager();

		void _initialise(void);

		Ogre::ResourceManager::ResourceCreateOrRetrieveResult createOrRetrieveMesh(const Ogre::String & name, const Ogre::String & group, bool isManual = false, Ogre::ManualResourceLoader * loader = nullptr, const Ogre::NameValuePairList * params = nullptr, Ogre::HardwareBuffer::Usage vertexBufferUsage = Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, Ogre::HardwareBuffer::Usage indexBufferUsage = Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, bool vertexBufferShadowed = true, bool indexBufferShadowed = true);

		Ogre::MeshPtr loadMesh(const Ogre::String & filename, const Ogre::String & groupName, Ogre::HardwareBuffer::Usage vertexBufferUsage = Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, Ogre::HardwareBuffer::Usage indexBufferUsage = Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, bool vertexBufferShadowed = true, bool indexBufferShadowed = true);

		void loadResource(Ogre::Resource * res);

	protected:
		Ogre::Resource * createImpl(const Ogre::String & name, Ogre::ResourceHandle handle, const Ogre::String & group, bool isManual, Ogre::ManualResourceLoader * loader, const Ogre::NameValuePairList * createParams);

		enum MeshBuildType {
			MBT_PLANE,
			MBT_CURVED_ILLUSION_PLANE,
			MBT_CURVED_PLANE
		};

		struct MeshBuildParams {
			MeshBuildType type;
			Ogre::Plane plane;
			Ogre::Real width;
			Ogre::Real height;
			Ogre::Real curvature;
			int xsegments;
			int ysegments;
			bool normals;
			unsigned short numTexCoordSets;
			Ogre::Real xTile;
			Ogre::Real yTile;
			Ogre::Vector3 upVector;
			Ogre::Quaternion orientation;
			Ogre::HardwareBuffer::Usage vertexBufferUsage;
			Ogre::HardwareBuffer::Usage indexBufferUsage;
			bool vertexShadowBuffer;
			bool indexShadowBuffer;
			int ySegmentsToKeep;
		};
		
		typedef Ogre::map<Ogre::Resource *, MeshBuildParams>::type MeshBuildParamsMap;
		MeshBuildParamsMap mMeshBuildParams;
	};

} /* namespace modules */
} /* namespace i6engine */
