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

#include "i6engine/api/components/AnimationControllerComponent.h"

#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace api {

	AnimationControllerComponent::AnimationControllerComponent(const int64_t id, const attributeMap & params) : Component(id, params), _animationMap(), _meshComponent() {
		Component::_objFamilyID = components::AnimationControllerComponent;
		Component::_objComponentID = components::AnimationControllerComponent;
	}

	AnimationControllerComponent::~AnimationControllerComponent() {
	}

	void AnimationControllerComponent::Init() {
		_meshComponent = getOwnerGO()->getGOC<MeshAppearanceComponent>(components::ComponentTypes::MeshAppearanceComponent);
	}

	attributeMap AnimationControllerComponent::synchronize() const {
		attributeMap params;
		return params;
	}

	void AnimationControllerComponent::addAnimationFrameEvent(const std::string & animation, uint64_t frameTime, const std::function<void(void)> & func) {
		_animationMap[animation].insert(std::make_pair(frameTime, func));
	}

	void AnimationControllerComponent::playAnimation(const std::string & anim, bool looping, double offsetPercent) const {
		auto meshComponent = _meshComponent.get();
		if (meshComponent != nullptr) {
			meshComponent->playAnimation(anim, looping, offsetPercent);
			auto it = _animationMap.find(anim);
			if (it != _animationMap.end()) {
				for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
					meshComponent->addAnimationFrameEvent(it2->first, it2->second);
				}
			}
		}
	}

	void AnimationControllerComponent::setAnimationSpeed(double animationSpeed) const {
		auto meshComponent = _meshComponent.get();
		if (meshComponent != nullptr) {
			meshComponent->setAnimationSpeed(animationSpeed);
		}
	}

	void AnimationControllerComponent::stopAnimation() const {
		auto meshComponent = _meshComponent.get();
		if (meshComponent != nullptr) {
			meshComponent->stopAnimation();
		}
	}

	std::pair<AddStrategy, int64_t> AnimationControllerComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> AnimationControllerComponent::getComponentOptions() {
		std::vector<componentOptions> result;
		return result;
	}

} /* namespace api */
} /* namespace i6e */
