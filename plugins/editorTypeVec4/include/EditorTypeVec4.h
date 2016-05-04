#ifndef __I6ENGINE_PLUGINS_EDITORTYPEVEC4_H__
#define __I6ENGINE_PLUGINS_EDITORTYPEVEC4_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

class QLineEdit;

namespace i6e {
namespace plugins {

	class EditorTypeVec4Widget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeVec4Widget(QWidget * parent);

		void setValues(const std::string & values);

		std::string getValues() const;

		void setReadOnly(bool readOnly);

	private:
		QLineEdit * _lineEditX;
		QLineEdit * _lineEditY;
		QLineEdit * _lineEditZ;
		QLineEdit * _lineEditW;
	};

	class EditorTypeVec4 : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
		Q_INTERFACES(i6e::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "Vec4";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPEVEC4_H__ */
