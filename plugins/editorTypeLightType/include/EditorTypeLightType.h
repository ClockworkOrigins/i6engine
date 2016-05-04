#ifndef __I6ENGINE_PLUGINS_EDITORTYPELIGHTTYPE_H__
#define __I6ENGINE_PLUGINS_EDITORTYPELIGHTTYPE_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

#include "i6engine/api/components/LuminousAppearanceComponent.h"

class QComboBox;

namespace i6e {
namespace plugins {

	class EditorTypeLightTypeWidget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeLightTypeWidget(QWidget * parent);

		void setValues(const std::string & values);

		std::string getValues() const;

		void setReadOnly(bool readOnly);

	private:
		QComboBox * _comboBox;

		static std::map<QString, api::LuminousAppearanceComponent::LightType> lightTypes;
	};

	class EditorTypeLightType : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
		Q_INTERFACES(i6e::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "LightType";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPELIGHTTYPE_H__ */
