#include "i6engine/rpg/components/ThirdPersonControlComponent.h"

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#include <float.h>
#endif

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/MovementComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/InventoryComponent.h"
#include "i6engine/rpg/components/ItemComponent.h"
#include "i6engine/rpg/components/NameComponent.h"

namespace i6engine {
namespace rpg {
namespace components {

	ThirdPersonControlComponent::ThirdPersonControlComponent(const int64_t id, const api::attributeMap & params) : Component(id, params), api::MessageSubscriberFacade(), _psc(), _highlightTargetID(-1) {
		_objFamilyID = config::ThirdPersonControlComponent;
		_objComponentID = config::ThirdPersonControlComponent;
	}

	ThirdPersonControlComponent::~ThirdPersonControlComponent() {
	}

	api::ComPtr ThirdPersonControlComponent::createC(const int64_t id, const api::attributeMap & params) {
		return utils::make_shared<ThirdPersonControlComponent, api::Component>(id, params);
	}

	void ThirdPersonControlComponent::Init() {
		addTicker();
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, ThirdPersonControlComponent::News, this);
		_psc = getOwnerGO()->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
	}

	void ThirdPersonControlComponent::Tick() {
		processMessages();

		api::GOPtr targetGO;
		int64_t highlightTargetID = -1;
		double distance = DBL_MAX;
		for (auto & go : api::EngineController::GetSingleton().getObjectFacade()->getGOList()) {
			if (go->getType() != "Player" && go->getGOC(config::NameComponent) != nullptr) {
				Vec3 point = (go->getGOC(api::components::PhysicalStateComponent) != nullptr) ? go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent)->getPosition() : go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent)->getPosition();
				Vec3 offset = _psc.get()->getPosition();
				Vec3 direction = math::rotateVector(Vec3(0.0, 0.0, 8.0), _psc.get()->getRotation());
				double currentDistance = (point - offset).length();
				double currentDistanceFromLine = math::disPointLine(offset, direction, point);
				if (currentDistance <= 8.0 && currentDistanceFromLine <= 2.0 && (point - (offset + direction)).length() > (point - (offset - direction)).length()) {
					if (currentDistance + currentDistanceFromLine < distance) {
						distance = currentDistance + currentDistanceFromLine;
						highlightTargetID = go->getID();
						targetGO = go;
					}
				}
			}
		}
		if (highlightTargetID != _highlightTargetID) {
			if (_highlightTargetID == -1) {
				// add MovableText to new target
				api::attributeMap params;
				params.insert(std::make_pair("font", "DejaVuSans"));
				params.insert(std::make_pair("size", "16"));
				params.insert(std::make_pair("colour", "1.0 1.0 1.0"));
				params.insert(std::make_pair("text", api::EngineController::GetSingleton().getTextManager()->getText(targetGO->getGOC<NameComponent>(config::ComponentTypes::NameComponent)->getName())));
				api::EngineController::GetSingletonPtr()->getObjectFacade()->createComponent(highlightTargetID, api::EngineController::GetSingletonPtr()->getIDManager()->getID(), "MovableText", params);
			} else {
				// remove MovableText from old target
				auto go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_highlightTargetID);
				if (go != nullptr) {
					go->getGOC(api::components::MovableTextComponent)->setDie();
				}
				if (highlightTargetID != -1) {
					// add MovableText to new target
					api::attributeMap params;
					params.insert(std::make_pair("font", "DejaVuSans"));
					params.insert(std::make_pair("size", "16"));
					params.insert(std::make_pair("colour", "1.0 1.0 1.0"));
					params.insert(std::make_pair("text", api::EngineController::GetSingleton().getTextManager()->getText(targetGO->getGOC<NameComponent>(config::ComponentTypes::NameComponent)->getName())));
					api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::ComponentMessageType, api::components::ComCreate, core::Method::Create, new api::components::Component_Create_Create(highlightTargetID, api::EngineController::GetSingletonPtr()->getIDManager()->getID(), core::IPKey(), "MovableText", params), core::Subsystem::Unknown));
				}
			}
			_highlightTargetID = highlightTargetID;
		}
	}

	void ThirdPersonControlComponent::Finalize() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
		removeTicker();
	}

	void ThirdPersonControlComponent::News(const api::GameMessage::Ptr & msg) {
		uint16_t msgType = msg->getMessageType();
		uint16_t subType = msg->getSubtype();

		if (msgType == api::messages::InputMessageType) {
			if (subType == api::keyboard::KeyboardMessageTypes::KeyKeyboard) {
				api::input::Input_Keyboard_Update * iku = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent());
				if (iku->pressed == api::KeyState::KEY_PRESSED || iku->pressed == api::KeyState::KEY_HOLD) {
					auto ic = getOwnerGO()->getGOC<InventoryComponent>(config::ComponentTypes::InventoryComponent);
					auto mc = getOwnerGO()->getGOC<api::MovementComponent>(api::components::ComponentTypes::MovementComponent);
					std::string keyMapping = api::EngineController::GetSingleton().getInputFacade()->getKeyMapping(iku->code);
					if (keyMapping == "forward" && !ic->isActive()) {
						mc->forward();
					} else if (keyMapping == "backward" && !ic->isActive()) {
						mc->backward();
					} else if (keyMapping == "left" && !ic->isActive()) {
						mc->left();
					} else if (keyMapping == "right" && !ic->isActive()) {
						mc->right();
					} else if (keyMapping == "inventory" && iku->pressed == api::KeyState::KEY_PRESSED) {
						if (ic->isActive()) {
							ic->hide();
						} else {
							ic->show();
						}
					} else if (keyMapping == "action" && iku->pressed == api::KeyState::KEY_PRESSED && !ic->isActive()) {
						if (_highlightTargetID != -1) {
							auto targetGO = api::EngineController::GetSingleton().getObjectFacade()->getObject(_highlightTargetID);
							if (targetGO != nullptr) {
								if (targetGO->getGOC<ItemComponent>(config::ComponentTypes::ItemComponent) != nullptr) {
									if (ic->addItem(targetGO)) {
										targetGO->setDie();
									}
								}
							}
						}
					}
				}
			}
		}
	}

	api::attributeMap ThirdPersonControlComponent::synchronize() const {
		return api::attributeMap();
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
