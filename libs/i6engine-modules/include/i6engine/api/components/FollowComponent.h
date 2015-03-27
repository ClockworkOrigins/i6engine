#ifndef __I6ENGINE_API_FOLLOWCOMPONENT_H__
#define __I6ENGINE_API_FOLLOWCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	class PhysicalStateComponent;

	class ISIXE_MODULES_API FollowComponent : public Component {
	public:
		FollowComponent(const int64_t id, const attributeMap & params);

		virtual ~FollowComponent();

		static ComPtr createC(const int64_t id, const attributeMap & params);

		void Init() override;

		void Finalize() override;

		void Tick() override;

		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Follow";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}

	private:
		utils::weakPtr<PhysicalStateComponent, Component> _ownPSC;
		utils::weakPtr<PhysicalStateComponent, Component> _targetPSC;

		uint64_t _lastTime;

		/**
		 * \brief speed in m/s
		 */
		double _speed;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_FOLLOWCOMPONENT_H__ */
