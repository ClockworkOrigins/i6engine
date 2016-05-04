#include "EditorTypeAngle.h"

#include <QHBoxLayout>
#include <QDoubleSpinBox>

namespace i6e {
namespace plugins {

	EditorTypeAngleWidget::EditorTypeAngleWidget(QWidget * parent) : TypeWidgetInterface(parent), _spinBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_spinBox = new QDoubleSpinBox(this);
		_spinBox->setMinimum(-360.0);
		_spinBox->setMaximum(360.0);
		layout->addWidget(_spinBox);
		setLayout(layout);
	}

	void EditorTypeAngleWidget::setValues(const std::string & values) {
		_spinBox->setValue(std::stod(values));
	}

	std::string EditorTypeAngleWidget::getValues() const {
		return std::to_string(_spinBox->value());
	}

	void EditorTypeAngleWidget::setReadOnly(bool readOnly) {
		_spinBox->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeAngle::createWidget(QWidget * parent) {
		return new EditorTypeAngleWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
