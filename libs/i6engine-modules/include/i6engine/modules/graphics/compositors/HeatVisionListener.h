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

#ifndef __I6ENGINE_MODULES_COMPOSITORS_HEATVISIONLISTENER_H__
#define __I6ENGINE_MODULES_COMPOSITORS_HEATVISIONLISTENER_H__

#include "OGRE/OgreCompositorInstance.h"

namespace i6e {
namespace modules {

	class HeatVisionListener : public Ogre::CompositorInstance::Listener {
	public:
		HeatVisionListener();
		virtual ~HeatVisionListener();
		virtual void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr & mat);
		virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr & mat);
	protected:
		Ogre::GpuProgramParametersSharedPtr fpParams;
		float start, end, curr;
		Ogre::Timer * timer;
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPOSITORS_HEATVISIONLISTENER_H__ */

/**
 * @}
 */
