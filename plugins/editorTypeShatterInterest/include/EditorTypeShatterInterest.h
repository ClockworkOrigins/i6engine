#ifndef __I6ENGINE_PLUGINS_EDITORTYPESHATTERINTEREST_H__
#define __I6ENGINE_PLUGINS_EDITORTYPESHATTERINTEREST_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

#include <cstdint>

class QCheckBox;

namespace i6e {
namespace api {
	enum ShatterInterest : uint16_t;
} /* namespace api */
namespace plugins {

	class EditorTypeShatterInterestWidget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeShatterInterestWidget(QWidget * parent);

		void setValues(const std::string & values);

		std::string getValues() const;

		void setReadOnly(bool readOnly);

	private:
		std::map<QString, QCheckBox *> _checkBoxes;

		static std::map<QString, api::ShatterInterest> shatterInterests;
	};

	class EditorTypeShatterInterest : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
		Q_INTERFACES(i6e::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "ShatterInterest";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPESHATTERINTEREST_H__ */
