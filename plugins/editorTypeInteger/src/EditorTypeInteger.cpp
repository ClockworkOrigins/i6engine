#include "EditorTypeInteger.h"

#include <QHBoxLayout>
#include <QSpinBox>

namespace i6e {
namespace plugins {

	EditorTypeIntegerWidget::EditorTypeIntegerWidget(QWidget * parent) : TypeWidgetInterface(parent), _spinBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_spinBox = new QSpinBox(this);
		layout->addWidget(_spinBox);
		setLayout(layout);
	}

	void EditorTypeIntegerWidget::setValues(const std::string & values) {
		_spinBox->setValue(std::stoi(values));
	}

	std::string EditorTypeIntegerWidget::getValues() const {
		return std::to_string(_spinBox->value());
	}

	void EditorTypeIntegerWidget::setReadOnly(bool readOnly) {
		_spinBox->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeInteger::createWidget(QWidget * parent) {
		return new EditorTypeIntegerWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
