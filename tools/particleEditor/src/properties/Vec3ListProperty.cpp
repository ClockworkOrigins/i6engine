#include "properties/Vec3ListProperty.h"

#include "properties/Vec3Property.h"

#include "widgets/AffectorPropertyWindows/PathFollowerPropertyWindow.h"

#include "ParticleUniverseDynamicAttribute.h"

#include <QPushButton>

namespace i6engine {
namespace particleEditor {
namespace properties {

	Vec3ListProperty::Vec3ListProperty(QWidget * par, QString label, QString name) : Property(par, label, name), _layout(nullptr), _positions() {
		QWidget * widget = new QWidget(this);
		_layout = new QVBoxLayout(widget);
		widget->setLayout(_layout);
		horizontalLayout->addWidget(widget);
		QPushButton * pb = new QPushButton(widgets::PRNL_PATHFOLLOWER_POSITION, widget);
		_layout->addWidget(pb);

		connect(pb, SIGNAL(clicked()), this, SLOT(addPosition()));
	}

	Vec3ListProperty::~Vec3ListProperty() {
	}

	void Vec3ListProperty::addPosition() {
		Vec3Property * v3p = new Vec3Property(this, widgets::PRNL_POSITION, widgets::PRNL_POSITION, ParticleUniverse::Vector3::ZERO);
		_layout->addWidget(v3p);
		_positions.push_back(v3p);
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
