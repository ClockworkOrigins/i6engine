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

#ifndef __I6ENGINE_API_LIFETIMECOMPONENT_H__
#define __I6ENGINE_API_LIFETIMECOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	/**
	 * \class LifetimeComponent
	 * \brief destroys the owning game object after given lifetime
	 */
	class ISIXE_MODULES_API LifetimeComponent : public Component {
	public:
		/**
		 * \brief This GameObject will be deleted after lifetime milliseconds after creation.
		 * \param[in] lifetime milliseconds until deletion
		 */
		LifetimeComponent(const int64_t id, const attributeMap & params);

		~LifetimeComponent();

		/**
		 * \brief creates the Component with given attributeMap
		 */
		static ComPtr createC(const int64_t id, const attributeMap & params);

		/**
		 * \brief Called when the time for this GameObject is over.
		 */
		void dead();

		/**
		 * \brief Instant kill
		 */
		void instantKill();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Lifetime";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		uint64_t _startTime;

		/**
		 * ticks left until the object will be removed
		 */
		uint64_t _lifetime;

		void Init() override;

		void Finalize() override;

		/**
		 * \brief ticking the component
		 */
		void Tick() override;

		LifetimeComponent(const LifetimeComponent &) = delete;
		const LifetimeComponent & operator=(const LifetimeComponent &) = delete;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_LIFETIMECOMPONENT_H__ */

/**
 * @}
 */
