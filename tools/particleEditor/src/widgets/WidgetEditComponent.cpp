#include "widgets/WidgetEditComponent.h"

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

	WidgetEditComponent::WidgetEditComponent(QWidget * par, QString name, QString type, QString subType) : QWidget(par) {
		setupUi(this);

		QString labelText = type + " (" + subType + ")";

		if (!name.isEmpty()) {
			labelText += " - " + name;
		}
		label->setText(labelText);

		if (type == EC_SYSTEM) {
			setStyleSheet("background: black");
			label->setStyleSheet("color: white");
		} else if (type == EC_TECHNIQUE) {
			setStyleSheet("background: " + QVariant(QColor(102, 152, 255)).toString() + ";");
		} else if (type == EC_RENDERER) {
			setStyleSheet("background: " + QVariant(QColor(228, 34, 23)).toString() + ";");
		} else if (type == EC_EMITTER) {
			setStyleSheet("background: " + QVariant(QColor(76, 196, 23)).toString() + ";");
		} else if (type == EC_OBSERVER) {
			setStyleSheet("background: " + QVariant(QColor(37, 65, 23)).toString() + ";");
		} else if (type == EC_AFFECTOR) {
			setStyleSheet("background: " + QVariant(QColor(251, 177, 23)).toString() + ";");
		} else if (type == EC_HANDLER) {
			setStyleSheet("background: " + QVariant(QColor(141, 56, 201)).toString() + ";");
		} else if (type == EC_BEHAVIOUR) {
			setStyleSheet("background: " + QVariant(QColor(48, 125, 126)).toString() + ";");
		} else if (type == EC_EXTERN) {
			setStyleSheet("background: " + QVariant(QColor(130, 123, 96)).toString() + ";");
		}
	}

	WidgetEditComponent::~WidgetEditComponent() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
