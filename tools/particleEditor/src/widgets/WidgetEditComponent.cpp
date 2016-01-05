#include "widgets/WidgetEditComponent.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QLabel>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	QString WidgetEditComponent::EC_SYSTEM = "System";
	QString WidgetEditComponent::EC_RENDERER = "Renderer";
	QString WidgetEditComponent::EC_TECHNIQUE = "Technique";
	QString WidgetEditComponent::EC_EMITTER = "Emitter";
	QString WidgetEditComponent::EC_AFFECTOR = "Affector";
	QString WidgetEditComponent::EC_OBSERVER = "Observer";
	QString WidgetEditComponent::EC_EXTERN = "Extern";
	QString WidgetEditComponent::EC_BEHAVIOUR = "Behaviour";
	QString WidgetEditComponent::EC_HANDLER = "Handler";
	QString WidgetEditComponent::CST_UNDEFINED = "Undefined";

	WidgetEditComponent::WidgetEditComponent(QGraphicsScene * scene, QString name, QString type, QString subType) : QGraphicsWidget(), _type(type), _element(nullptr) {
		QString labelText = type + " (" + subType + ")";
		QWidget * widget = new QWidget();
		QHBoxLayout * hLayout = new QHBoxLayout(widget);
		widget->setLayout(hLayout);
		QLabel * label = new QLabel(widget);
		hLayout->addWidget(label);
		QGraphicsLinearLayout * layout = new QGraphicsLinearLayout(this);
		setLayout(layout);
		layout->addItem(scene->addWidget(widget));
		if (!name.isEmpty()) {
			labelText += " - " + name;
		}
		label->setText(labelText);

		if (type == EC_SYSTEM) {
			widget->setStyleSheet("background: black");
			label->setStyleSheet("color: white");
		} else if (type == EC_TECHNIQUE) {
			widget->setStyleSheet("background: " + QVariant(QColor(102, 152, 255)).toString() + ";");
		} else if (type == EC_RENDERER) {
			widget->setStyleSheet("background: " + QVariant(QColor(228, 34, 23)).toString() + ";");
		} else if (type == EC_EMITTER) {
			widget->setStyleSheet("background: " + QVariant(QColor(76, 196, 23)).toString() + ";");
		} else if (type == EC_OBSERVER) {
			widget->setStyleSheet("background: " + QVariant(QColor(37, 65, 23)).toString() + ";");
		} else if (type == EC_AFFECTOR) {
			widget->setStyleSheet("background: " + QVariant(QColor(251, 177, 23)).toString() + ";");
		} else if (type == EC_HANDLER) {
			widget->setStyleSheet("background: " + QVariant(QColor(141, 56, 201)).toString() + ";");
		} else if (type == EC_BEHAVIOUR) {
			widget->setStyleSheet("background: " + QVariant(QColor(48, 125, 126)).toString() + ";");
		} else if (type == EC_EXTERN) {
			widget->setStyleSheet("background: " + QVariant(QColor(130, 123, 96)).toString() + ";");
		}

		resize(200, 80);
	}

	WidgetEditComponent::~WidgetEditComponent() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
