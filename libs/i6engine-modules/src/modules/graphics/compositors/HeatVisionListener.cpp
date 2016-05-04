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
