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
 * \addtogroup API
 * @{
 */

#ifndef __I6ENGINE_API_WAYNETMANAGER_H__
#define __I6ENGINE_API_WAYNETMANAGER_H__

#include <map>
#include <string>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

namespace i6e {
namespace api {

	class EngineController;

	/**
	 * \brief handles the current Waynet
	 */
	class ISIXE_MODULES_API WaynetManager {
		friend class EngineController;

		struct Edge {
			double length;
			std::string targetWP;
			Vec3 startPosition;
			Vec3 targetPosition;
			Quaternion targetRotation;
		};

	public:
		WaynetManager();

		/**
		 * \brief recreates waynet out of all found waypoints
		 */
		void createWaynet();

		/**
		 * \brief returns the shortest path between startPos and targetWP
		 */
		std::vector<Vec3> getShortestPath(const Vec3 & startPos, const std::string & targetWP);

		/**
		 * \brief shows waynet (has to be called in each tick because lines are resetted after one graphics tick)
		 */
		void showWaynet() const;

		/**
		 * \brief returns the waypoint name being next to the position
		 */
		std::string getNearestWaypoint(const Vec3 & pos) const;

	private:
		std::map<std::string, std::vector<Edge>> _waypoints;
		// first pair is start and end waypoint
		std::map<std::pair<std::string, std::string>, std::vector<Vec3>> _paths;

		void reset();

		std::vector<Vec3> getShortestPath(const std::string & startWP, const std::string & targetWP);
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_WAYNETMANAGER_H__ */

/**
 * @}
 */
