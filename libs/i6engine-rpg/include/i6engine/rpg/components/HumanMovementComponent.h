#ifndef __I6ENGINE_RPG_COMPONENTS_HUMANMOVEMENTCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_HUMANMOVEMENTCOMPONENT_H__

#include "i6engine/api/components/MovementComponent.h"

namespace i6engine {
namespace rpg {
namespace components {

	class ISIXE_RPG_API HumanMovementComponent : public api::MovementComponent {
	public:
		HumanMovementComponent(const int64_t id, const api::attributeMap & params);
		virtual ~HumanMovementComponent();

		static api::ComPtr createC(const int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "HumanMovement";
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		void forward();

		void backward();

		void left();

		void right();
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENTS_HUMANMOVEMENTCOMPONENT_H__ */
