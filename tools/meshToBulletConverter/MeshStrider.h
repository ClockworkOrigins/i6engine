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

/**
 * \addtogroup Tools
 * @{
 */

#ifndef __I6ENGINE_TOOLS_MESHSTRIDER_H__
#define __I6ENGINE_TOOLS_MESHSTRIDER_H__

#include "i6engine/i6engineBuildSettings.h"

#include "BulletCollision/CollisionShapes/btStridingMeshInterface.h"

namespace Ogre {
	class Mesh;
} /* namespace Ogre */

namespace i6e {
namespace tools {

	/**
	 * Translates a Mesh into a Physics Object.
	 * \todo Use less magic.
	 */
	class MeshStrider : public btStridingMeshInterface {
	public:
		/**
		 * \brief constructor
		 */
		explicit MeshStrider(Ogre::Mesh * m) : _mesh(m) {}

		/**
		 * \brief sets the mesh
		 */
		void set(Ogre::Mesh * m) {
			if (m == nullptr) {
				return;
			}

			_mesh = m;
		}

		/**
		 * \returns Number of Sub meshes.
		 * \throws I6_FAILURE When return-value would be less than 0.
		 */
		int getNumSubParts() const;

		/**
		 * \deprecated Unimplemented Method
		 * Only exists because an interface says so.
		 */
		void getLockedVertexIndexBase(unsigned char ** vertexbase, int & numverts, PHY_ScalarType & type, int & stride, unsigned char ** indexbase, int & indexstride, int & numfaces, PHY_ScalarType & indicestype, int subpart = 0);

		/**
		 * \throws I6_FAILURE when something bad happens.
		 */
		void getLockedReadOnlyVertexIndexBase(const unsigned char ** vertexbase, int & numverts, PHY_ScalarType & type, int & stride, const unsigned char ** indexbase, int & indexstride, int & numfaces, PHY_ScalarType & indicestype, int subpart = 0) const;

		/**
		 * \deprecated Unimplemented Method
		 * Only exists because an interface says so.
		 */
		void unLockVertexBase(int subpart);
		void unLockReadOnlyVertexBase(int subpart) const;

		/**
		 * \deprecated Unimplemented Method
		 * Only exists because an interface says so.
		 */
		void preallocateVertices(int numverts);

		/**
		 * \deprecated Unimplemented Method
		 * Only exists because an interface says so.
		 */
		void preallocateIndices(int numindices);

	private:
		Ogre::Mesh * _mesh;

		/**
		 * \brief forbidden
		 */
		MeshStrider(const MeshStrider &);

		/**
		 * \brief forbidden
		 */
		const MeshStrider & operator=(const MeshStrider &);
	};

} /* namespace tools */
} /* namespace i6e */

#endif /* __I6ENGINE_TOOLS_MESHSTRIDER_H__ */

/**
 * @}
 */
