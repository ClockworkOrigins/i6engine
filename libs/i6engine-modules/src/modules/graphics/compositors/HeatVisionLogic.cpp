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

#include "i6engine/modules/graphics/compositors/HeatVisionLogic.h"

#include "i6engine/modules/graphics/compositors/HeatVisionListener.h"

namespace i6engine {
namespace modules {

	Ogre::CompositorInstance::Listener * HeatVisionLogic::createListener(Ogre::CompositorInstance * instance) {
		return new HeatVisionListener();
	}

} /* namespace modules */
} /* namespace i6engine */