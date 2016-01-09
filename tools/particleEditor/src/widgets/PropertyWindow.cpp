#include "widgets/PropertyWindow.h"

#include "properties/StringProperty.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	PropertyWindow::PropertyWindow(QWidget * par, QString name) : QWidget(par), _name(name), _types() {
		setupUi(this);

		PRNL_NAME = "Name";
		PRNL_POST_RGB = " RGB";
		PRNL_POST_ALPHA = " Alpha";
		PRNL_W = ".w";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_TYPE = "Attribute type";
		PRNL_VALUE = "Value";
		PRNL_MIN_VALUE = "Minimum";
		PRNL_MAX_VALUE = "Maximum";
		PRNL_OSC_TYPE = "Oscillation type";
		PRNL_OSC_SINE = "Sine";
		PRNL_OSC_SQUARE = "Square";
		PRNL_OSC_FREQUENCY = "Frequency";
		PRNL_OSC_PHASE = "Phase";
		PRNL_OSC_BASE = "Base";
		PRNL_OSC_AMPLITUDE = "Amplitude";
		PRNL_CURVED_INTERPOLATION = "Interpolation";
		PRNL_CONTROL_POINT = "Control Point";
		PRNL_POSITION = "Position";
		PRNL_CONTROL_POINT_X = ".x";
		PRNL_CONTROL_POINT_Y = ".y";
		PRNL_POSITION_X = ".x";
		PRNL_POSITION_Y = ".y";
		PRNL_POSITION_Z = ".z";
		PRNL_FLOAT = "Value";
		DYN_FIXED = "Fixed";
		DYN_RANDOM = "Random";
		DYN_CURVED = "Curved";
		DYN_OSCILLATE = "Oscillate";
		DYN_CURVED_LINEAR = "Linear";
		DYN_CURVED_SPLINE = "Spline";
		PT_ALL = "All types";
		PT_VISUAL = "Visual";
		PT_EMITTER = "Emitter";
		PT_AFFECTOR = "Affector";
		PT_SYSTEM = "System";
		PT_TECHNIQUE = "Technique";

		_types.append(DYN_FIXED);
		_types.append(DYN_RANDOM);
		_types.append(DYN_CURVED);
		_types.append(DYN_OSCILLATE);

		if (!_name.isEmpty()) {
			append(new properties::StringProperty(this, PRNL_NAME, PRNL_NAME, _name));
		}

		hide();
	}

	PropertyWindow::~PropertyWindow() {
	}

	void PropertyWindow::append(properties::Property * prop) {
		verticalLayout->addWidget(prop);
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
