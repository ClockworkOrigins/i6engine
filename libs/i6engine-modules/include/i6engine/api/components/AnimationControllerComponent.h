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
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_ANIMATIONCONTROLLERCOMPONENT_H__
#define __I6ENGINE_API_ANIMATIONCONTROLLERCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	class MeshAppearanceComponent;

	/**
	 * \class AnimationControllerComponent
	 * \brief Stores different animation events for animations and registers them again when starting an animation
	 */
	class ISIXE_MODULES_API AnimationControllerComponent : public Component {
	public:
		/**
		 * \brief Constructor of the component
		 *
		 * Sets visibility and ID
		 */
		AnimationControllerComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor
		 */
		~AnimationControllerComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "AnimationController";
		}

		/**
		 * \brief adds an event being triggered, when a specified frame time of the animation is reached
		 */
		void addAnimationFrameEvent(const std::string & animation, uint64_t frameTime, const std::function<void(void)> & func);

		/**
		 * \brief plays given animation
		 */
		void playAnimation(const std::string & anim, bool looping, double offsetPercent = 0.0) const;

		/**
		 * \brief set animation speed for current animation
		 */
		void setAnimationSpeed(double animationSpeed) const;

		/**
		 * \brief stops current animation
		 */
		void stopAnimation() const;

	private:
		std::map<std::string, std::map<uint64_t, std::function<void(void)>>> _animationMap;
		utils::weakPtr<MeshAppearanceComponent, Component> _meshComponent;

		void Init() override;

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;

		std::vector<componentOptions> getComponentOptions() override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_ANIMATIONCONTROLLERCOMPONENT_H__ */

/**
 * @}
 */
