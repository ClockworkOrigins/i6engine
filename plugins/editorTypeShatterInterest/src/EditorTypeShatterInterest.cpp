#include "EditorTypeShatterInterest.h"

#include "i6engine/api/components/PhysicalStateComponent.h"

#include <QHBoxLayout>
#include <QCheckBox>

namespace i6e {
namespace plugins {

	std::map<QString, api::ShatterInterest> EditorTypeShatterInterestWidget::shatterInterests = { { "START", api::ShatterInterest::START }, { "END", api::ShatterInterest::END }, { "ALWAYS", api::ShatterInterest::ALWAYS } };

	EditorTypeShatterInterestWidget::EditorTypeShatterInterestWidget(QWidget * parent) : TypeWidgetInterface(parent), _checkBoxes() {
		QLayout * layout = new QHBoxLayout(this);
		for (auto & p : shatterInterests) {
			QCheckBox * checkBox = new QCheckBox(p.first, this);
			_checkBoxes.insert(std::make_pair(p.first, checkBox));
			layout->addWidget(checkBox);
		}
		setLayout(layout);
	}

	void EditorTypeShatterInterestWidget::setValues(const std::string & values) {
		int si = std::stoi(values);
		for (auto & p : shatterInterests) {
			_checkBoxes[p.first]->setChecked(si & p.second);
		}
	}

	std::string EditorTypeShatterInterestWidget::getValues() const {
		int si = 0;
		for (auto & p : _checkBoxes) {
			if (p.second->isChecked()) {
				si |= shatterInterests[p.first];
			}
		}
		return std::to_string(si);
	}

	void EditorTypeShatterInterestWidget::setReadOnly(bool readOnly) {
		for (auto & p : _checkBoxes) {
			p.second->setCheckable(!readOnly);
		}
	}

	editor::plugins::TypeWidgetInterface * EditorTypeShatterInterest::createWidget(QWidget * parent) {
		return new EditorTypeShatterInterestWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
