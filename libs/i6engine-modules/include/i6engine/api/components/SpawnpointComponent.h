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

#ifndef __I6ENGINE_API_SPAWNPOINTCOMPONENT_H__
#define __I6ENGINE_API_SPAWNPOINTCOMPONENT_H__

#include <vector>

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	/**
	* \brief Only defines this object as a spawnpoint
	*/
	class ISIXE_MODULES_API SpawnpointComponent : public Component {
	public:
		/**
		* \brief Constructor
		*/
		SpawnpointComponent(const int64_t id, const attributeMap & params);
		~SpawnpointComponent();

		/**
		* \brief creates the Component with given attributeMap
		*/
		static ComPtr createC(const int64_t id, const attributeMap & params);

		/**
		 * \brief Adds an object type to spawn on this component
		 *
		 * \param[in] type Type of the object being able to be spawned on the component
		 */
		inline void addSpawntype(const std::string & type) { _spawntypes.push_back(type); }

		/**
		 * \brief Adds object types to spawn on this component
		 *
		 * \param[in] types Types of the objects being able to be spawned on the component (type1;type2;type3;)
		 */
		void addSpawntypes(const std::string & types);

		/**
		 * \brief Removes the given type from the spawn list
		 *
		 * \param[in] type Type of the object being removed from the list
		 */
		void removeSpawntype(const std::string & type);

		/**
		 * \brief Return wether the given type is contained in the list (true) or not (false)
		 *
		 * \return True if the type can be respawned, otherwise else
		 */
		bool containsSpawntype(const std::string & type) const;

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		/**
		 * \brief returns true, if this spawnpoint currently can be used
		 */
		bool available() const { return _state; }

		/**
		 * \brief sets the state of the Spawnpoint
		 */
		void setState(bool b);

		std::string getTemplateName() const override {
			return "Spawnpoint";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		std::vector<std::string> _spawntypes;
		bool _state;

		void Init() override;

		/**
		 * \brief Receives messages for this component
		 */
		void News(const GameMessage::Ptr & msg) override;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_SPAWNPOINTCOMPONENT_H__ */

/**
 * @}
 */
