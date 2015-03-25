#include "i6engine/rpg/components/ThirdPersonControlComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	ThirdPersonControlComponent::ThirdPersonControlComponent(const int64_t id, const api::attributeMap & params) : Component(id, params), api::MessageSubscriberFacade(), _psc() {
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
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, ThirdPersonControlComponent::News);
		_psc = getOwnerGO()->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
	}

	void ThirdPersonControlComponent::Tick() {
		processMessages();
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
					auto psc = _psc.get();
					if (iku->code == i6engine::api::KeyCode::KC_W) {
						psc->applyCentralForce(Vec3(0.0, 0.0, -30.0), true);
					} else if (iku->code == i6engine::api::KeyCode::KC_S) {
						psc->applyCentralForce(Vec3(0.0, 0.0, 25.0), true);
					} else if (iku->code == i6engine::api::KeyCode::KC_A) {
						Quaternion rot(Vec3(0.0, 1.0, 0.0), 1.5 * PI / 180);
						psc->applyRotation(rot);
					} else if (iku->code == i6engine::api::KeyCode::KC_D) {
						Quaternion rot(Vec3(0.0, 1.0, 0.0), -1.5 * PI / 180);
						psc->applyRotation(rot);
					}
				}
			} else if (subType == api::mouse::MouseMessageTypes::MouMouse) {
				double xPos = dynamic_cast<api::input::Input_Mouse_Update *>(msg->getContent())->intNewX / double(api::EngineController::GetSingleton().getGraphicsFacade()->getCurrentResolution().width);
			}
		}
	}

	i6engine::api::attributeMap ThirdPersonControlComponent::synchronize() {
		return i6engine::api::attributeMap();
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
