#include "widgets/EventHandlerPropertyWindows/DoScaleEventHandlerPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEventHandlers/ParticleUniverseDoScaleEventHandler.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	DoScaleEventHandlerPropertyWindow::DoScaleEventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EventHandlerPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_SCALE_FRACTION = QApplication::tr("Scale fraction");
		PRNL_SCALE_TYPE = QApplication::tr("Scale type");
		SC_TIME_TO_LIVE = QApplication::tr("Time to live");
		SC_VELOCITY = QApplication::tr("Velocity");

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

	void DoScaleEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) {
		EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);

		// Copy properties from eventHandler to property window
		ParticleUniverse::DoScaleEventHandler * doScaleEventHandler = static_cast<ParticleUniverse::DoScaleEventHandler *>(eventHandler);

		// Scale Fraction: Ogre:: Real
		setDouble(PRNL_SCALE_FRACTION, doScaleEventHandler->getScaleFraction());

		// Scale Type: List
		ParticleUniverse::DoScaleEventHandler::ScaleType scaleType = doScaleEventHandler->getScaleType();
		QString scaleTypeString = SC_TIME_TO_LIVE;
		if (scaleType == ParticleUniverse::DoScaleEventHandler::ST_VELOCITY) {
			scaleTypeString = SC_VELOCITY;
		}
		setEnumString(PRNL_SCALE_TYPE, scaleTypeString);
	}

	void DoScaleEventHandlerPropertyWindow::copyAttributeToEventHandler(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::DoScaleEventHandler * handler = static_cast<ParticleUniverse::DoScaleEventHandler *>(_owner->getPUElement());
		if (!handler) {
			return;
		}

		if (propertyName == PRNL_SCALE_FRACTION) {
			// Scale Fraction: Ogre:: Real
			handler->setScaleFraction(prop->getDouble());
		} else if (propertyName == PRNL_SCALE_TYPE) {
			// Scale Type: List
			QString scaleType = prop->getEnumString();
			if (scaleType == SC_TIME_TO_LIVE) {
				handler->setScaleType(ParticleUniverse::DoScaleEventHandler::ST_TIME_TO_LIVE);
			} else if (scaleType == SC_VELOCITY) {
				handler->setScaleType(ParticleUniverse::DoScaleEventHandler::ST_VELOCITY);
			}
		} else {
			// Update handler with another attribute
			EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
