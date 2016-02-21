#include "properties/TimeAndColourProperty.h"

#include "i6engine/math/i6eVector.h"

#include "properties/ColourWithAlphaProperty.h"

#include "widgets/PropertyWindow.h"

#include <QLabel>
#include <QDoubleSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	TimeAndColourProperty::TimeAndColourProperty(QWidget * par, QString label, QString name, const std::pair<double, Vec4> & value) : Property(par, label, name), _layout(nullptr), _value(value), _colourProperty(nullptr), _doubleSpinBox(nullptr) {
		QWidget * widget = new QWidget(this);

		_layout = new QGridLayout(widget);
		widget->setLayout(_layout);

		QLabel * l = new QLabel(QApplication::tr("Time point"), widget);
		_layout->addWidget(l, 0, 0);
		_doubleSpinBox = new QDoubleSpinBox(this);
		_doubleSpinBox->setMinimum(0.0);
		_doubleSpinBox->setMaximum(1.0);
		_doubleSpinBox->setSingleStep(0.001);
		_doubleSpinBox->setValue(_value.first);
		_layout->addWidget(_doubleSpinBox, 1, 0);
		connect(_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changedValue()));

		_colourProperty = new ColourWithAlphaProperty(widget, QApplication::tr("Colour"), QApplication::tr("Colour"), _value.second);
		_layout->addWidget(_colourProperty, 2, 0);
		connect(_colourProperty, SIGNAL(changed(QString)), this, SLOT(changedValue()));

		horizontalLayout->addWidget(widget);
	}

	TimeAndColourProperty::~TimeAndColourProperty() {
	}

	void TimeAndColourProperty::setTimeAndColour(const std::pair<double, Vec4> & value) {
		_value = value;
		_colourProperty->setColourWithAlpha(_value.second);
		_doubleSpinBox->setValue(_value.first);
	}

	void TimeAndColourProperty::changedValue() {
		auto p = std::make_pair(_doubleSpinBox->value(), _colourProperty->getColourWithAlpha());
		if (std::abs(_value.first - p.first) > 0.001 || _value.second != p.second) {
			_value = p;
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
