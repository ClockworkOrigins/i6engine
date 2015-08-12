#ifndef PMC
#define PMC

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/KeyCodes.h"
#include "i6engine/api/components/Component.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/MessageSubscriberFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"

class MovementComponent : public i6engine::api::Component, public i6engine::api::MessageSubscriberFacade {
public:
	static i6engine::utils::sharedPtr<i6engine::api::Component, i6engine::api::Component> createC(const int id, const i6engine::api::attributeMap & params) {
		return i6engine::utils::make_shared<MovementComponent, i6engine::api::Component>(id, params);
	}
	
	~MovementComponent() {
		removeTicker();
		ISIXE_UNREGISTERMESSAGETYPE(i6engine::api::messages::InputMessageType);
	}

	void Init() {
		ISIXE_REGISTERMESSAGETYPE(i6engine::api::messages::InputMessageType, MovementComponent::Action, this);
		addTicker();
	}
	
	void Tick() {
		i6engine::api::MessageSubscriberFacade::processMessages();
	}

	void Action(const i6engine::api::GameMessage::Ptr msg) {
	}

	i6engine::api::attributeMap synchronize() const {
		return i6engine::api::attributeMap();
	}

	std::vector<i6engine::api::componentOptions> getComponentOptions() {
		return {};
	}

private:
	MovementComponent(const int id, const i6engine::api::attributeMap & params) : Component(id) {
		Component::_objFamilyID = 100;
		Component::_objComponentID = 100;
	}
};

#endif