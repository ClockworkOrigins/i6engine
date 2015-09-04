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

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_TOOLS_MESHSTRIDER_H__ */

/**
 * @}
 */
