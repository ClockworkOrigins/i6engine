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

#ifndef __I6ENGINE_API_MOVINGCAMERACOMPONENT_H__
#define __I6ENGINE_API_MOVINGCAMERACOMPONENT_H__

#include "i6engine/api/components/CameraComponent.h"

namespace i6engine {
namespace api {

	/**
	 * \class MovingCameraComponent
	 * \brief Attaches a moving camera to an object.
	 * This camera will follow the object in a more sophisticated way.
	 */
	class ISIXE_MODULES_API MovingCameraComponent : public CameraComponent {
	public:
		MovingCameraComponent(const int64_t id, const attributeMap & params);
		~MovingCameraComponent();

		static ComPtr createC(const int64_t id, const attributeMap & params);

		void Init() override;

		void Finalize() override;

		void Tick() override;

		std::string getTemplateName() const override {
			return "MovingCamera";
		}

	private:
		Vec3 _oldPos;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_MOVINGCAMERACOMPONENT_H__ */

/**
 * @}
 */
