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

#include "i6engine/modules/graphics/compositors/GaussianBlurListener.h"

#include "OGRE/OgreMaterial.h"
#include "OGRE/OgreTechnique.h"

namespace i6engine {
namespace modules {

	GaussianListener::GaussianListener() : Ogre::CompositorInstance::Listener(), mVpWidth(), mVpHeight(), mBloomTexWeights(), mBloomTexOffsetsHorz(), mBloomTexOffsetsVert() {
	}

	GaussianListener::~GaussianListener() {
	}

	void GaussianListener::notifyViewportSize(int width, int height) {
		mVpWidth = width;
		mVpHeight = height;
		// Calculate gaussian texture offsets & weights
		float deviation = 3.0f;
		float texelSize = 1.0f / (float) std::min(mVpWidth, mVpHeight);

		// central sample, no offset
		mBloomTexOffsetsHorz[0][0] = 0.0f;
		mBloomTexOffsetsHorz[0][1] = 0.0f;
		mBloomTexOffsetsVert[0][0] = 0.0f;
		mBloomTexOffsetsVert[0][1] = 0.0f;
		mBloomTexWeights[0][0] = mBloomTexWeights[0][1] = mBloomTexWeights[0][2] = Ogre::Math::gaussianDistribution(0, 0, deviation);
		mBloomTexWeights[0][3] = 1.0f;

		// 'pre' samples
		for (int i = 1; i < 8; ++i) {
			mBloomTexWeights[i][0] = mBloomTexWeights[i][1] = mBloomTexWeights[i][2] = Ogre::Math::gaussianDistribution((Ogre::Real)i, 0, deviation);
			mBloomTexWeights[i][3] = 1.0f;
			mBloomTexOffsetsHorz[i][0] = i * texelSize;
			mBloomTexOffsetsHorz[i][1] = 0.0f;
			mBloomTexOffsetsVert[i][0] = 0.0f;
			mBloomTexOffsetsVert[i][1] = i * texelSize;
		}
		// 'post' samples
		for (int i = 8; i < 15; ++i) {
			mBloomTexWeights[i][0] = mBloomTexWeights[i][1] = mBloomTexWeights[i][2] = mBloomTexWeights[i - 7][0];
			mBloomTexWeights[i][3] = 1.0f;

			mBloomTexOffsetsHorz[i][0] = -mBloomTexOffsetsHorz[i - 7][0];
			mBloomTexOffsetsHorz[i][1] = 0.0f;
			mBloomTexOffsetsVert[i][0] = 0.0f;
			mBloomTexOffsetsVert[i][1] = -mBloomTexOffsetsVert[i - 7][1];
		}
	}

	void GaussianListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr & mat) {
		// Prepare the fragment params offsets
		switch (pass_id) {
		case 701: // blur horz
		{
			// horizontal bloom
			mat->load();
			Ogre::GpuProgramParametersSharedPtr fparams = mat->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
			fparams->setNamedConstant("sampleOffsets", mBloomTexOffsetsHorz[0], 15);
			fparams->setNamedConstant("sampleWeights", mBloomTexWeights[0], 15);

			break;
		}
		case 700: // blur vert
		{
			// vertical bloom
			mat->load();
			Ogre::GpuProgramParametersSharedPtr fparams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
			fparams->setNamedConstant("sampleOffsets", mBloomTexOffsetsVert[0], 15);
			fparams->setNamedConstant("sampleWeights", mBloomTexWeights[0], 15);

			break;
		}
		default: {
			break;
		}
		}
	}

	void GaussianListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr & mat) {
	}

} /* namespace modules */
} /* namespace i6engine */
