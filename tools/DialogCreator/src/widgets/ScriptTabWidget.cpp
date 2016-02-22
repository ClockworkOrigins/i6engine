#include "widgets/ScriptTabWidget.h"

#include <QStringListModel>

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	ScriptTabWidget::ScriptTabWidget(QWidget * par, plugins::ScriptLanguagePluginInterface * plugin) : QWidget(par), _plugin(plugin), _scriptFunctions(_plugin->getScriptFunctions()) {
		setupUi(this);

		QStringList list;

		for (dialogCreator::plugins::ScriptFunction scriptFunc : _scriptFunctions) {
			QString entry = scriptFunc.name + "(";
			for (int i = 0; i < scriptFunc.signatureTypes.size(); i++) {
				entry += scriptFunc.signatureTypes[i];
				if (i < scriptFunc.signatureTypes.size() - 1) {
					entry += ", ";
				}
			}
			entry += ")";
			if (scriptFunc.returnType != "void") {
				entry += ": " + scriptFunc.returnType;
			}
			list.append(entry);
		}

		QStringListModel * model = new QStringListModel(listView);
		model->setStringList(list);
		listView->setModel(model);
	}

	ScriptTabWidget::~ScriptTabWidget() {
	}

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6engine */
