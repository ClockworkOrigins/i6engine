#include "i6engine/api/manager/WaynetManager.h"

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#include <cfloat>
#endif

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/components/WaypointComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
namespace api {

	WaynetManager::WaynetManager() : _waypoints(), _paths() {
	}

	std::vector<Vec3> WaynetManager::getShortestPath(const Vec3 & startPos, const std::string & targetWP) {
		return getShortestPath(getNearestWaypoint(startPos), targetWP);
	}

	void WaynetManager::createWaynet() {
		reset();

		auto goList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Waypoint");
		std::set<std::pair<int64_t, int64_t>> edges;
		std::map<std::string, GOPtr> WPMapping;
		for (auto & go : goList) {
			auto wc = go->getGOC<WaypointComponent>(components::ComponentTypes::WaypointComponent);
			if (wc == nullptr) {
				ISIXE_THROW_FAILURE("WaynetManager", "Found Waypoint without WaypointComponent!");
			}
			auto ownSSC = go->getGOC<StaticStateComponent>(components::ComponentTypes::StaticStateComponent);
			if (ownSSC == nullptr) {
				ISIXE_THROW_FAILURE("WaynetManager", "Found Waypoint without StaticStateComponent!");
			}
			WPMapping.insert(std::make_pair(wc->getName(), go));
			for (std::string target : wc->getConnections()) {
				GOPtr targetGO;
				if (WPMapping.find(target) != WPMapping.end()) {
					targetGO = WPMapping.find(target)->second;
				} else {
					for (auto & to : goList) {
						auto wc2 = to->getGOC<WaypointComponent>(components::ComponentTypes::WaypointComponent);
						if (wc2 == nullptr) {
							ISIXE_THROW_FAILURE("WaynetManager", "Found Waypoint without WaypointComponent!");
						}
						if (wc2->getName() == target) {
							WPMapping.insert(std::make_pair(target, to));
							targetGO = to;
							break;
						}
					}
				}
				if (edges.find(std::make_pair(go->getID(), targetGO->getID())) != edges.end()) {
					continue;
				}
				auto targetSSC = targetGO->getGOC<StaticStateComponent>(components::ComponentTypes::StaticStateComponent);

				// connection from go to target
				Edge a;
				a.targetWP = target;
				a.startPosition = ownSSC->getPosition();
				a.targetPosition = targetSSC->getPosition();
				a.targetRotation = targetSSC->getRotation();
				a.length = (targetSSC->getPosition() - ownSSC->getPosition()).length();

				// connection from target to go
				Edge b;
				b.targetWP = wc->getName();
				b.startPosition = targetSSC->getPosition();
				b.targetPosition = ownSSC->getPosition();
				b.targetRotation = ownSSC->getRotation();
				b.length = (targetSSC->getPosition() - ownSSC->getPosition()).length();

				_waypoints[wc->getName()].push_back(a);
				_waypoints[target].push_back(b);

				edges.insert(std::make_pair(go->getID(), targetGO->getID()));
				edges.insert(std::make_pair(targetGO->getID(), go->getID()));
			}
		}
	}

	void WaynetManager::showWaynet() const {
		std::set<std::pair<std::string, std::string>> edges;
		for (auto & p : _waypoints) {
			for (auto & e : p.second) {
				if (edges.find(std::make_pair(p.first, e.targetWP)) == edges.end()) {
					EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraLine, core::Method::Create, new graphics::Graphics_Line_Create(e.startPosition, e.targetPosition), core::Subsystem::Object));
					edges.insert(std::make_pair(p.first, e.targetWP));
					edges.insert(std::make_pair(e.targetWP, p.first));
				}
			}
		}
	}

	std::string WaynetManager::getNearestWaypoint(const Vec3 & pos) const {
		double minDist = DBL_MAX;
		std::string minName = "";

		for (auto go : EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Waypoint")) {
			auto ssc = go->getGOC<StaticStateComponent>(components::ComponentTypes::StaticStateComponent);
			if ((ssc->getPosition() - pos).length() < minDist) {
				minDist = (ssc->getPosition() - pos).length();
				minName = go->getGOC<WaypointComponent>(components::ComponentTypes::WaypointComponent)->getName();
			}
		}

		return minName;
	}

	void WaynetManager::reset() {
		_waypoints.clear();
	}

	std::vector<Vec3> WaynetManager::getShortestPath(const std::string & startWP, const std::string & targetWP) {
		auto it = _paths.find(std::make_pair(startWP, targetWP)); // check if we returned this path before
		if (it != _paths.end()) {
			return it->second;
		}
		std::map<std::string, double> min_distance;
		min_distance[startWP] = 0.0;
		std::map<std::string, std::string> previous;
		std::set<std::pair<double, std::string>> vertex_queue;
		vertex_queue.insert(std::make_pair(min_distance[startWP], startWP));

		while (!vertex_queue.empty()) {
			double dist = vertex_queue.begin()->first;
			std::string u = vertex_queue.begin()->second;
			vertex_queue.erase(vertex_queue.begin());

			// Visit each edge exiting u
			const auto & neighbors = _waypoints[u];
			for (auto neighbor_iter = neighbors.begin(); neighbor_iter != neighbors.end(); neighbor_iter++) {
				std::string v = neighbor_iter->targetWP;
				double weight = neighbor_iter->length;
				double distance_through_u = dist + weight;
				if (min_distance.find(v) == min_distance.end() || distance_through_u < min_distance[v]) {
					vertex_queue.erase(std::make_pair(min_distance[v], v));

					min_distance[v] = distance_through_u;
					previous[v] = u;
					vertex_queue.insert(std::make_pair(min_distance[v], v));
				}
			}
		}

		std::string target = targetWP;
		std::vector<Vec3> path;
		for (; target != startWP; target = previous[target]) {
			path.insert(path.begin(), _waypoints[target].front().startPosition);
		}
		path.insert(path.begin(), _waypoints[startWP].front().startPosition);
		_paths.insert(std::make_pair(std::make_pair(startWP, targetWP), path));
		return path;
	}

} /* namespace api */
} /* namespace i6engine */
