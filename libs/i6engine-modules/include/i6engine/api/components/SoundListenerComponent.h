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

#ifndef __I6ENGINE_API_SOUNDLISTENERCOMPONENT_H__
#define __I6ENGINE_API_SOUNDLISTENERCOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	class PhysicalStateComponent;

	/**
	 * \class SoundListenerComponent
	 * \brief Attaches a sound listener to an object. The Listener will follow the object
	 */
	class ISIXE_MODULES_API SoundListenerComponent : public Component {
	public:
		/**
		 * \brief Constructor with attribute map
		 */
		explicit SoundListenerComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor
		 */
		~SoundListenerComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "SoundListener";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}

	private:
		/**
		 * \brief Position of the camera
		 */
		Vec3 _position;

		/**
		 * \brief Point the Camera looks at
		 */
		Quaternion _rotation;

		/**
		 * \brief weakPtr on PhysicalStateComponent for faster access
		 */
		utils::weakPtr<PhysicalStateComponent, Component> _psc;

		/**
		 * \brief initializes the component
		 */
		void Init() override;

		void Finalize() override;

		void Tick() override;

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_SOUNDLISTENERCOMPONENT_H__ */

/**
 * @}
 */
