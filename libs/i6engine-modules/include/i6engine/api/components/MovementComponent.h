/**
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_COMPONENTS_MOVEMENTCOMPONENT_H__
#define __I6ENGINE_API_COMPONENTS_MOVEMENTCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	class PhysicalStateComponent;

	/**
	 * \brief base class for Movement components
	 */
	class ISIXE_MODULES_API MovementComponent : public Component {
	public:
		MovementComponent(const int64_t id, const attributeMap & params);
		virtual ~MovementComponent();

		void Init() override;

		void Tick() override;

		void Finalize() override;

		virtual void forward() = 0;

		virtual void backward() = 0;

		virtual void left() = 0;

		virtual void right() = 0;

	protected:
		utils::weakPtr<PhysicalStateComponent, Component> _psc;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_COMPONENTS_MOVEMENTCOMPONENT_H__ */

/**
 * @}
 */
