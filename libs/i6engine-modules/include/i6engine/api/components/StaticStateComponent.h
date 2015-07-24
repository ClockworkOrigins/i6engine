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

#ifndef __I6ENGINE_API_STATICSTATECOMPONENT_H__
#define __I6ENGINE_API_STATICSTATECOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	/**
	 * \class StaticStateComponent
	 * \brief Makes it possible to place a static gameobject
	 */
	class ISIXE_MODULES_API StaticStateComponent : public Component {
	public:
		/**
		 * \brief Constructor
		 */
		StaticStateComponent(const int64_t id, const attributeMap & params);

		~StaticStateComponent();

		/**
		 * \brief creates the Component with given attributeMap
		 */
		static ComPtr createC(const int64_t id, const attributeMap & params);

		/**
		 * \brief Sets position
		 */
		inline void setPosition(const Vec3 & position) {
			_position = position;
			sendUpdateMessage();
		}

		/**
		 * \brief Sets rotation
		 */
		inline void setRotation(const Quaternion & rotation) {
			_rotation = rotation;
			sendUpdateMessage();
		}

		/**
		 * \brief Sets scale
		 */
		inline void setScale(const Vec3 & scale) {
			_scale = scale;
			sendUpdateMessage();
		}

		/**
		 * \brief Gets position
		 */
		inline Vec3 getPosition() const {
			return _position;
		}

		/**
		 * \brief Gets rotation
		 */
		inline Quaternion getRotation() const { return _rotation; }

		/**
		 * \brief Gets scale
		 */
		inline Vec3 getScale() const {
			return _scale;
		}

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "StaticState";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		/**
		 * \brief Stores the position of the object.
		 */
		Vec3 _position;

		/**
		 * \brief Stores the rotation of the object.
		 */
		Quaternion _rotation;

		/**
		 * \brief Stores the scale of the object.
		 */
		Vec3 _scale;

		/**
		 * \brief Initialized the Component
		 */
		void Init() override;

		void sendUpdateMessage();
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_STATICSTATECOMPONENT_H__ */

/**
 * @}
 */
