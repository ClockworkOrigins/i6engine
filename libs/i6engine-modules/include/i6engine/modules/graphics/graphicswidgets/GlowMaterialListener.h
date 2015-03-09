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
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_GLOWMATERIALLISTENER_H__
#define __I6ENGINE_MODULES_GLOWMATERIALLISTENER_H__

#include "Ogre.h"
#include "OgreMaterialManager.h"

namespace i6engine {
namespace modules {

	class GlowMaterialListener : public Ogre::MaterialManager::Listener {
	public:
		GlowMaterialListener() : _mBlackMat() {
			_mBlackMat = Ogre::MaterialManager::getSingleton().create("mGlowBlack", "Internal");
			_mBlackMat->getTechnique(0)->getPass(0)->setDiffuse(0, 0, 0, 0);
			_mBlackMat->getTechnique(0)->getPass(0)->setSpecular(0, 0, 0, 0);
			_mBlackMat->getTechnique(0)->getPass(0)->setAmbient(0, 0, 0);
			_mBlackMat->getTechnique(0)->getPass(0)->setSelfIllumination(0, 0, 0);
		}

		Ogre::Technique * handleSchemeNotFound(unsigned short, const Ogre::String & schemeName, Ogre::Material * mat, unsigned short, const Ogre::Renderable *) {
			if (schemeName == "glow") {
				return _mBlackMat->getTechnique(0);
			}
			return nullptr;
		}

	protected:
		Ogre::MaterialPtr _mBlackMat;

	private:
		/**
		 * \brief forbidden
		 */
		GlowMaterialListener(const GlowMaterialListener &);

		/**
		 * \brief forbidden
		 */
		GlowMaterialListener & operator=(const GlowMaterialListener &);
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GLOWMATERIALLISTENER_H__ */

/**
 * @}
 */
