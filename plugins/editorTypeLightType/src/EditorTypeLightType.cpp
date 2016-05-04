#include "EditorTypeLightType.h"

#include <QHBoxLayout>
#include <QComboBox>

namespace i6e {
namespace plugins {

	std::map<QString, api::LuminousAppearanceComponent::LightType> EditorTypeLightTypeWidget::lightTypes = { { "POINT", api::LuminousAppearanceComponent::LightType::POINT }, { "DIRECTIONAL", api::LuminousAppearanceComponent::LightType::DIRECTIONAL }, { "SPOT", api::LuminousAppearanceComponent::LightType::SPOT } };

	EditorTypeLightTypeWidget::EditorTypeLightTypeWidget(QWidget * parent) : TypeWidgetInterface(parent), _comboBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_comboBox = new QComboBox(this);
		for (auto & p : lightTypes) {
			_comboBox->addItem(p.first);
		}
		layout->addWidget(_comboBox);
		setLayout(layout);
	}

	void EditorTypeLightTypeWidget::setValues(const std::string & values) {
		api::LuminousAppearanceComponent::LightType lt = api::LuminousAppearanceComponent::LightType(std::stoi(values));
		for (auto & p : lightTypes) {
			if (p.second == lt) {
				_comboBox->setCurrentText(p.first);
				break;
			}
		}
	}

	std::string EditorTypeLightTypeWidget::getValues() const {
		return std::to_string(int(lightTypes[_comboBox->currentText()]));
	}

	void EditorTypeLightTypeWidget::setReadOnly(bool readOnly) {
		_comboBox->setEnabled(!readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeLightType::createWidget(QWidget * parent) {
		return new EditorTypeLightTypeWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
