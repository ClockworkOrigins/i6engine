#ifndef __I6ENGINE_API_WAYNETMANAGER_H__
#define __I6ENGINE_API_WAYNETMANAGER_H__

#include <map>
#include <string>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

namespace i6engine {
namespace api {

	class EngineController;

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
		WaynetManager();

		void reset();

		std::map<std::string, std::vector<Edge>> _waypoints;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_WAYNETMANAGER_H__ */
