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

#ifndef __I6ENGINE_MODULES_COMPOSITORS_GAUSSIANBLURLISTENER_H__
#define __I6ENGINE_MODULES_COMPOSITORS_GAUSSIANBLURLISTENER_H__

#include "OGRE/OgreCompositorInstance.h"

namespace i6e {
namespace modules {

	class GaussianListener : public Ogre::CompositorInstance::Listener {
	protected:
		int mVpWidth, mVpHeight;
		// Array params - have to pack in groups of 4 since this is how Cg generates them
		// also prevents dependent texture read problems if ops don't require swizzle
		float mBloomTexWeights[15][4];
		float mBloomTexOffsetsHorz[15][4];
		float mBloomTexOffsetsVert[15][4];

	public:
		GaussianListener();
		virtual ~GaussianListener();
		void notifyViewportSize(int width, int height);
		virtual void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr & mat);
		virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr & mat);
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPOSITORS_GAUSSIANBLURLISTENER_H__ */

/**
 * @}
 */
