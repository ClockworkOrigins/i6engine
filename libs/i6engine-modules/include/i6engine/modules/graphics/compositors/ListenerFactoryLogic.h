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

#ifndef __I6ENGINE_MODULES_COMPOSITORS_LISTENERFACTORYLOGIC_H__
#define __I6ENGINE_MODULES_COMPOSITORS_LISTENERFACTORYLOGIC_H__

#include "OGRE/OgreCompositorLogic.h"
#include "OGRE/OgreCompositorInstance.h"

namespace i6engine {
namespace modules {

	class ListenerFactoryLogic : public Ogre::CompositorLogic {
	public:
		virtual void compositorInstanceCreated(Ogre::CompositorInstance * newInstance) {
			Ogre::CompositorInstance::Listener * listener = createListener(newInstance);
			newInstance->addListener(listener);
			mListeners[newInstance] = listener;
		}

		virtual void compositorInstanceDestroyed(Ogre::CompositorInstance* destroyedInstance) {
			delete mListeners[destroyedInstance];
			mListeners.erase(destroyedInstance);
		}

	protected:
		// This is the method that implementations will need to override
		virtual Ogre::CompositorInstance::Listener * createListener(Ogre::CompositorInstance * instance) = 0;

	private:
		typedef std::map<Ogre::CompositorInstance *, Ogre::CompositorInstance::Listener *> ListenerMap;
		ListenerMap mListeners;

	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_COMPOSITORS_LISTENERFACTORYLOGIC_H__ */

/**
 * @}
 */
