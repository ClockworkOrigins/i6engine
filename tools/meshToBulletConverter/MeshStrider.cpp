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

#include "MeshStrider.h"

#include "i6engine/utils/Exceptions.h"

#include "OGRE/Ogre.h"

namespace i6e {
namespace tools {

	int MeshStrider::getNumSubParts() const {
		int ret = _mesh->getNumSubMeshes();

		if (ret <= 0) {
			ISIXE_THROW_FAILURE("MeshStrider", "Fehler: getNumSumMessages returned " << ret);
		}

		return ret;
	}

	void MeshStrider::getLockedReadOnlyVertexIndexBase(const unsigned char ** vertexbase, int & numverts, PHY_ScalarType & type, int & stride, const unsigned char ** indexbase, int & indexstride, int & numfaces, PHY_ScalarType & indicestype, int subpart) const {
		Ogre::SubMesh * submesh = _mesh->getSubMesh(uint16_t(subpart));

		Ogre::VertexData * vertex_data = submesh->useSharedVertices ? _mesh->sharedVertexData : submesh->vertexData;

		const Ogre::VertexElement * posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

		Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

		*vertexbase = reinterpret_cast<unsigned char *>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
		// as second argument. So make it float, to avoid trouble when Ogre::Real will
		// be comiled/typedefed as float:
		// Ogre::Real* pReal;
		float * pReal;
		posElem->baseVertexPointerToElement(reinterpret_cast<void *>(const_cast<unsigned char *>(*vertexbase)), &pReal);
		*vertexbase = reinterpret_cast<unsigned char *>(pReal);

		stride = int(vbuf->getVertexSize());

		numverts = int(vertex_data->vertexCount);

		if (numverts == 0) {
			ISIXE_THROW_FAILURE("MeshStrider", "Error: numverts is 0");
		}

		type = PHY_FLOAT;

		Ogre::IndexData * index_data = submesh->indexData;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		if (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT) {
			indicestype = PHY_INTEGER;
		} else {
			if (ibuf->getType() != Ogre::HardwareIndexBuffer::IT_16BIT) {
				ISIXE_THROW_FAILURE("MeshStrider", "Error: wrong type: " << ibuf->getType());
			}
			indicestype = PHY_SHORT;
		}

		if (submesh->operationType == Ogre::RenderOperation::OT_TRIANGLE_LIST) {
			numfaces = int(index_data->indexCount) / 3;
			indexstride = int(ibuf->getIndexSize()) * 3;
		} else if (submesh->operationType == Ogre::RenderOperation::OT_TRIANGLE_STRIP) {
			numfaces = int(index_data->indexCount) - 2;
			indexstride = int(ibuf->getIndexSize());
		}

		*indexbase = reinterpret_cast<unsigned char *>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	}

	void MeshStrider::getLockedVertexIndexBase(unsigned char ** vertexbase, int & numverts, PHY_ScalarType & type, int & stride, unsigned char ** indexbase, int & indexstride, int & numfaces, PHY_ScalarType & indicestype, int subpart) {
	}

	void MeshStrider::unLockReadOnlyVertexBase(int subpart) const {
		Ogre::SubMesh * submesh = _mesh->getSubMesh(uint16_t(subpart));

		Ogre::VertexData * vertex_data = submesh->useSharedVertices ? _mesh->sharedVertexData : submesh->vertexData;

		const Ogre::VertexElement * posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

		Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

		vbuf->unlock();

		Ogre::IndexData * index_data = submesh->indexData;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		ibuf->unlock();
	}

	void MeshStrider::unLockVertexBase(int subpart) {
	}

	void MeshStrider::preallocateVertices(int numverts) {
	}

	void MeshStrider::preallocateIndices(int numindices) {
	}

} /* namespace tools */
} /* namespace i6e */
