#ifndef __I6ENGINE_PLUGINS_EDITORTYPEDOUBLE_H__
#define __I6ENGINE_PLUGINS_EDITORTYPEDOUBLE_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

class QDoubleSpinBox;

namespace i6engine {
namespace plugins {

	class EditorTypeDoubleWidget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeDoubleWidget(QWidget * parent);

		void setValues(const std::string & values);

		std::string getValues() const;

		void setReadOnly(bool readOnly);

	private:
		QDoubleSpinBox * _spinBox;
	};

	class EditorTypeDouble : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6engine.editor.type.TypePluginInterface")
		Q_INTERFACES(i6engine::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "Double";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPEDOUBLE_H__ */
