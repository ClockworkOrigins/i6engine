#include "widgets/AffectorPropertyWindows/ColourAffectorPropertyWindow.h"

#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseColourAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ColourAffectorPropertyWindow::ColourAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_TIME_AND_COLOUR = "Time and Colour";
		PRNL_TIME_AND_COLOUR_PARENT = "Time/Colour";
		PRNL_TIME_AND_COLOUR_TIME = "Time instance";
		PRNL_TIME_AND_COLOUR_COLOUR = "Colour instance";
		PRNL_COLOUR_OPERATION = "Colour operation";
		COP_SET = "Set";
		COP_MULTIPLY = "Multiply";

		// Time and Colour: List
		//wxPGProperty * pid = append(new ParentPropertyTimeAndColour(PRNL_TIME_AND_COLOUR, PRNL_TIME_AND_COLOUR));
		//SetPropertyEditor(pid, wxPG_EDITOR(TextCtrlAndButton)); // Add a button

		// Colour Operation: List of types
		QStringList colourOperations;
		colourOperations.append(COP_SET);
		colourOperations.append(COP_MULTIPLY);
		append(new properties::EnumProperty(this, PRNL_COLOUR_OPERATION, PRNL_COLOUR_OPERATION, colourOperations));
	}

	ColourAffectorPropertyWindow::~ColourAffectorPropertyWindow() {
	}

	void ColourAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::ColourAffector * colourAffector = static_cast<ParticleUniverse::ColourAffector *>(affector);

		// Colour Operation: List of types
		ParticleUniverse::ColourAffector::ColourOperation colourOperation = colourAffector->getColourOperation();
		QString colourOperationString = COP_SET;
		if (colourOperation == ParticleUniverse::ColourAffector::CAO_MULTIPLY) {
			colourOperationString = COP_MULTIPLY;
		}
		setEnumString(PRNL_COLOUR_OPERATION, colourOperationString);
	}

	void ColourAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ColourAffector * affector = static_cast<ParticleUniverse::ColourAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_COLOUR_OPERATION) {
			// Colour Operation: List of types
			QString operation = prop->getEnumString();
			if (operation == COP_SET) {
				affector->setColourOperation(ParticleUniverse::ColourAffector::CAO_SET);
			} else if (operation == COP_MULTIPLY) {
				affector->setColourOperation(ParticleUniverse::ColourAffector::CAO_MULTIPLY);
			}
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
