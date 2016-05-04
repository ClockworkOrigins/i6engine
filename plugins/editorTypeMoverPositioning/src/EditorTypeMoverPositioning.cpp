#include "EditorTypeMoverPositioning.h"

#include <QHBoxLayout>
#include <QComboBox>

namespace i6e {
namespace plugins {

	std::map<QString, api::MoverComponent::Positioning> EditorTypeMoverPositioningWidget::positionings = { { "ABSOLUTE", api::MoverComponent::Positioning::POSITIONING_ABSOLUTE }, { "RELATIVE", api::MoverComponent::Positioning::POSITIONING_RELATIVE } };

	EditorTypeMoverPositioningWidget::EditorTypeMoverPositioningWidget(QWidget * parent) : TypeWidgetInterface(parent), _comboBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_comboBox = new QComboBox(this);
		for (auto & p : positionings) {
			_comboBox->addItem(p.first);
		}
		layout->addWidget(_comboBox);
		setLayout(layout);
	}

	void EditorTypeMoverPositioningWidget::setValues(const std::string & values) {
		api::MoverComponent::Positioning pos = api::MoverComponent::Positioning(std::stoi(values));
		for (auto & p : positionings) {
			if (p.second == pos) {
				_comboBox->setCurrentText(p.first);
				break;
			}
		}
	}

	std::string EditorTypeMoverPositioningWidget::getValues() const {
		return std::to_string(int(positionings[_comboBox->currentText()]));
	}

	void EditorTypeMoverPositioningWidget::setReadOnly(bool readOnly) {
		_comboBox->setEnabled(!readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeMoverPositioning::createWidget(QWidget * parent) {
		return new EditorTypeMoverPositioningWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
