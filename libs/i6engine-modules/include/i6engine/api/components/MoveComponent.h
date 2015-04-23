#ifndef __I6ENGINE_API_COMPONENTS_MOVECOMPONENT_H__
#define __I6ENGINE_API_COMPONENTS_MOVECOMPONENT_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	class MovementComponent;

	class ISIXE_MODULES_API MoveComponent : public Component {
	public:
		MoveComponent(const int64_t id, const attributeMap & params);
		~MoveComponent();

		static ComPtr createC(const int64_t id, const attributeMap & params);

		void Init() override;

		void Tick() override;

		void Finalize() override;

		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Move";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}

	private:
		utils::weakPtr<MovementComponent, Component> _mc;
		uint16_t _index;
		std::vector<Vec3> _path;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_COMPONENTS_MOVECOMPONENT_H__ */
