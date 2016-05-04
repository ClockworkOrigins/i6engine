#ifndef __I6ENGINE_PLUGINS_EDITORTYPEMOVERINTERPOLATEMODE_H__
#define __I6ENGINE_PLUGINS_EDITORTYPEMOVERINTERPOLATEMODE_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

#include "i6engine/api/components/MoverInterpolateComponent.h"

class QComboBox;

namespace i6e {
namespace plugins {

	class EditorTypeMoverInterpolateModeWidget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeMoverInterpolateModeWidget(QWidget * parent);

		void setValues(const std::string & values);

		std::string getValues() const;

		void setReadOnly(bool readOnly);

	private:
		QComboBox * _comboBox;

		static std::map<QString, api::MoverInterpolateComponent::Mode> modes;
	};

	class EditorTypeMoverInterpolateMode : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
		Q_INTERFACES(i6e::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "MoverInterpolateMode";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPEMOVERINTERPOLATEMODE_H__ */
