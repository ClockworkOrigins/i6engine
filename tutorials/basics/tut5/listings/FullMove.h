	void Tick() {
		api::MessageSubscriberFacade::processMessages();
		if (isForward) {
			api::GOPtr go = getOwnerGO();
			utils::sharedPtr<api::PhysicalStateComponent, api::Component> psc = go->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
			Quaternion q = psc->getRotation();
			psc->setLinearVelocity((q * Vec3(0, 0, 10) * q.inverse()).toVector(), 2);
		}
		if (isLeft) {
			api::GOPtr go = getOwnerGO();
			utils::sharedPtr<api::PhysicalStateComponent, api::Component> psc = go->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
			Quaternion quat(Vec3(0, 1, 0), 3.1415926 / 64);
			quat = quat / quat.length();
			psc->applyRotation(quat);
		}
		if (isRight) {
			api::GOPtr go = getOwnerGO();
			utils::sharedPtr<api::PhysicalStateComponent, api::Component> psc = go->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
			Quaternion quat(Vec3(0, 1, 0), - 3.1415926 / 64);
			psc->applyRotation(quat);
		}
	}

	void Action(const api::GameMessage::Ptr msg) {
		std::string type = msg->getSubtype();

		if (type != api::keyboard::KeyKeyboard) {
			return;
		}
		api::KeyState pressed = dynamic_cast<api::Input_Keyboard_Update *>(msg->getContent())->pressed;
		api::KeyCode button = static_cast<api::KeyCode>(static_cast<api::Input_Keyboard_Update *>(msg->getContent())->code);

		if (pressed == api::KeyState::KEY_PRESSED && button == api::KeyCode::KC_W) {
			isForward = true;
		} else if (pressed == api::KeyState::KEY_RELEASED && button == api::KeyCode::KC_W) {
			isForward = false;
			api::GOPtr go = getOwnerGO();
			utils::sharedPtr<api::PhysicalStateComponent, api::Component> psc = go->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
			psc->setLinearVelocity(Vec3(0, 0, 0), 2);
		} else if (pressed == api::KeyState::KEY_PRESSED && button == api::KeyCode::KC_A) {
			isLeft = true;
		} else if (pressed == api::KeyState::KEY_RELEASED && button == api::KeyCode::KC_A) {
			isLeft = false;
		} else if (pressed == api::KeyState::KEY_PRESSED && button == api::KeyCode::KC_D) {
			isRight = true;
		} else if (pressed == api::KeyState::KEY_RELEASED && button == api::KeyCode::KC_D) {
			isRight = false;
		}
	}