#ifndef __I6ENGINE_PLUGINS_EDITORTYPEMOVERINTERPOLATEWAY_H__
#define __I6ENGINE_PLUGINS_EDITORTYPEMOVERINTERPOLATEWAY_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

#include "i6engine/api/components/MoverInterpolateComponent.h"

class QComboBox;

namespace i6engine {
namespace plugins {

	class EditorTypeMoverInterpolateWayWidget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeMoverInterpolateWayWidget(QWidget * parent);

		void setValues(const std::string & values);

		std::string getValues() const;

		void setReadOnly(bool readOnly);

	private:
		QComboBox * _comboBox;

		static std::map<QString, api::MoverInterpolateComponent::Way> ways;
	};

	class EditorTypeMoverInterpolateWay : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6engine.editor.type.TypePluginInterface")
		Q_INTERFACES(i6engine::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "MoverInterpolateWay";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPEMOVERINTERPOLATEWAY_H__ */
