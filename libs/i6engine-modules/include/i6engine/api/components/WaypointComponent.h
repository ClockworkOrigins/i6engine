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

#ifndef __I6ENGINE_API_WAYPOINTCOMPONENT_H__
#define __I6ENGINE_API_WAYPOINTCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	/**
	 * \class WaypointComponent
	 * \brief Represents a Waypoint in the world, containing an identifier and a list of connections
	 */
	class ISIXE_MODULES_API WaypointComponent : public Component {
	public:
		WaypointComponent(const int64_t id, const attributeMap & params);
		~WaypointComponent();

		/**
		 * \brief creates the Component with given attributeMap
		 */
		static ComPtr createC(const int64_t id, const attributeMap & params);

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Waypoint";
		}

		std::vector<componentOptions> getComponentOptions() override;

		std::string getName() const {
			return _name;
		}

		/**
		 * \brief returns a vector with all connected Waypoints
		 */
		std::vector<std::string> getConnections() const {
			return _connections;
		}

		/**
		 * \brief returns true, if a connection to given Waypoint exists
		 */
		bool isConnected(const std::string & name) const;

		/**
		 * \brief adds connection to given Waypoint
		 */
		void addConnection(const std::string & name);

		/**
		 * \brief removes connection to given Waypoint
		 */
		void removeConnection(const std::string & name);

	private:
		std::string _name;
		std::vector<std::string> _connections;

		void Init() override;

		void Finalize() override;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_WAYPOINTCOMPONENT_H__ */

/**
 * @}
 */
