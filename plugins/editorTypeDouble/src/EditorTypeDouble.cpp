#include "EditorTypeDouble.h"

#include <QHBoxLayout>
#include <QDoubleSpinBox>

namespace i6engine {
namespace plugins {

	EditorTypeDoubleWidget::EditorTypeDoubleWidget(QWidget * parent) : TypeWidgetInterface(parent), _spinBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_spinBox = new QDoubleSpinBox(this);
		layout->addWidget(_spinBox);
		setLayout(layout);
	}

	void EditorTypeDoubleWidget::setValues(const std::string & values) {
		_spinBox->setValue(std::stod(values));
	}

	std::string EditorTypeDoubleWidget::getValues() const {
		return std::to_string(_spinBox->value());
	}

	void EditorTypeDoubleWidget::setReadOnly(bool readOnly) {
		_spinBox->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeDouble::createWidget(QWidget * parent) {
		return new EditorTypeDoubleWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6engine */

Q_PLUGIN_METADATA(IID "i6engine.editor.type.TypePluginInterface")
