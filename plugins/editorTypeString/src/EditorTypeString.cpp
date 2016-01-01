#include "EditorTypeString.h"

#include <QHBoxLayout>
#include <QLineEdit>

namespace i6engine {
namespace plugins {

	EditorTypeStringWidget::EditorTypeStringWidget(QWidget * parent) : TypeWidgetInterface(parent), _lineEdit(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_lineEdit = new QLineEdit(this);
		layout->addWidget(_lineEdit);
		setLayout(layout);
	}

	void EditorTypeStringWidget::setValues(const std::string & values) {
		_lineEdit->setText(QString::fromStdString(values));
	}

	std::string EditorTypeStringWidget::getValues() const {
		return _lineEdit->text().toStdString();
	}

	void EditorTypeStringWidget::setReadOnly(bool readOnly) {
		_lineEdit->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeString::createWidget(QWidget * parent) {
		return new EditorTypeStringWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6engine */

Q_PLUGIN_METADATA(IID "i6engine.editor.type.TypePluginInterface")
