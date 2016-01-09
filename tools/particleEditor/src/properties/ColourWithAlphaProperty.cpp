#include "properties/ColourWithAlphaProperty.h"

#include "widgets/PropertyWindow.h"

#include "ParticleUniverseDynamicAttribute.h"

#include <QLabel>
#include <QSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	ColourWithAlphaProperty::ColourWithAlphaProperty(QWidget * par, QString label, QString name, Vec4 value) : Property(par, label, name), _layout(nullptr), _value(value), _spinBoxR(nullptr), _spinBoxG(nullptr), _spinBoxB(nullptr), _spinBoxA(nullptr) {
		QWidget * widget = new QWidget(this);
		_layout = new QGridLayout(widget);
		widget->setLayout(_layout);
		horizontalLayout->addWidget(widget);
		_layout->addWidget(new QLabel("Colour", widget), 0, 0);
		QLabel * l = new QLabel("R", this);
		_layout->addWidget(l, 0, 1);
		_spinBoxR = new QSpinBox(this);
		_spinBoxR->setMaximum(255);
		_spinBoxR->setValue(value.getX());
		_layout->addWidget(_spinBoxR, 0, 2);

		l = new QLabel("G", this);
		_layout->addWidget(l, 0, 3);
		_spinBoxG = new QSpinBox(this);
		_spinBoxG->setMaximum(255);
		_spinBoxG->setValue(value.getY());
		_layout->addWidget(_spinBoxG, 0, 4);

		l = new QLabel("B", this);
		_layout->addWidget(l, 0, 5);
		_spinBoxB = new QSpinBox(this);
		_spinBoxB->setMaximum(255);
		_spinBoxB->setValue(value.getZ());
		_layout->addWidget(_spinBoxB, 0, 6);

		l = new QLabel("Alpha", this);
		_layout->addWidget(l, 1, 1);
		_spinBoxA = new QSpinBox(this);
		_spinBoxA->setMaximum(255);
		_spinBoxA->setValue(value.getW());
		_layout->addWidget(_spinBoxA, 1, 2);
	}

	ColourWithAlphaProperty::~ColourWithAlphaProperty() {
	}

	void ColourWithAlphaProperty::setColourWithAlpha(Vec4 value) {
		_value = value;
		_spinBoxR->setValue(value.getX());
		_spinBoxG->setValue(value.getY());
		_spinBoxB->setValue(value.getZ());
		_spinBoxA->setValue(value.getW());
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
