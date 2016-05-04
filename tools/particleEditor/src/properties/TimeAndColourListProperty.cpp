#include "properties/TimeAndColourListProperty.h"

#include "properties/TimeAndColourProperty.h"

#include <QPushButton>

namespace i6e {
namespace particleEditor {
namespace properties {

	TimeAndColourListProperty::TimeAndColourListProperty(QWidget * par, QString label, QString name) : Property(par, label, name), _layout(nullptr), _timeAndColours() {
		QWidget * widget = new QWidget(this);
		_layout = new QVBoxLayout(widget);
		widget->setLayout(_layout);
		horizontalLayout->addWidget(widget);
		QPushButton * pb = new QPushButton(QApplication::tr("Add time and colour"), widget);
		_layout->addWidget(pb);

		connect(pb, SIGNAL(clicked()), this, SLOT(addTimeAndColour()));
	}

	TimeAndColourListProperty::~TimeAndColourListProperty() {
	}

	void TimeAndColourListProperty::setTimeAndColourList(std::vector<std::pair<double, Vec4>> value) {
		for (TimeAndColourProperty * v3p : _timeAndColours) {
			_layout->removeWidget(v3p);
			delete v3p;
		}
		_timeAndColours.clear();
		for (std::pair<double, Vec4> & p : value) {
			TimeAndColourProperty * tacp = new TimeAndColourProperty(this, "", "", p);
			_layout->addWidget(tacp);
			_timeAndColours.push_back(tacp);
			connect(tacp, SIGNAL(changed(QString)), this, SLOT(changedValue()));
		}
	}

	std::vector<std::pair<double, Vec4>> TimeAndColourListProperty::getTimeAndColourList() const {
		std::vector<std::pair<double, Vec4>> value;
		for (TimeAndColourProperty * tacp : _timeAndColours) {
			value.push_back(tacp->getTimeAndColour());
		}
		return value;
	}

	void TimeAndColourListProperty::addTimeAndColour() {
		TimeAndColourProperty * v3p = new TimeAndColourProperty(this, "", "", std::make_pair(0.0, Vec4()));
		_layout->addWidget(v3p);
		_timeAndColours.push_back(v3p);
		connect(v3p, SIGNAL(changed(QString)), this, SLOT(changedValue()));
	}

	void TimeAndColourListProperty::changedValue() {
		triggerChangedSignal();
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
