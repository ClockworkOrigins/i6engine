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

	void InterParticleColliderPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::InterParticleCollider * interParticleCollider = static_cast<ParticleUniverse::InterParticleCollider *>(affector);

		// Adjustment: ParticleUniverse::Real
		setDouble(PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT, interParticleCollider->getAdjustment());

		// Collision response: List
		ParticleUniverse::InterParticleCollider::InterParticleCollisionResponse collisionResponse = interParticleCollider->getInterParticleCollisionResponse();
		QString collisionResponseString = CR_AVERAGE_VELOCITY;
		if (collisionResponse == ParticleUniverse::InterParticleCollider::IPCR_ANGLE_BASED_VELOCITY) {
			collisionResponseString = CR_ANGLE_BASED_VELOCITY;
		}
		setEnumString(PRNL_COLLISON_RESPONSE, collisionResponseString);
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
