#ifndef __I6ENGINE_PLUGINS_EDITORTYPEBOOL_H__
#define __I6ENGINE_PLUGINS_EDITORTYPEBOOL_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

class QCheckBox;

namespace i6engine {
namespace plugins {

	class EditorTypeBoolWidget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeBoolWidget(QWidget * parent);

		void setValues(const std::string & values);

		std::string getValues() const;

		void setReadOnly(bool readOnly);

	private:
		QCheckBox * _checkBox;
	};

	class EditorTypeBool : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6engine.editor.type.TypePluginInterface")
		Q_INTERFACES(i6engine::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "Bool";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPEBOOL_H__ */
