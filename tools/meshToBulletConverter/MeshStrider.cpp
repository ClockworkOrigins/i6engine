/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
