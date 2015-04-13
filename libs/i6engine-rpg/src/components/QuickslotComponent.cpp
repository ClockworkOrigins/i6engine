#include "i6engine/rpg/components/QuickslotComponent.h"

#include "boost/lexical_cast.hpp"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/configs/PhysicsConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/InventoryComponent.h"

namespace i6engine {
namespace rpg {
namespace components {

	QuickslotComponent::QuickslotComponent(const int64_t id, const i6engine::api::attributeMap & params) : Component(id, params), api::MessageSubscriberFacade(), _slots(10) {
		_objFamilyID = config::QuickslotComponent;
		_objComponentID = config::QuickslotComponent;

		for (size_t i = 0; i < _slots.size(); i++) {
			_slots[i].first = UINT32_MAX;
			_slots[i].second = "";
		}
	}

	QuickslotComponent::~QuickslotComponent() {
	}

	void QuickslotComponent::Init() {
		if (getOwnerGO()->getGOC(config::ComponentTypes::InventoryComponent) == nullptr) {
			ISIXE_THROW_API("QuickslotComponent", "Quickslot requires an InventoryComponent on character!");
		}
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, QuickslotComponent::News, this);

		addTicker();

		std::string name = "Quickslot_";
		double startX = 0.1;
		double startY = 0.8;
		double sizeX = 0.08;
		double sizeY = 0.12;

		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();

		for (uint32_t i = 0; i < 10; i++) {
			gf->addImage(name + "Image_" + std::to_string(i), "RPG/StaticImage", "RPG", "TbM_Filling", startX + i * sizeX, startY, sizeX, sizeY);
			gf->addImage(name + "Thumbnail_" + std::to_string(i), "RPG/StaticImage", "RPG", "TbM_Filling", startX + i * sizeX, startY, sizeX, sizeY);
			gf->addPrint(name + "Key_" + std::to_string(i), "RPG/Blanko", startX + i * sizeX, startY + sizeY - 0.02, ((i < 9) ? std::to_string(i + 1) : "0"), api::gui::Alignment::Left, -1);
			gf->setSize(name + "Key_" + std::to_string(i), sizeX, sizeY);
			gf->addPrint(name + "Amount_" + std::to_string(i), "RPG/Blanko", startX + i * sizeX, startY + sizeY - 0.02, "0", api::gui::Alignment::Right, -1);
			gf->setSize(name + "Amount_" + std::to_string(i), sizeX, sizeY);
			gf->setVisibility(name + "Amount_" + std::to_string(i), false);
		}

		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode::KC_1, "quickslot1");
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode::KC_2, "quickslot2");
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode::KC_3, "quickslot3");
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode::KC_4, "quickslot4");
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode::KC_5, "quickslot5");
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode::KC_6, "quickslot6");
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode::KC_7, "quickslot7");
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode::KC_8, "quickslot8");
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode::KC_9, "quickslot9");
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode::KC_0, "quickslot0");

		getOwnerGO()->getGOC<InventoryComponent>(config::ComponentTypes::InventoryComponent)->registerUpdateCallback(std::bind(&QuickslotComponent::updateItem, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	void QuickslotComponent::Finalize() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);

		removeTicker();

		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		for (uint32_t i = 0; i < 10; i++) {
			gf->deleteWidget("Quickslot_Image_" + std::to_string(i));
			gf->deleteWidget("Quickslot_Thumbnail_" + std::to_string(i));
			gf->deleteWidget("Quickslot_Key_" + std::to_string(i));
			gf->deleteWidget("Quickslot_Amount_" + std::to_string(i));
		}
	}

	api::ComPtr QuickslotComponent::createC(const int64_t id, const api::attributeMap & params) {
		return utils::make_shared<QuickslotComponent, api::Component>(id, params);
	}

	api::attributeMap QuickslotComponent::synchronize() const {
		api::attributeMap params;
		return params;
	}

	void QuickslotComponent::News(const api::GameMessage::Ptr & msg) {
		if (msg->getMessageType() == api::messages::InputMessageType) {
			if (msg->getSubtype() == api::keyboard::KeyKeyboard) {
				api::KeyState pressed = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent())->pressed;
				api::KeyCode button = static_cast<api::input::Input_Keyboard_Update *>(msg->getContent())->code;

				if (pressed != api::KeyState::KEY_PRESSED) {
					return;
				}

				std::string keyString = api::EngineController::GetSingletonPtr()->getInputFacade()->getKeyMapping(button);

				if (keyString == "quickslot1") {
					selectSlot(0);
				} else if (keyString == "quickslot2") {
					selectSlot(1);
				} else if (keyString == "quickslot3") {
					selectSlot(2);
				} else if (keyString == "quickslot4") {
					selectSlot(3);
				} else if (keyString == "quickslot5") {
					selectSlot(4);
				} else if (keyString == "quickslot6") {
					selectSlot(5);
				} else if (keyString == "quickslot7") {
					selectSlot(6);
				} else if (keyString == "quickslot8") {
					selectSlot(7);
				} else if (keyString == "quickslot9") {
					selectSlot(8);
				} else if (keyString == "quickslot0") {
					selectSlot(9);
				}
			}
		}
	}

	void QuickslotComponent::selectSlot(uint32_t slot) {
		auto ic = getOwnerGO()->getGOC<InventoryComponent>(config::ComponentTypes::InventoryComponent);
		if (ic->isActive()) { // add selected item to this slot
			auto selection = ic->getSelectedItem();
			if (std::get<0>(selection) != UINT32_MAX) {
				api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
				for (uint16_t i = 0; i < _slots.size(); i++) {
					if (_slots[i].first == std::get<0>(selection) && _slots[i].second == std::get<1>(selection)) {
						gf->setImage("Quickslot_Thumbnail_" + std::to_string(i), "RPG", "TbM_Filling");
						gf->setText("Quickslot_Amount_" + std::to_string(i), "");
						gf->setVisibility("Quickslot_Amount_" + std::to_string(i), false);
						_slots[i].first = UINT32_MAX;
						break;
					}
				}
				_slots[slot].first = std::get<0>(selection);
				_slots[slot].second = std::get<1>(selection);
				gf->setImage("Quickslot_Thumbnail_" + std::to_string(slot), std::get<2>(selection), std::get<3>(selection));
				gf->setText("Quickslot_Amount_" + std::to_string(slot), std::to_string(ic->getItemCount(_slots[slot].first, _slots[slot].second)));
				gf->setVisibility("Quickslot_Amount_" + std::to_string(slot), true);
			}
		} else { // use item in this slot
			if (_slots[slot].first != UINT32_MAX) {
				ic->useItem(_slots[slot].first, _slots[slot].second, [this, ic, slot]() {
					updateItem(_slots[slot].first, _slots[slot].second, ic->getItemCount(_slots[slot].first, _slots[slot].second));
				});
			}
		}
	}

	void QuickslotComponent::Tick() {
		processMessages();
	}

	void QuickslotComponent::updateItem(uint32_t item, const std::string & name, uint32_t amount) {
		uint32_t slot = UINT32_MAX;
		for (size_t i = 0; i < _slots.size(); i++) {
			if (_slots[i].first == item && _slots[i].second == name) {
				slot = i;
				break;
			}
		}
		if (slot == UINT32_MAX) {
			return;
		}
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		if (amount) {
			gf->setText("Quickslot_Amount_" + std::to_string(slot), std::to_string(amount));
		} else {
			gf->setImage("Quickslot_Thumbnail_" + std::to_string(slot), "RPG", "TbM_Filling");
			gf->setText("Quickslot_Amount_" + std::to_string(slot), "");
			gf->setVisibility("Quickslot_Amount_" + std::to_string(slot), false);
			_slots[slot].first = UINT32_MAX;
		}
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
