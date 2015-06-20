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

		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Move";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}

		void navigate(const Vec3 & to);

		void navigate(const std::string & to);

	private:
		utils::weakPtr<MovementComponent, Component> _mc;
		uint16_t _index;
		std::vector<Vec3> _path;

		void Init() override;

		void Tick() override;

		void Finalize() override;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_COMPONENTS_MOVECOMPONENT_H__ */
