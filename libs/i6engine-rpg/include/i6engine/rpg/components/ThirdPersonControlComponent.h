#ifndef __I6ENGINE_RPG_COMPONENTS_THIRDPERSONCONTROLCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_THIRDPERSONCONTROLCOMPONENT_H__

#include "i6engine/api/components/Component.h"
#include "i6engine/api/facades/MessageSubscriberFacade.h"

namespace i6engine {
namespace api {
	class PhysicalStateComponent;
} /* namespace api */
namespace rpg {
namespace components {

	class ISIXE_RPG_API ThirdPersonControlComponent : public api::Component, public api::MessageSubscriberFacade {
	public:
		ThirdPersonControlComponent(const int64_t id, const api::attributeMap & params);
		virtual ~ThirdPersonControlComponent();

		static api::ComPtr createC(const int64_t id, const api::attributeMap & params);

		void Init() override;

		void Tick() override;

		void Finalize() override;

		void News(const api::GameMessage::Ptr & msg);

		api::attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "ThirdPersonControl";
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

	private:
		utils::weakPtr<api::PhysicalStateComponent, api::Component> _psc;
		int64_t _highlightTargetID;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENTS_THIRDPERSONCONTROLCOMPONENT_H__ */
