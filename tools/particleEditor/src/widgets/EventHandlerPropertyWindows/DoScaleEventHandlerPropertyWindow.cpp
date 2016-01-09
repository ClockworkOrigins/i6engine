#include "widgets/EventHandlerPropertyWindows/DoScaleEventHandlerPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "ParticleEventHandlers/ParticleUniverseDoScaleEventHandler.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	DoScaleEventHandlerPropertyWindow::DoScaleEventHandlerPropertyWindow(QWidget * par, QString name) : EventHandlerPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_SCALE_FRACTION = "Scale fraction";
		PRNL_SCALE_TYPE = "Scale type";
		SC_TIME_TO_LIVE = "Time to live";
		SC_VELOCITY = "Velocity";

		// Scale Fraction: Ogre:: Real
		append(new properties::DoubleProperty(this, PRNL_SCALE_FRACTION, PRNL_SCALE_FRACTION, ParticleUniverse::DoScaleEventHandler::DEFAULT_SCALE_FRACTION));

		// Scale Type: List
		QStringList scaleTypes;
		scaleTypes.append(SC_TIME_TO_LIVE);
		scaleTypes.append(SC_VELOCITY);
		append(new properties::EnumProperty(this, PRNL_SCALE_TYPE, PRNL_SCALE_TYPE, scaleTypes));
	}

	DoScaleEventHandlerPropertyWindow::~DoScaleEventHandlerPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
