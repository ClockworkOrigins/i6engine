/**
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_COMPONENT_QUICKSLOTCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENT_QUICKSLOTCOMPONENT_H__

#include "i6engine/api/components/Component.h"
#include "i6engine/api/facades/MessageSubscriberFacade.h"

namespace i6engine {
namespace rpg {
namespace components {

	/**
	 * \brief Adds a Quickslot bar to the screen enabling quick use of items
	 */
	class ISIXE_RPG_API QuickslotComponent : public api::Component, api::MessageSubscriberFacade {
	public:
		QuickslotComponent(const int64_t id, const api::attributeMap & params);

		~QuickslotComponent();

		static api::ComPtr createC(const int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Quickslot";
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		void show();

		void hide();

	private:
		std::vector<std::pair<uint32_t, std::string>> _slots;

		void Init() override;

		void Tick() override;

		void Finalize() override;

		void News(const api::GameMessage::Ptr & msg) override;

		void selectSlot(uint32_t slot);

		void updateItem(uint32_t item, const std::string & name, uint32_t amount);
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENT_QUICKSLOTCOMPONENT_H__ */

/**
 * @}
 */
