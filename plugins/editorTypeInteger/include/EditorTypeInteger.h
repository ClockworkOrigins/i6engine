#ifndef __I6ENGINE_PLUGINS_EDITORTYPEINTEGER_H__
#define __I6ENGINE_PLUGINS_EDITORTYPEINTEGER_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

class QSpinBox;

namespace i6engine {
namespace plugins {

	class EditorTypeIntegerWidget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeIntegerWidget(QWidget * parent);

		void setValues(const std::string & values);

		std::string getValues() const;

		void setReadOnly(bool readOnly);

	private:
		QSpinBox * _spinBox;
	};

	class EditorTypeInteger : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6engine.editor.type.TypePluginInterface")
		Q_INTERFACES(i6engine::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "Integer";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPEINTEGER_H__ */
