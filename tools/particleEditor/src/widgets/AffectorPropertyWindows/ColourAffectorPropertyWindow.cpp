#include "widgets/AffectorPropertyWindows/ColourAffectorPropertyWindow.h"

#include "properties/EnumProperty.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ColourAffectorPropertyWindow::ColourAffectorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
