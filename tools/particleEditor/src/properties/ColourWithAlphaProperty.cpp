#include "properties/ColourWithAlphaProperty.h"

#include "widgets/PropertyWindow.h"

#include "ParticleUniverseDynamicAttribute.h"

#include <QLabel>
#include <QSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	ColourWithAlphaProperty::ColourWithAlphaProperty(QWidget * par, QString label, QString name, Vec4 value) : Property(par, label, name), _layout(nullptr), _value(value) {
		QWidget * widget = new QWidget(this);
		_layout = new QGridLayout(widget);
		widget->setLayout(_layout);
		horizontalLayout->addWidget(widget);
		_layout->addWidget(new QLabel("Colour", widget), 0, 0);
		QLabel * l = new QLabel("R", this);
		_layout->addWidget(l, 0, 1);
		QSpinBox * dsb = new QSpinBox(this);
		dsb->setMaximum(255);
		dsb->setValue(value.getX());
		_layout->addWidget(dsb, 0, 2);

		l = new QLabel("G", this);
		_layout->addWidget(l, 0, 3);
		dsb = new QSpinBox(this);
		dsb->setMaximum(255);
		dsb->setValue(value.getY());
		_layout->addWidget(dsb, 0, 4);

		l = new QLabel("B", this);
		_layout->addWidget(l, 0, 5);
		dsb = new QSpinBox(this);
		dsb->setMaximum(255);
		dsb->setValue(value.getZ());
		_layout->addWidget(dsb, 0, 6);

		l = new QLabel("Alpha", this);
		_layout->addWidget(l, 1, 1);
		dsb = new QSpinBox(this);
		dsb->setMaximum(255);
		dsb->setValue(value.getW());
		_layout->addWidget(dsb, 1, 2);
	}

	ColourWithAlphaProperty::~ColourWithAlphaProperty() {
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
