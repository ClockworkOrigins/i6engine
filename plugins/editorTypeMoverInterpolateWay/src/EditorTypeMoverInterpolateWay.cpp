#include "EditorTypeMoverInterpolateWay.h"

#include <QHBoxLayout>
#include <QComboBox>

namespace i6engine {
namespace plugins {

	std::map<QString, api::MoverInterpolateComponent::Way> EditorTypeMoverInterpolateWayWidget::ways = { { "LINEAR", api::MoverInterpolateComponent::Way::LINEAR }, { "BEZIER", api::MoverInterpolateComponent::Way::BEZIER } };

	EditorTypeMoverInterpolateWayWidget::EditorTypeMoverInterpolateWayWidget(QWidget * parent) : TypeWidgetInterface(parent), _comboBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_comboBox = new QComboBox(this);
		for (auto & p : ways) {
			_comboBox->addItem(p.first);
		}
		layout->addWidget(_comboBox);
		setLayout(layout);
	}

	void EditorTypeMoverInterpolateWayWidget::setValues(const std::string & values) {
		api::MoverInterpolateComponent::Way way = api::MoverInterpolateComponent::Way(std::stoi(values));
		for (auto & p : ways) {
			if (p.second == way) {
				_comboBox->setCurrentText(p.first);
				break;
			}
		}
	}

	std::string EditorTypeMoverInterpolateWayWidget::getValues() const {
		return std::to_string(int(ways[_comboBox->currentText()]));
	}

	void EditorTypeMoverInterpolateWayWidget::setReadOnly(bool readOnly) {
		_comboBox->setEnabled(!readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeMoverInterpolateWay::createWidget(QWidget * parent) {
		return new EditorTypeMoverInterpolateWayWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6engine */

Q_PLUGIN_METADATA(IID "i6engine.editor.type.TypePluginInterface")
