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

#include "i6engine/modules/graphics/compositors/HDRLogic.h"

#include "i6engine/modules/graphics/compositors/HDRListener.h"

#include "OGRE/OgreCompositorChain.h"

namespace i6engine {
namespace modules {

	Ogre::CompositorInstance::Listener * HDRLogic::createListener(Ogre::CompositorInstance * instance) {
		HDRListener * listener = new HDRListener();
		Ogre::Viewport * vp = instance->getChain()->getViewport();
		listener->notifyViewportSize(vp->getActualWidth(), vp->getActualHeight());
		listener->notifyCompositor(instance);
		return listener;
	}

} /* namespace modules */
} /* namespace i6engine */
