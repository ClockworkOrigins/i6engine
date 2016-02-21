#include "widgets/ExternPropertyWindows/GravityExternPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"
#include "widgets/AffectorPropertyWindows/GravityAffectorPropertyWindow.h"

#include "Externs/ParticleUniverseGravityExtern.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	GravityExternPropertyWindow::GravityExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ExternPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_EXTERN_THRESHOLD = QApplication::tr("Distance threshold");

		// Distance Threshold: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_EXTERN_THRESHOLD, PRNL_EXTERN_THRESHOLD, std::numeric_limits<double>::max()));

		// Gravity: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_GRAVITY, PRNL_GRAVITY, ParticleUniverse::GravityAffector::DEFAULT_GRAVITY));
	}

	GravityExternPropertyWindow::~GravityExternPropertyWindow() {
	}

	void GravityExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern * externObject) {
		ExternPropertyWindow::copyAttributesFromExtern(externObject);

		// Copy properties from Extern to property window
		ParticleUniverse::GravityExtern * gravityExtern = static_cast<ParticleUniverse::GravityExtern *>(externObject);

		// Distance Threshold: ParticleUniverse::Real
		setDouble(PRNL_EXTERN_THRESHOLD, gravityExtern->getDistanceThreshold());

		// Gravity: ParticleUniverse::Real
		setDouble(PRNL_GRAVITY, gravityExtern->getGravity());
	}

	void GravityExternPropertyWindow::copyAttributeToExtern(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::Extern * ext = static_cast<ParticleUniverse::Extern *>(_owner->getPUElement());
		ParticleUniverse::GravityExtern * externObject = static_cast<ParticleUniverse::GravityExtern *>(ext);

		if (propertyName == PRNL_EXTERN_THRESHOLD) {
			// Distance Threshold: ParticleUniverse::Real
			ParticleUniverse::Attachable * attachable = static_cast<ParticleUniverse::Attachable *>(_owner->getPUElement());
			attachable->setDistanceThreshold(prop->getDouble());
		} else if (propertyName == PRNL_GRAVITY) {
			// Gravity: ParticleUniverse::Real
			externObject->setGravity(prop->getDouble());
		} else {
			// Update extern with another attribute
			ExternPropertyWindow::copyAttributeToExtern(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
