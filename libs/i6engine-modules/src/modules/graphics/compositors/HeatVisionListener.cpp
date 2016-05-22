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

#include "i6engine/modules/graphics/compositors/HeatVisionListener.h"

#include "OGRE/OgreMaterial.h"
#include "OGRE/OgreTechnique.h"
#include "OGRE/OgreTimer.h"

namespace i6e {
namespace modules {

	HeatVisionListener::HeatVisionListener() {
		timer = new Ogre::Timer();
		start = end = curr = 0.0f;
	}

	HeatVisionListener::~HeatVisionListener() {
		delete timer;
	}

	void HeatVisionListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr & mat) {
		if (pass_id == 0xDEADBABE) {
			timer->reset();
			fpParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
		}
	}

	void HeatVisionListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr & mat) {
		if (pass_id == 0xDEADBABE) {
			// "random_fractions" parameter
			fpParams->setNamedConstant("random_fractions", Ogre::Vector4(Ogre::Math::RangeRandom(0.0, 1.0), Ogre::Math::RangeRandom(0, 1.0), 0, 0));

			// "depth_modulator" parameter
			float inc = float(timer->getMilliseconds()) / 1000.0f;
			if ((fabs(curr - end) <= 0.001)) {
				// take a new value to reach
				end = Ogre::Math::RangeRandom(0.95, 1.0);
				start = curr;
			} else {
				if (curr > end) curr -= inc;
				else curr += inc;
			}
			timer->reset();

			fpParams->setNamedConstant("depth_modulator", Ogre::Vector4(curr, 0, 0, 0));
		}
	}

} /* namespace modules */
} /* namespace i6e */
