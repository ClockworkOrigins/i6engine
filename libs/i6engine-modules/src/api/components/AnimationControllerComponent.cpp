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

#include "i6engine/api/components/AnimationControllerComponent.h"

#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
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

	std::pair<AddStrategy, int64_t> AnimationControllerComponent::howToAdd(const ComPtr & comp) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> AnimationControllerComponent::getComponentOptions() {
		std::vector<componentOptions> result;
		return result;
	}

} /* namespace api */
} /* namespace i6engine */
