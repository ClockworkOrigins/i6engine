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
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_GLOWMATERIALLISTENER_H__
#define __I6ENGINE_MODULES_GLOWMATERIALLISTENER_H__

#include "Ogre.h"
#include "OgreMaterialManager.h"

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GLOWMATERIALLISTENER_H__ */

/**
 * @}
 */
