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
