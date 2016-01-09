#include "widgets/AffectorPropertyWindows/InterParticleColliderPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "ParticleAffectors/ParticleUniverseInterParticleCollider.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	InterParticleColliderPropertyWindow::InterParticleColliderPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT = "Adjustment";
		PRNL_COLLISON_RESPONSE = "Collision response";
		CR_AVERAGE_VELOCITY = "Average velocity";
		CR_ANGLE_BASED_VELOCITY = "Angle based velocity";

		// Adjustment: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT, PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT, ParticleUniverse::InterParticleCollider::DEFAULT_ADJUSTMENT));

		// Collision response: List
		QStringList collisionResponse;
		collisionResponse.append(CR_AVERAGE_VELOCITY);
		collisionResponse.append(CR_ANGLE_BASED_VELOCITY);
		append(new properties::EnumProperty(this, PRNL_COLLISON_RESPONSE, PRNL_COLLISON_RESPONSE, collisionResponse));
	}

	InterParticleColliderPropertyWindow::~InterParticleColliderPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
