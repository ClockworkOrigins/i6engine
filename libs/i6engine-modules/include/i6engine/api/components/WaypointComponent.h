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

#ifndef __I6ENGINE_API_WAYPOINTCOMPONENT_H__
#define __I6ENGINE_API_WAYPOINTCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \class WaypointComponent
	 * \brief Represents a Waypoint in the world, containing an identifier and a list of connections
	 * For creating a WaypointComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | name | yes | std::string | name of this waypoint | yes |
	 * | connections | yes | std::string | names of the waypoints this one is connected to, seperated by delimitter ; | yes |
	 */
	class ISIXE_MODULES_API WaypointComponent : public Component {
	public:
		WaypointComponent(const int64_t id, const attributeMap & params);
		~WaypointComponent();

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
} /* namespace i6e */

#endif /* __I6ENGINE_API_WAYPOINTCOMPONENT_H__ */

/**
 * @}
 */
