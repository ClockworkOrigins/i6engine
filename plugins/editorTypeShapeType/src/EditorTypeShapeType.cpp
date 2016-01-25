#include "EditorTypeShapeType.h"

#include <QHBoxLayout>
#include <QComboBox>

namespace i6engine {
namespace plugins {

	std::map<QString, api::PhysicalStateComponent::ShapeType> EditorTypeShapeTypeWidget::shapeTypes = { { "PLANE", api::PhysicalStateComponent::ShapeType::PLANE }, { "BOX", api::PhysicalStateComponent::ShapeType::BOX }, { "SPHERE", api::PhysicalStateComponent::ShapeType::SPHERE }, { "FILE", api::PhysicalStateComponent::ShapeType::FILE } };

	EditorTypeShapeTypeWidget::EditorTypeShapeTypeWidget(QWidget * parent) : TypeWidgetInterface(parent), _comboBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_comboBox = new QComboBox(this);
		for (auto & p : shapeTypes) {
			_comboBox->addItem(p.first);
		}
		layout->addWidget(_comboBox);
		setLayout(layout);
	}

	void EditorTypeShapeTypeWidget::setValues(const std::string & values) {
		api::PhysicalStateComponent::ShapeType st = api::PhysicalStateComponent::ShapeType(std::stoi(values));
		for (auto & p : shapeTypes) {
			if (p.second == st) {
				_comboBox->setCurrentText(p.first);
				break;
			}
		}
	}

	std::string EditorTypeShapeTypeWidget::getValues() const {
		return std::to_string(int(shapeTypes[_comboBox->currentText()]));
	}

	void EditorTypeShapeTypeWidget::setReadOnly(bool readOnly) {
		_comboBox->setEnabled(!readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeShapeType::createWidget(QWidget * parent) {
		return new EditorTypeShapeTypeWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6engine */

Q_PLUGIN_METADATA(IID "i6engine.editor.type.TypePluginInterface")
