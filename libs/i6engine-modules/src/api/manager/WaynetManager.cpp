#include "i6engine/api/manager/WaynetManager.h"

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

	WaynetManager::WaynetManager() : _waypoints() {
	}

	std::vector<Vec3> WaynetManager::getShortestPath(const Vec3 & startPos, const std::string & targetWP) {
		return {};
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
				a.startPosition = targetSSC->getPosition();
				a.targetPosition = ownSSC->getPosition();
				a.targetRotation = ownSSC->getRotation();
				a.length = (targetSSC->getPosition() - ownSSC->getPosition()).length();

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
					EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraLine, i6engine::core::Method::Create, new graphics::Graphics_Line_Create(e.startPosition, e.targetPosition), core::Subsystem::Object));
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

} /* namespace api */
} /* namespace i6engine */
