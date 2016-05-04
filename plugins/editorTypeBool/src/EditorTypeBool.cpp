#include "EditorTypeBool.h"

#include <QHBoxLayout>
#include <QCheckBox>

namespace i6e {
namespace plugins {

	EditorTypeBoolWidget::EditorTypeBoolWidget(QWidget * parent) : TypeWidgetInterface(parent), _checkBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_checkBox = new QCheckBox(this);
		layout->addWidget(_checkBox);
		setLayout(layout);
	}

	void EditorTypeBoolWidget::setValues(const std::string & values) {
		_checkBox->setChecked(std::stoi(values));
	}

	std::string EditorTypeBoolWidget::getValues() const {
		return std::to_string(_checkBox->isChecked());
	}

	void EditorTypeBoolWidget::setReadOnly(bool readOnly) {
		_checkBox->setEnabled(!readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeBool::createWidget(QWidget * parent) {
		return new EditorTypeBoolWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
