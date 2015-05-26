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

#ifndef __I6ENGINE_API_SHATTERCOMPONENT_H__
#define __I6ENGINE_API_SHATTERCOMPONENT_H__

#include "i6engine/utils/sharedPtr.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	typedef utils::sharedPtr<GameObject, GameObject> GOPtr;

	/**
	 * \class ShatterComponent
	 * \brief This class is needed on every GameObject with: TRIGGER
	 * The \ref shatter() method is called for detected collisions
	 */
	class ISIXE_MODULES_API ShatterComponent : public Component {
	public:
		/**
		 * \brief Constructor of the component with a given id
		 */
		ShatterComponent(int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor of the component
		 */
		virtual ~ShatterComponent();

		/**
		 * \brief Initializes the Component
		 */
		virtual void Init() override;

		inline void resetRespawn() { _respawn = 0; }

		/**
		 * \brief Is called after a collision is detected
		 * \param[in] other GameObject you collided with
		 */
		virtual void shatter(const GOPtr & other) = 0;

		/**
		 * \brief Receives messages for this component
		 */
		virtual void News(const GameMessage::Ptr & msg) override;

		/**
		 * \brief synchronizes the Components state
		 */
		virtual attributeMap synchronize() const override;

	private:
		/**
		 * \brief This Method resets the respawn counter
		 */
		ShatterComponent(const ShatterComponent &) = delete;
		const ShatterComponent & operator=(const ShatterComponent &) = delete;

		uint32_t _respawn;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_SHATTERCOMPONENT_H__ */

/**
 * @}
 */
