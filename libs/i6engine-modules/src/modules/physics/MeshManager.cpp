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

#include "OgreStableHeaders.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/modules/physics/MeshManager.h"

#include "OgreMesh.h"
#include "OgreSubMesh.h"
#include "OgreMatrix4.h"
#include "OgreMatrix3.h"
#include "OgreVector3.h"
#include "OgrePlane.h"
#include "OgreHardwareBufferManager.h"
#include "OgrePatchSurface.h"
#include "OgreException.h"

#include "OgrePrefabFactory.h"

namespace i6engine {
namespace modules {

	MeshManager::MeshManager() {
	}
	//-----------------------------------------------------------------------
	MeshManager::~MeshManager() {
	}
	//-----------------------------------------------------------------------
	void MeshManager::_initialise(void) {
	}
	//-----------------------------------------------------------------------
	Ogre::ResourceManager::ResourceCreateOrRetrieveResult MeshManager::createOrRetrieveMesh(const Ogre::String & name, const Ogre::String & group, bool isManual, Ogre::ManualResourceLoader * loader, const Ogre::NameValuePairList * params, Ogre::HardwareBuffer::Usage vertexBufferUsage, Ogre::HardwareBuffer::Usage indexBufferUsage, bool vertexBufferShadowed, bool indexBufferShadowed) {
		Ogre::ResourceManager::ResourceCreateOrRetrieveResult res = Ogre::ResourceManager::createOrRetrieve(name, group, isManual, loader, params);
		Ogre::MeshPtr pMesh = res.first;
		// Was it created?
		if (res.second) {
			pMesh->setVertexBufferPolicy(vertexBufferUsage, vertexBufferShadowed);
			pMesh->setIndexBufferPolicy(indexBufferUsage, indexBufferShadowed);
		}
		return res;

	}

	//-----------------------------------------------------------------------
	Ogre::MeshPtr MeshManager::loadMesh(const Ogre::String & filename, const Ogre::String& groupName, Ogre::HardwareBuffer::Usage vertexBufferUsage, Ogre::HardwareBuffer::Usage indexBufferUsage, bool vertexBufferShadowed, bool indexBufferShadowed) {
		Ogre::MeshPtr pMesh = createOrRetrieveMesh(filename, groupName, false, 0, 0, vertexBufferUsage, indexBufferUsage, true, true).first;
		pMesh->load();
		return pMesh;
	}

	//-----------------------------------------------------------------------
	void MeshManager::loadResource(Ogre::Resource * res) {
		Ogre::Mesh * msh = static_cast<Ogre::Mesh *>(res);

		// attempt to create a prefab mesh
		bool createdPrefab = Ogre::PrefabFactory::createPrefab(msh);
		if (!createdPrefab) {
			ISIXE_LOG_ERROR("MeshManager", "Could not create Prefab");
		}
	}

	//-----------------------------------------------------------------------
	Ogre::Resource * MeshManager::createImpl(const Ogre::String & name, Ogre::ResourceHandle handle,
		const Ogre::String & group, bool isManual, ManualResourceLoader* loader,
		const Ogre::NameValuePairList * createParams) {
		// no use for createParams here
		return OGRE_NEW Ogre::Mesh(this, name, handle, group, isManual, loader);
	}
	//-----------------------------------------------------------------------

} /* namespace modules */
} /* namespace i6engine */
