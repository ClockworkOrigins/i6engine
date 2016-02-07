#ifndef __I6ENGINE_DIALOGCREATOR_WIDGETS_SCRIPTTABWIDGET_H__
#define __I6ENGINE_DIALOGCREATOR_WIDGETS_SCRIPTTABWIDGET_H__

#include "ui_widgetScriptTab.h"

#include <set>

#include "Plugins/ScriptLanguagePluginInterface.h"

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	class ScriptTabWidget : public QWidget, public Ui::scriptTabWidget {
		Q_OBJECT

	public:
		ScriptTabWidget(QWidget * par, plugins::ScriptLanguagePluginInterface * plugin);
		~ScriptTabWidget();

	private:
		plugins::ScriptLanguagePluginInterface * _plugin;
		QVector<dialogCreator::plugins::ScriptFunction> _scriptFunctions;
	};

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6engine */

#endif /* __I6ENGINE_DIALOGCREATOR_WIDGETS_SCRIPTTABWIDGET_H__ */
