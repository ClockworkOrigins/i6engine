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

class MovementComponent : public i6e::api::Component, public i6e::api::MessageSubscriberFacade {
public:
	static i6e::utils::sharedPtr<i6e::api::Component, i6e::api::Component> createC(const int id, const i6e::api::attributeMap & params) {
		return i6e::utils::make_shared<MovementComponent, i6e::api::Component>(id, params);
	}
	
	~MovementComponent() {
		removeTicker();
		ISIXE_UNREGISTERMESSAGETYPE(i6e::api::messages::InputMessageType);
	}

	void Init() {
		ISIXE_REGISTERMESSAGETYPE(i6e::api::messages::InputMessageType, MovementComponent::Action, this);
		addTicker();
	}
	
	void Tick() {
		i6e::api::MessageSubscriberFacade::processMessages();
	}

	void Action(const i6e::api::GameMessage::Ptr msg) {
	}

	i6e::api::attributeMap synchronize() const {
		return i6e::api::attributeMap();
	}

	std::vector<i6e::api::componentOptions> getComponentOptions() {
		return {};
	}

private:
	MovementComponent(const int id, const i6e::api::attributeMap & params) : Component(id) {
		Component::_objFamilyID = 100;
		Component::_objComponentID = 100;
	}
};

#endif